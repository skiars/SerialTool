#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>

class AbstractView;
class QTabWidget;
class QAction;
class QSettings;

class ViewManager : public QObject
{
    Q_OBJECT
public:
    explicit ViewManager(QString *docPath, QTabWidget *tabWidget);
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
    void tabIndexChanged(int index);
    void saveFile();
    void openFile();

private:
    QVector<AbstractView *> loadExtensions(const QString &path);

private:
    QVector<AbstractView *> *m_views;
    AbstractView *m_currentView;
    QAction *m_openFileAction = nullptr, *m_saveFileAction = nullptr;
    QTabWidget *m_tabWidget;
    QString *m_docPath;
};

#endif // VIEWMEDIATOR_H
