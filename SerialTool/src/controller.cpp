#include "controller.h"
#include "views/viewmanager.h"
#include "toolbox/toolboxmanager.h"
#include "port/portmanager.h"
#include <QDebug>

Controller::Controller(QString *docPath, QMainWindow *window, QMenu *menu,
                       QAction *openAction, QAction *saveAction)
{
    m_views = new ViewManager(docPath, window);
    m_toolBoxs = new ToolBoxManager(docPath, menu);

    m_views->setFileAction(openAction, saveAction);
}

void Controller::loadConfig(QSettings *config)
{
    m_views->loadConfig(config);
}

void Controller::saveConfig(QSettings *config)
{
    m_views->saveConfig(config);
}

void Controller::loadSettings(QSettings *config)
{
    m_views->loadSettings(config);
}

void Controller::retranslate()
{
    m_views->retranslate();
    m_toolBoxs->retranslate();
}

void Controller::setEnabled(bool enabled)
{
    m_views->setEnabled(enabled);
}

void Controller::clear(void)
{
    m_rxCount = 0;
    m_txCount = 0;
    m_views->clear();
}

void Controller::setPortManager(PortManager *manager)
{
    m_port = manager;
    connect(m_port, &PortManager::readyRead, this, &Controller::readPortData);
    connect(m_views, &ViewManager::transmitData, this, &Controller::writePortData);
    connect(m_toolBoxs, &ToolBoxManager::transmitData, this, &Controller::writePortData);
}

void Controller::readPortData()
{
    if (m_pause == false) {
        QByteArray array = m_port->readAll();

        if (!array.isEmpty()) {
            m_rxCount += array.length();
            m_views->receiveData(array);
            m_toolBoxs->receiveData(array);
        }
    }
}

// 向端口发送数据,带参数
void Controller::writePortData(const QByteArray &array)
{
    m_txCount += array.length();
    m_port->write(array);
}

void Controller::setWindowOpacity(qreal level)
{
    m_toolBoxs->setWindowOpacity(level);
}
