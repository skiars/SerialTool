#include "qvterminal.h"

#include <QKeyEvent>
#include <QPainter>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QDebug>
#include <QMenu>

QVTerminal::QVTerminal(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    _device = Q_NULLPTR;

    _cursorPos.setX(0);
    _cursorPos.setY(0);
    //_cursorTimer.start(500);
    _cvisible = true;
    connect(&_cursorTimer, &QTimer::timeout, this, &QVTerminal::toggleCursor);

    _echo = false;
    _crlf = false;
    _state = QVTerminal::Text;
    setFormat(QVTCharFormat());
    _layout = new QVTLayout();
}

QVTerminal::~QVTerminal()
{
}

void QVTerminal::setIODevice(QIODevice *device)
{
    _device = device;
    if (_device) {
        connect(_device, &QIODevice::readyRead, this, &QVTerminal::read);
        read();
    }
}

void QVTerminal::appendData(const QByteArray &data)
{
    QByteArray text;

    setUpdatesEnabled(false);
    QByteArray::const_iterator it = data.cbegin();
    while (it != data.cend()) {
        QChar c = *it;
        switch (_state) {
        case QVTerminal::Text:
            switch (c.unicode()) {
            case '\033':
                // \033即\x1B即ASCII 27，ESC
                // 匹配ESC[，即CSI	，Control Sequence Introducer
                appendString(text);
                text.clear();
                _state = QVTerminal::Escape;
                break;
            case '\r':
                appendString(text);
                text.clear();
                _cursorPos.setX(0);
                break;
            case '\n':
                appendString(text);
                text.clear();
                moveCursor(0, 1);
                break;
            case '\b':
                appendString(text);
                text.clear();
                moveCursor(-1, 0);
                break;
            default:
                if (c.isPrint()) {
                    text.append(c);
                }
            }
            break;
        case QVTerminal::Escape:
            _formatValue = 0;
            if (c == '[') {
                _state = QVTerminal::Format;
            } else if (c == '(') {
                _state = QVTerminal::ResetFont;
            }
            break;
        case QVTerminal::Format:
            if (c >= '0' && c <= '9') {
                _formatValue = _formatValue * 10 + (c.cell() - '0');
            } else {
                if (c == ';' || c == 'm') {
                    if (_formatValue == 0) { // reset format
                        _curentFormat = _format;
                    } else if (_formatValue == 4) { // underline
                        _curentFormat.font()->setUnderline(true);
                    } else if (_formatValue == 7) { // reverse
                        QColor foreground = _curentFormat.foreground();
                        _curentFormat.setForeground(_curentFormat.background());
                        _curentFormat.setBackground(foreground);
                    } else if (_formatValue / 10 == 3) { // foreground
                        _curentFormat.setForeground(vt100color(_formatValue % 10 + '0'));
                    } else if (_formatValue / 10 == 4) { // background
                        _curentFormat.setBackground(vt100color(_formatValue % 10 + '0'));
                    }
                    if (c == ';') {
                        _formatValue = 0;
                        _state = QVTerminal::Format;
                    } else {
                        _state = QVTerminal::Text;
                    }
                }else if(c=='J') {
                    reduceString(-1);
                    _state = QVTerminal::Text;
                } else {
                    formatChar(c);
                    _state = QVTerminal::Text;
                }
            }
            break;
        case QVTerminal::ResetFont:
            _curentFormat = _format;
            _state = QVTerminal::Text;
            break;
        }
        it++;
    }
    appendString(text);

    bool updataScrollBar = false;
    if(verticalScrollBar()->value() == verticalScrollBar()->maximum()){
        updataScrollBar = true;
    }
    
    verticalScrollBar()->setRange(0, _ch * (_layout->lineCount() + 1) - viewport()->size().height());
    if(updataScrollBar){
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }

    setUpdatesEnabled(true);
    update();
}

void QVTerminal::formatChar(const QChar &c)
{
    switch (c.unicode()) {
    case 'K':
        clearToEnd();
        break;
    case 'A':
        moveCursor(0, -_formatValue);
        break;
    case 'B':
        moveCursor(0, _formatValue);
        break;
    case 'C':
        moveCursor(_formatValue, 0);
        break;
    case 'D':
        moveCursor(-_formatValue, 0);
        break;
    default:
        break;
    }
}

void QVTerminal::moveCursor(int xpos, int ypos)
{
    _cursorPos += QPoint(xpos, ypos);
    if (_cursorPos.x() < 0) {
        _cursorPos.setX(0);
    }
    if (_cursorPos.y() < 0) {
        _cursorPos.setY(0);
    }
    if (_cursorPos.y() >= _layout->lineCount()) {
        _layout->appendLine(_cursorPos.y() - _layout->lineCount() + 1);
    }
}

void QVTerminal::copy(){
    if(chooseText.size() > 0){
        qDebug() << "choose text: " << chooseText.data();
        QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
        clipboard->setText(chooseText.data());
        chooseText.clear();
    }
}

void QVTerminal::paste()
{
    QByteArray data;
    data.append(QApplication::clipboard()->text());
    writeData(data);
}

QColor QVTerminal::vt100color(char c)
{
    switch (c) {
    case '1': return QColor(Qt::red);
    case '2': return QColor(Qt::green);
    case '3': return QColor(Qt::yellow);
    case '4': return QColor(Qt::blue);
    case '5': return QColor(Qt::magenta);
    case '6': return QColor(Qt::cyan);
    case '7': return QColor(Qt::white);
    default:  return QColor(Qt::black);
    }
}

void QVTerminal::read()
{
    if (!_device)
        return;
    if (_device->isReadable())
        appendData(_device->readAll());
}

void QVTerminal::appendString(QString str)
{
    foreach (QChar c, str) {
        QVTChar termChar(c, _curentFormat);
        _layout->lineAt(_cursorPos.y()).append(termChar, _cursorPos.x());
        _cursorPos.setX(_cursorPos.x() + 1);
    }
}

void QVTerminal::reduceString(int mode)
{
    if(mode > 0)
        _layout->lineAt(_cursorPos.y()).reduce(_cursorPos.x()+1);
    else{
        _layout->lineAt(_cursorPos.y()).reduce(_cursorPos.x());
    //   _cursorPos.setX(_cursorPos.x() -1);
    }
}

void QVTerminal::toggleCursor()
{
    _cvisible = !_cvisible;
    viewport()->update();
}

void QVTerminal::clearToEnd()
{
    _layout->lineAt(_cursorPos.y()).reserve(_cursorPos.x());
}

bool QVTerminal::crlf() const
{
    return _crlf;
}

void QVTerminal::setCrlf(bool crlf)
{
    _crlf = crlf;
}

void QVTerminal::writeData(QByteArray data)
{
//    _device->write(data);
    transmitData(data);
    if (_echo) {
        appendData(data);
    }
}

bool QVTerminal::echo() const
{
    return _echo;
}

void QVTerminal::setEcho(bool echo)
{
    _echo = echo;
}

QVTCharFormat* QVTerminal::format()
{
    return &_format;
}

void QVTerminal::setFormat(const QVTCharFormat &format)
{
    _format = format;
    _curentFormat = format;
    QFontMetrics fm(*_format.font());
    _cw = fm.boundingRect('M').width();
    _ch = fm.height();
    _cascent = fm.ascent();
}

bool QVTerminal::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent*>(event));
        return true;
    }
    return QAbstractScrollArea::event(event);
}

void QVTerminal::keyPressEvent(QKeyEvent *event)
{
    QByteArray data;
    switch (event->key()) {
    case Qt::Key_Up:
        data.append("\033[A");
        break;
    case Qt::Key_Down:
        data.append("\033[B");
        break;
    case Qt::Key_Right:
        data.append("\033[C");
        break;
    case Qt::Key_Left:
        data.append("\033[D");
        break;
    case Qt::Key_Home:
        data.append('\x01');
        break;
    case Qt::Key_End:
        data.append('\x05');
        break;
    case Qt::Key_Tab:
        data.append('\t');
        break;
    case Qt::Key_Backspace:
        data.append('\b');
        break;
    // ref.PC-Style Function Keys
    // https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
    case Qt::Key_Delete:
        data.append("\033[3~");
        break;
    case Qt::Key_Insert:
        data.append("\033[2~");
        break;
    case Qt::Key_PageUp:
        data.append("\033[5~");
        break;
    case Qt::Key_PageDown:
        data.append("\033[6~");
        break;
    case Qt::Key_Return:
        data.append('\n');
        break;
    default:
        data.append(event->text().toUtf8());
        QAbstractScrollArea::keyPressEvent(event);
    }
    transmitData(data);
}

void QVTerminal::paintEvent(QPaintEvent */* paintEvent */)
{
    QPainter p(viewport());
    p.setPen(QColor(187, 187, 187));
    p.setBrush(QColor(0x23, 0x26, 0x29));
    p.setFont(*_format.font());

    p.fillRect(viewport()->rect(), QColor(0x23, 0x26, 0x29));

    QPoint pos(0, 0);
    QByteArray text;

    QPoint chooseStart(-1, -1);
    QPoint chooseEnd(-1, -1);

    int firstLine = verticalScrollBar()->value() / _ch;
    int lastLine = viewport()->size().height() / _ch + firstLine;
    if (lastLine > _layout->lineCount()) {
        lastLine = _layout->lineCount();
    }

    QPoint curPos(_cursorPos.x() * _cw, (_cursorPos.y() - firstLine) * _ch);

    // draw cursor
    if (_cvisible && chooseSatus != 2 && chooseSatus != 3) {
        p.fillRect(QRect(curPos, QSize(_cw, _ch)), _format.foreground());
    }


    int diff_l = chooseLine - firstLine;
    if(diff_l >0){
        choosePosStart.setY(choosePosStart.y() + (diff_l * _ch));
        if(chooseSatus == 3){
            choosePosEnd.setY(choosePosEnd.y() + (diff_l * _ch));
        }
    }else if(diff_l < 0){
        choosePosStart.setY(choosePosStart.y() + (diff_l * _ch));
        if(chooseSatus == 3){
            choosePosEnd.setY(choosePosEnd.y() + (diff_l * _ch));
        }
    }
    chooseLine = firstLine;

    int diff_y = choosePosStart.y() - choosePosEnd.y();
    if((-_ch <= diff_y && diff_y <= _ch && choosePosStart.x() > choosePosEnd.x()) || (choosePosStart.y() > (choosePosEnd.y() + _ch))){
        chooseStart = choosePosEnd;
        chooseEnd = choosePosStart;
    }else{
        chooseStart = choosePosStart;
        chooseEnd = choosePosEnd;
    }

    bool chooseflag = 0;
    int chooseLastLine = -1;

    chooseText.clear();

    // draw text
    for (int l = firstLine; l < lastLine; l++) {
        pos.setX(0);
        for (auto vtc : _layout->lineAt(l).chars()) {
            if(chooseSatus == 2 || chooseSatus == 3){
                if(chooseflag == 0){
                    if((chooseStart.y() - _ch) < pos.y() && pos.y() <= (chooseStart.y()) && pos.x() > (chooseStart.x()-_cw/2) && pos.x() < chooseStart.x()+_cw){
                        chooseflag = 1;
                    }
                }else if(chooseflag == 1){
                    if((pos.y() > (chooseEnd.y()- _ch) && pos.x() > chooseEnd.x()) || (pos.y() > (chooseEnd.y()))){
                        chooseflag = 0;
                    }
                }

                if(chooseflag == 1){
                    p.fillRect(QRect(pos, QSize(_cw, _ch)), _format.foreground());
                    p.setPen(vtc.background());
                    if(chooseLastLine != -1){
                        if(chooseLastLine != l){
                            chooseLastLine = l;
                            chooseText.append("\r\n");
                        }
                    }else{
                        chooseLastLine = l;
                    }
                    chooseText.append(vtc.c());
                }else {
                    p.setPen(pos == curPos ? vtc.background() : vtc.foreground());
                }

            }else{
                p.setPen(pos == curPos ? vtc.background() : vtc.foreground());
            }

            p.drawText(pos.x(), pos.y() + _cascent, vtc.c());
            //p.setBrush(QBrush());
            //p.drawRect(QRect(pos, QSize(_cw, _ch)));
            pos.setX(pos.x() + _cw);
        }
        pos.setY(pos.y() + _ch);
    }
}

void QVTerminal::resizeEvent(QResizeEvent */* event */)
{
    QScrollBar *bar = verticalScrollBar();
    int pos = bar->maximum() - bar->value();
    bar->setPageStep(_ch * 10);
    bar->setSingleStep(_ch);
    bar->setRange(0, _ch * (_layout->lineCount() + 1) - viewport()->size().height());
    bar->setValue(bar->maximum() - pos);
}

void QVTerminal::mouseReleaseEvent(QMouseEvent *event){
    chooseSatus = 3;
    choosePosEnd = event->pos();
    setUpdatesEnabled(false);
    setUpdatesEnabled(true);
    update();
}

void QVTerminal::mouseMoveEvent(QMouseEvent *event){
    chooseSatus = 2;
    choosePosEnd = event->pos();
    setUpdatesEnabled(false);
    setUpdatesEnabled(true);
    update();
    QWidget::mouseMoveEvent(event);
}

void QVTerminal::mousePressEvent(QMouseEvent *event)
{
    choosePosStart = event->pos();
    if (event->button() == Qt::RightButton) {
        if(chooseSatus == 2 || chooseSatus == 3){
            QVTerminal::copy();
        }else{
            QVTerminal::paste();
        }
    }else if (event->button() == Qt::LeftButton) {
        chooseSatus = 1;
        chooseLine = verticalScrollBar()->value() / _ch;
    }
    chooseSatus = 0;
    QWidget::mousePressEvent(event);
}

//#ifndef QT_NO_CONTEXTMENU
void QVTerminal::contextMenuEvent(QContextMenuEvent *event)
{

}
//#endif // QT_NO_CONTEXTMENU

bool QVTerminal::viewportEvent(QEvent *event)
{
    return QAbstractScrollArea::viewportEvent(event);
}


void QVTerminal::clear()
{
//    if(_enabled){
        delete _layout;
        _layout = new QVTLayout();
        _cursorPos.setX(0);
        _cursorPos.setY(0);
        setUpdatesEnabled(false);
        setUpdatesEnabled(true);
        update();
//    }
}

void QVTerminal::setEnabled(bool enabled)
{
    _enabled = enabled;
}
