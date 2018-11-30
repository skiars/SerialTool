#ifndef VEDIOBOX_H
#define VEDIOBOX_H

#include "../abstracttoolbox.h"

namespace Ui {
    class VedioBox;
}

class VedioBox : public AbstractToolBox
{
    Q_OBJECT

public:
    VedioBox(QWidget *parent = nullptr);
    ~VedioBox();
    void retranslate();
    void receiveData(const QByteArray &array);
    void setFilePath(const QString &path);

private slots:
    void saveImage();
    void copyImage();

private:
    Ui::VedioBox *ui;
    QByteArray m_array;
    QPixmap image;
    char imageData[600];
    QString filePath;
};

class VedioBoxFactory : public AbstractToolBoxFactory
{
    Q_OBJECT
public:
    AbstractToolBox* create(QWidget *parent = nullptr) { return new VedioBox(parent); }
    QString title() { return tr("Video Box"); }
};

#endif // VEDIOBOX_H
