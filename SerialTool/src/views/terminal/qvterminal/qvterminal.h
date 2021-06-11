#ifndef QVTERMINAL_H
#define QVTERMINAL_H

#include <QAbstractScrollArea>
#include <QAction>
#include <QTimer>
#include <QWidget>

#include "qvtlayout.h"

class QVTerminal : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit QVTerminal(QWidget *parent = nullptr);
    ~QVTerminal();

    void setIODevice(QIODevice *device);

    // style
    QVTCharFormat* format();
    void setFormat(const QVTCharFormat &format);

    // mode
    bool echo() const;
    void setEcho(bool echo);

    bool crlf() const;
    void setCrlf(bool crlf);

    void setTextCodec(const QString &name);
    void setTabWidth(int width);

signals:
    void transmitData(const QByteArray &data);

public slots:
    void writeData(QByteArray data);

    void copy();
    void paste();
    void appendData(const QByteArray &data);

    void clear();

    void setEnabled(bool);

protected slots:
    void read();
    void appendString(QByteArray array);
    void reduceString(int position);
    void toggleCursor();
    void clearToEnd();

private:
    void formatChar(const QChar &c);
    void moveCursor(int xpos, int ypos);

private:
    QIODevice *_device;

    // parser
    enum State {
        Text,
        Escape,
        Format,
        ResetFont
    };
    State _state;
    int _formatValue;

    bool _enabled;

    // cursor
    QVTCharFormat _format;
    QVTCharFormat _curentFormat;
    int _cw, _ch, _cascent;
    QPoint _cursorPos;
    QTimer _cursorTimer;
    bool _cvisible;

    // data
    QVTLayout *_layout;

    // mode
    bool _echo;
    bool _crlf;

    int _chooseSatus = false;
    QByteArray _chooseText;
    QPoint _choosePosStart;
    QPoint _choosePosEnd;

    QString _textCodec;
    int _tabWidth;

    // QWidget interface
protected:
    virtual bool event(QEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
#ifndef QT_NO_CONTEXTMENU
    virtual void contextMenuEvent(QContextMenuEvent *event);
#endif // QT_NO_CONTEXTMENU

    // QAbstractScrollArea interface
protected:
    virtual bool viewportEvent(QEvent *event);
    QColor vt100color(char c);

//    QAction *_pasteAction;
//    QAction *_copyAction;
};

#endif // QVTERMINAL_H
