#ifndef ABSTRACTPORT_H
#define ABSTRACTPORT_H

#include <QWidget>

class QSettings;

class AbstractPort : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractPort(QWidget *parent = nullptr) : QWidget(parent) {}

    virtual void retranslate() {}
    virtual void loadConfig(QSettings *config) = 0;
    virtual void saveConfig(QSettings *config) = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual QByteArray readAll() = 0;
    virtual void write(const QByteArray &data) = 0;
    virtual bool portStatus(QString *string) = 0;
    virtual bool isOpen() = 0;
    virtual bool usePortSetDialog() { return false; }
    virtual void portSetDialog() {}
    virtual QString autoOpen(bool open)=0;
signals:
    void readyRead();
    void portError();
    void portChanged();
};

class AbstractPortFactory
{
public:
    AbstractPortFactory() {}
    virtual ~AbstractPortFactory() {}
    virtual AbstractPort* create(QWidget *parent = nullptr) = 0;
    virtual QString type() = 0;
};

#endif // ABSTRACTPORT_H
