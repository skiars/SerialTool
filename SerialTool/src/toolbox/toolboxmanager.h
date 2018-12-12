#ifndef TOOLBOXMANAGER_H
#define TOOLBOXMANAGER_H

#include <QObject>

class QMenu;
class QAction;
class QWidget;
class AbstractToolBox;
class AbstractToolBoxFactory;

class ToolBoxManager : public QObject
{
    Q_OBJECT
public:
    ToolBoxManager(QString *docPath, QMenu *menu);
    ~ToolBoxManager();

    void retranslate();
    void receiveData(const QByteArray &array);
    void setWindowOpacity(qreal level);

signals:
    void transmitData(const QByteArray &);

private slots:
    void onActionTriggered();

private:
    qreal m_opacity = 1.0;
    QWidget *m_parent;
    QString *m_docPath;
    QVector<QAction *> *m_actions;
    QVector<AbstractToolBoxFactory *> *m_factorys;
};

#endif // TOOLBOXEDIATOR_H
