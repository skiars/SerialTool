#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QWidget>

class QSettings;

class AbstractView : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractView(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual QString title() = 0;
    virtual QString iid() = 0; // Independent ID
    virtual void loadConfig(QSettings * /* config */) {}
    virtual void saveConfig(QSettings * /* config */) {}
    virtual void loadSettings(QSettings * /* config */) {}
    virtual void retranslate() {}
    virtual void receiveData(const QByteArray & /* array */) {}
    virtual void setEnabled(bool /*enabled*/) {}
    virtual void clear() {}

    virtual QString openFileFilter() { return QString(); }
    virtual QString saveFileFilter() { return QString(); }
    virtual void saveFile(const QString & /* fileName */, const QString & /* filter */) {}
    virtual void openFile(const QString & /* fileName */, const QString & /* filter */) {}

    virtual void takeMessage(const QString & /* sender */, const QByteArray & /* message */) {}

signals:
    void transmitData(const QByteArray &data);
    void sendMessage(const QString &receiver, const QByteArray &message);
};

Q_DECLARE_INTERFACE(AbstractView, "gztss.SerialTool.AbstractView/1.1")

#endif // ABSTRACTVIEW_H
