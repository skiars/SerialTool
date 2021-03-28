#include "portmanager.h"
#include "abstractport.h"
#include "serialport.h"
#include "tcpudpport.h"
#include <QSettings>
#include <QAction>
#include <QToolBar>
#include <QDebug>

PortManager::PortManager(QToolBar *toolbar, QAction *portSetAction)
{
    m_portFactorys = new QVector<AbstractPortFactory *>;

    m_toolbar = toolbar;
    m_portSetAction = portSetAction;

    m_portFactorys->append(new SerialPortFactory);
    m_portFactorys->append(new TcpUdpPortFactory);

    m_currentPortType = m_portFactorys->at(0)->type();
    setupPort(m_portFactorys->at(0)->create(toolbar));
}

PortManager::~PortManager()
{
    for (AbstractPortFactory *factory : *m_portFactorys) {
        delete factory;
    }
    delete m_portFactorys;
}

void PortManager::retranslate()
{
    m_currentPort->retranslate();
}

void PortManager::loadConfig(QSettings *config)
{
    m_config = config;
    m_currentPort->loadConfig(config);
}

void PortManager::saveConfig(QSettings *config)
{
    m_currentPort->saveConfig(config);
}

void PortManager::loadSettings(QSettings *config)
{
    QString type = config->value("PortType").toString();

    m_config->endGroup(); // Group "Settings"
    for (AbstractPortFactory *factory : *m_portFactorys) {
        if (factory->type() == type && type != m_currentPortType) {
            m_currentPortType = type;
            if (m_currentPort->isOpen()) {
                m_currentPort->close();
                emit portError(); // close port action
            }
            m_currentPort->saveConfig(config);
            delete m_currentPort;
            setupPort(factory->create(m_toolbar));
            m_currentPort->loadConfig(config);
            break;
        }
    }
    m_config->beginGroup("Settings");
}

QString PortManager::autoOpen(bool open){
    return  m_currentPort->autoOpen(open);
}

bool PortManager::open()
{
    return m_currentPort->open();
}

void PortManager::close()
{
    return m_currentPort->close();
}

QByteArray PortManager::readAll()
{
    return m_currentPort->readAll();
}

void PortManager::write(const QByteArray &data)
{
    m_currentPort->write(data);
}

bool PortManager::portStatus(QString *string)
{
    return m_currentPort->portStatus(string);
}

bool PortManager::isOpen()
{
    return m_currentPort->isOpen();
}

void PortManager::portSetDialog()
{
    m_currentPort->portSetDialog();
}

void PortManager::setupPort(AbstractPort *port)
{
    m_currentPort = port;
    m_toolbar->insertWidget(m_portSetAction, port);

    if (m_currentPort->usePortSetDialog()) {
        m_portSetAction->setVisible(true);
        connect(m_portSetAction, &QAction::triggered, m_currentPort, &AbstractPort::portSetDialog);
    } else {
        m_portSetAction->setVisible(false);
    }

    connect(port, &AbstractPort::readyRead, this, &PortManager::readyRead);
    connect(port, &AbstractPort::portError, this, &PortManager::portError);
    connect(port, &AbstractPort::portChanged, this, &PortManager::portChanged);
}
