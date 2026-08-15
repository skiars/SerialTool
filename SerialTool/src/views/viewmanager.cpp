#include "viewmanager.h"
#include <QSettings>
#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
#include <QDockWidget>
#include <QPluginLoader>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "abstractview.h"
#include "scriptextension/scriptextensionview.h"
#include "texttr/texttrview.h"
#include "terminal/terminalview.h"
#include "oscilloscope/oscilloscopeview.h"
#include "filetransmit/filetransmitview.h"

ViewManager::ViewManager(QString *docPath, QMainWindow *window) :
    m_window(window),
    m_docPath(docPath)
{
    m_views = new QVector<AbstractView *>;

    // create views
    m_views->append(new TextTRView());
    m_views->append(new TerminalView());
    m_views->append(new OscilloscopeView());
    m_views->append(new FileTransmitView());

    m_views->append(loadExtensions("extensions"));
    delete window->takeCentralWidget();
    window->setDockNestingEnabled(true);

    // 左右双栏布局：TextTR 主视图停靠左侧，其余视图 Tab 化停靠右侧
    QDockWidget *leftDock = nullptr;
    QDockWidget *rightAlign = nullptr;
    for (AbstractView *view : *m_views) {
        QDockWidget *dock = new QDockWidget(view->title(), window);
        dock->setObjectName(view->iid());
        dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dock->setWidget(view);
        if (view->iid() == "text_tx/rx") {
            window->addDockWidget(Qt::LeftDockWidgetArea, dock);
            leftDock = dock;
        } else {
            if (rightAlign == nullptr) {
                window->addDockWidget(Qt::RightDockWidgetArea, dock);
                rightAlign = dock;
            } else {
                window->tabifyDockWidget(rightAlign, dock);
            }
        }
        connect(view, &AbstractView::transmitData, this, &ViewManager::transmitData);
        connect(view, &AbstractView::sendMessage, this, &ViewManager::dispatchMessage);
    }
    // 首次布局默认 60/40 分栏（后续运行由 restoreState 恢复用户自定义）
    if (leftDock && rightAlign) {
        int total = window->width() - 40;
        window->resizeDocks({leftDock, rightAlign},
                            {total * 3 / 5, total * 2 / 5}, Qt::Horizontal);
    }
}

ViewManager::~ViewManager()
{
    for (AbstractView *view : *m_views) {
        delete view;
    }
    delete m_views;
}

void ViewManager::loadConfig(QSettings *config)
{
    for (AbstractView *view : *m_views) {
        view->loadConfig(config);
    }
}

void ViewManager::saveConfig(QSettings *config)
{
    for (AbstractView *view : *m_views) {
        view->saveConfig(config);
    }
}

void ViewManager::loadSettings(QSettings *config)
{
    for (AbstractView *view : *m_views) {
        view->loadSettings(config);
    }
}

void ViewManager::retranslate()
{
    for (int i = 0; i < m_views->size(); ++i) {
        AbstractView *view = m_views->at(i);
        view->retranslate();
    }
}

void ViewManager::dispatchMessage(const QString &receiver, const QByteArray &message)
{
    AbstractView *sender = dynamic_cast<AbstractView *>(QObject::sender());
    for (AbstractView *view : *m_views) {
        if (view->iid() == receiver || receiver.isEmpty()) {
            view->takeMessage(sender->iid(), message);
        }
    }
}

void ViewManager::receiveData(const QByteArray &array)
{
    // 所有视图都应收到数据, 否则 Tab 化停靠时不可见的视图会丢失数据
    for (AbstractView *view : *m_views) {
        view->receiveData(array);
    }
}

void ViewManager::setEnabled(bool enabled)
{
    for (AbstractView *view : *m_views) {
        view->setEnabled(enabled);
    }
}

void ViewManager::clear(void)
{
    for (AbstractView *view : *m_views) {
        view->clear();
    }
}

void ViewManager::setFileAction(QAction *openAction, QAction *saveAction)
{
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
}

AbstractView* ViewManager::findActiveView()
{
    QWidget* wid = QApplication::focusWidget();
    AbstractView* view = nullptr;
    while (view == nullptr && wid != nullptr) {
        view = dynamic_cast<AbstractView *>(wid);
        if (view) {
            break; // its a AbstractView
        }
        wid = dynamic_cast<QWidget *>(wid->parent());
    }
    return view;
}

void ViewManager::saveFile()
{
    QString filter;
    AbstractView *view = findActiveView();
    if (view && !view->saveFileFilter().isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(view, tr("Save"), *m_docPath,
            view->saveFileFilter(), &filter,
            QFileDialog::HideNameFilterDetails);
        if (fileName.isEmpty()) {
            return;
        }
        *m_docPath = QFileInfo(fileName).path();
        view->saveFile(fileName, filter);
    } else {
        QMessageBox::warning(m_window, tr("Warning"),
                             tr("This view does not support this operation."));
    }
}

void ViewManager::openFile()
{
    QString filter;
    AbstractView *view = findActiveView();
    if (view && !view->openFileFilter().isEmpty()) {
        QString fileName = QFileDialog::getOpenFileName(view, tr("Open"), *m_docPath,
            view->openFileFilter(), &filter,
            QFileDialog::HideNameFilterDetails);
        if (fileName.isEmpty()) {
            return;
        }
        *m_docPath = QFileInfo(fileName).path();
        view->openFile(fileName, filter);
    } else {
        QMessageBox::warning(m_window, tr("Warning"),
                             tr("This view does not support this operation."));
    }
}

QVector<AbstractView *> ViewManager::loadExtensions(const QString &path)
{
    QDir dir(path);
    QVector<AbstractView *> list;
    for (QString fileName : dir.entryList(QDir::Files)) {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        AbstractView *view = dynamic_cast<AbstractView *>(loader.instance());
        if (view) {
            qDebug() << "file:" << fileName;
            view->setParent(m_window);
            list.append(view);
        }
    }
    for (QString fileName : dir.entryList(QStringList("*.js"), QDir::Files)) {
        QString name = dir.absoluteFilePath(fileName);
        AbstractView *view = new ScriptExtensionView(name, m_window);
        if (view) {
            list.append(view);
        }
    }
    return list;
}
