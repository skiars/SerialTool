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
    // 记录并退出当前分组, 使端口配置可以在各自分组下正确读写,
    // 完成后恢复原分组, 不再依赖调用方的分组状态
    QString group = config->group();
    QString type = config->value("PortType").toString();

    if (!group.isEmpty()) {
        config->endGroup();
    }
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
    if (!group.isEmpty()) {
        config->beginGroup(group);
    }
}

bool PortManager::open()
{
    return m_currentPort->open();
}

void PortManager::reset()
{
    return m_currentPort->reset();
}

void PortManager::close()
{
    return m_currentPort->close();
}

void PortManager::setAutoOpen(bool enable){
    m_currentPort->setAutoOpen(enable);
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
