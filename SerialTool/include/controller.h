#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class QMenu;
class QAction;
class QSettings;
class QMainWindow;
class PortManager;
class ViewManager;
class ToolBoxManager;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QString *docPath, QMainWindow *window,
                        QMenu *menu, QAction *openAction, QAction *saveAction);
    void setPortManager(PortManager *manager);
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    void retranslate();
    void setEnabled(bool enabled);
    void clear(void);
    void setPause(bool paused) { m_pause = paused; }
    int receiveCount() { return m_rxCount; }
    int transmitCount () { return m_txCount; }
    void setWindowOpacity(qreal level);

signals:

private slots:
    void readPortData();
    void writePortData(const QByteArray &array);

private:
    ViewManager *m_views;
    ToolBoxManager *m_toolBoxs;
    PortManager *m_port;
    bool m_pause = false;
    int m_rxCount = 0, m_txCount = 0;
};

#endif // CONTROLLER_H
