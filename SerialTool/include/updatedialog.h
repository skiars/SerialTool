#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateDialog;
}

class QNetworkAccessManager;
class QNetworkReply;
class QFile;

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = 0);
    ~UpdateDialog();

private:
    bool compareVersion(QString verLocal, QString verNetwork);

private slots:
    void checkUpdateFinished(QNetworkReply *reply);
    void downloadReadyRead();
    void downloadFinished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadError();
    void startUpdate();
    void cancelDownload();

private:
    Ui::UpdateDialog *ui;
    QNetworkAccessManager *m_manager = NULL;
    QNetworkReply *m_downloadReply = NULL;
    QString m_downloadUrl, m_revInfo;
    QFile *m_updateFile = NULL;
};

#endif // UPDATEDIALOG_H
