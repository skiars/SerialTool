#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>

class QSettings;

class AbstractView : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractView(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual void loadConfig(QSettings */*config*/) {}
    virtual void saveConfig(QSettings */*config*/) {}
    virtual void loadSettings(QSettings */*config*/) {}
    virtual void retranslate() {}
    virtual QString title() { return "Abstract View"; }
    virtual void receiveData(const QByteArray &/*array*/) {}
    virtual bool holdReceive() { return false; }
    virtual void setEnabled(bool /*enabled*/) {}
    virtual void clear() {}

    virtual QString openFileFilter() { return QString(); }
    virtual QString saveFileFilter() { return QString(); }
    virtual void saveFile(const QString &fileName, const QString &filter) {
        (void)fileName; (void)filter;
    }
    virtual void openFile(const QString &fileName, const QString &filter) {
        (void)fileName; (void)filter;
    }

signals:
    void transmitData(const QByteArray &);

public slots:
};

Q_DECLARE_INTERFACE(AbstractView, "gztss.SerialTool.AbstractView/1.0")

#endif // ABSTRACTVIEW_H
