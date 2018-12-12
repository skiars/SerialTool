#include "toolboxmanager.h"
#include "abstracttoolbox.h"
#include "vediobox/vediobox.h"
#include "valuedisplay/valuedisplay.h"
#include <QMenu>
#include <QAction>

ToolBoxManager::ToolBoxManager(QString *docPath, QMenu *menu)
{
    m_actions = new QVector<QAction *>;
    m_factorys = new QVector<AbstractToolBoxFactory *>;
    m_docPath = docPath;
    m_parent = menu;

    m_factorys->append(new ValueDisplayFactory());
    m_factorys->append(new VedioBoxFactory());

    for (AbstractToolBoxFactory *factory : *m_factorys) {
        QAction *action = new QAction(menu);

        action->setText(factory->title());
        m_actions->append(action);
        menu->addAction(action);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(onActionTriggered()));
    }
}

ToolBoxManager::~ToolBoxManager()
{
    for (AbstractToolBoxFactory *factory : *m_factorys) {
        delete factory;
    }
    delete m_factorys;
    delete m_actions;
}

void ToolBoxManager::onActionTriggered()
{
    int index = m_actions->indexOf((QAction *)sender());
    AbstractToolBoxFactory *factory = m_factorys->at(index);

    if (factory->product() == nullptr) {
        AbstractToolBox *toolBox = factory->create(m_parent);

        factory->setProduct(toolBox);
        toolBox->setModal(false); // 非模态对话框
        toolBox->setAttribute(Qt::WA_DeleteOnClose);
        toolBox->setFilePath(*m_docPath);
        toolBox->setWindowOpacity(m_opacity);
        toolBox->show();
        connect(toolBox, SIGNAL(transmitData(const QByteArray &)),
                this, SIGNAL(transmitData(const QByteArray &)));
    }
}

void ToolBoxManager::retranslate()
{
    for (int i = 0; i < m_factorys->size(); ++i) {
        AbstractToolBoxFactory *factory = m_factorys->at(i);

        if (factory->product() != nullptr) {
            factory->product()->retranslate();
        }
        m_actions->at(i)->setText(factory->title());
    }
}

void ToolBoxManager::receiveData(const QByteArray &array)
{
    for (AbstractToolBoxFactory *factory : *m_factorys) {
        if (factory->product() != nullptr) {
            factory->product()->receiveData(array);
        }
    }
}

void ToolBoxManager::setWindowOpacity(qreal level)
{
    m_opacity = level;
    for (AbstractToolBoxFactory *factory : *m_factorys) {
        if (factory->product() != nullptr) {
            factory->product()->setWindowOpacity(level);
        }
    }
}
