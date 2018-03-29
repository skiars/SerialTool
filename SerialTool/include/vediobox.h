#ifndef __VEDIOBOX_H
#define __VEDIOBOX_H

#include <QDialog>

namespace Ui {
    class VedioBox;
}

class VedioBox : public QDialog
{
    Q_OBJECT

public:
    VedioBox(QWidget *parent = NULL);
    ~VedioBox();
    void append(const QByteArray &arr);
    void setFilePath(const QString &path);

private slots:
    void saveImage();
    void copyImage();

private:
    Ui::VedioBox *ui;
    QByteArray array;
    QPixmap image;
    char imageData[600];
    QString filePath;
};

#endif
