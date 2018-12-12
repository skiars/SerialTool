#ifndef ABSTRACTTOOLBOX_H
#define ABSTRACTTOOLBOX_H

#include <QDialog>

class AbstractToolBox : public QDialog
{
    Q_OBJECT
public:
    explicit AbstractToolBox(QWidget *parent = nullptr) : QDialog(parent) {}

    virtual void retranslate() {}
    virtual void receiveData(const QByteArray &array) { (void)array; }
    virtual void setFilePath(const QString &path) { (void)path; }

signals:
    void transmitData(const QByteArray &);
};

class AbstractToolBoxFactory : public QObject
{
    Q_OBJECT
public:
    AbstractToolBoxFactory() {}
    virtual AbstractToolBox* create(QWidget *parent = nullptr) = 0;
    virtual QString title() { return "Tool Box"; }

    AbstractToolBox *product() { return m_product; }

    void setProduct(AbstractToolBox *product) {
        m_product = product;
        connect(product, SIGNAL(destroyed()), this, SLOT(toolBoxDestroyed()));
    }

private slots:
    void toolBoxDestroyed() { m_product = nullptr; }

private:
    AbstractToolBox *m_product = nullptr;
};

#endif // ABSTRACTTOOLBOX_H
