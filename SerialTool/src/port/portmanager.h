#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QObject>

class QAction;
class QToolBar;
class QSettings;
class AbstractPort;
class AbstractPortFactory;

class PortManager : public QObject
{
    Q_OBJECT
public:
    explicit PortManager(QToolBar *toolbar, QAction *portSetAction);
    ~PortManager();
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    bool open();
    void close();
    QByteArray readAll();
    void write(const QByteArray &data);
    bool portStatus(QString *string);
    bool isOpen();
    void portSetDialog();
    QString autoOpen(bool open);

signals:
    void readyRead();
    void portError();
    void portChanged();



private:
    void setupPort(AbstractPort *port);

private:
    QVector<AbstractPortFactory *> *m_portFactorys;
    AbstractPort *m_currentPort;
    QString m_currentPortType;
    QAction *m_portSetAction;
    QToolBar *m_toolbar;
    QSettings *m_config;
};

#endif // PORTMANAGER_H
