#ifndef __VEDIOBOX_H
#define __VEDIOBOX_H

#include "ui_vediobox.h"
#include <QDialog>

class VedioBox : public QDialog
{
    Q_OBJECT

public:
    VedioBox(QWidget *parent = NULL);
    void addData(const QByteArray &arr);
    void setFilePath(const QString &path);

private slots:
    void saveImage();
    void copyImage();

private:
    Ui_VedioBox ui;
    QByteArray array;
    QPixmap image;
    char imageData[600];
    QString filePath;
};

#endif
