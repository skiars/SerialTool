#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>

class AbstractView;
class QAction;
class QSettings;
class QMainWindow;

class ViewManager : public QObject
{
    Q_OBJECT
public:
    explicit ViewManager(QString *docPath, QMainWindow *window);
    ~ViewManager();

    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    void retranslate();
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);
    void clear(void);
    void setFileAction(QAction *openAction, QAction *saveAction);

signals:
    void transmitData(const QByteArray &);

private slots:
    void dispatchMessage(const QString &receiver, const QByteArray &message);
    void saveFile();
    void openFile();

private:
    QVector<AbstractView *> loadExtensions(const QString &path);
    AbstractView* findActiveView();

private:
    struct Hotspot {
        AbstractView *view;
        int postion;
    };
    QVector<AbstractView *> *m_views;
    QMainWindow *m_window;
    QString *m_docPath;
};

#endif // VIEWMEDIATOR_H
