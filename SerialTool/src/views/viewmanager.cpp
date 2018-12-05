#include "viewmanager.h"
#include <QSettings>
#include <QTabWidget>
#include <QAction>
#include <QFileDialog>
#include "abstractview.h"
#include "terminal/texttrview.h"
#include "oscilloscope/oscilloscopeview.h"
#include "filetransmit/filetransmitview.h"

ViewManager::ViewManager(QString *docPath, QTabWidget *tabWidget)
{
    m_views = new QVector<AbstractView *>;

    // create views
    m_views->append(new TextTRView());
    m_views->append(new OscilloscopeView());
    m_views->append(new FileTransmitView());

    m_docPath = docPath;
    m_tabWidget = tabWidget;
    m_currentView = m_views->at(0);
    for (AbstractView *view : *m_views) {
        tabWidget->addTab(view, view->title());
        connect(view, &AbstractView::transmitData, this, &ViewManager::transmitData);
    }
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));
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
        m_tabWidget->setTabText(i, view->title());
    }
}

void ViewManager::receiveData(const QByteArray &array)
{
    for (AbstractView *view : *m_views) {
        if (view->holdReceive() || m_currentView == view) {
            view->receiveData(array);
        }
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
    m_currentView->clear();
}

void ViewManager::setFileAction(QAction *openAction, QAction *saveAction)
{
    m_openFileAction = openAction;
    m_saveFileAction = saveAction;

    m_openFileAction->setEnabled(!m_currentView->openFileFilter().isEmpty());
    m_saveFileAction->setEnabled(!m_currentView->saveFileFilter().isEmpty());
    connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(m_saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));
}

void ViewManager::tabIndexChanged(int index)
{
    AbstractView *view = m_views->at(index);

    m_currentView = view;
    m_openFileAction->setEnabled(!m_currentView->openFileFilter().isEmpty());
    m_saveFileAction->setEnabled(!m_currentView->saveFileFilter().isEmpty());
}

void ViewManager::saveFile()
{
    QString filter;
    QString fileName = QFileDialog::getSaveFileName(m_currentView, tr("Save"), *m_docPath,
        m_currentView->saveFileFilter(), &filter,
        QFileDialog::HideNameFilterDetails);
    if (fileName.isEmpty()) {
        return;
    }
    *m_docPath = QFileInfo(fileName).path();
    m_currentView->saveFile(fileName, filter);
}

void ViewManager::openFile()
{
    QString filter;
    QString fileName = QFileDialog::getOpenFileName(m_currentView, tr("Open"), *m_docPath,
        m_currentView->openFileFilter(), &filter,
        QFileDialog::HideNameFilterDetails);
    if (fileName.isEmpty()) {
        return;
    }
    *m_docPath = QFileInfo(fileName).path();
    m_currentView->openFile(fileName, filter);
}
