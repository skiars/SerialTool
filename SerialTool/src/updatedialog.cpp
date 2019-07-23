#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "version.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QProcess>
#include <QFile>

#define LASTRELEASE_URL "https://api.github.com/repos/skiars/SerialTool/releases/latest"

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    // 不显示问号.
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setFixedSize(400, 400); // 不能伸缩的对话框

    QPixmap pix(":/SerialTool/images/logo.ico");
    pix = pix.scaledToWidth(64, Qt::SmoothTransformation);
    ui->label_2->setPixmap(pix);

    m_manager = new QNetworkAccessManager();
    m_manager->get(QNetworkRequest(QUrl(LASTRELEASE_URL)));
    ui->updateBtn->setEnabled(false);
    ui->label->setText(tr("<b>SerialTool</b><br>Current Version: V") + SOFTWARE_VERSION +
                       tr("<br>Check update...<br><br>"));

    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkUpdateFinished(QNetworkReply*)));
    connect(ui->cancelBtn, SIGNAL(released()), this, SLOT(close()));
}

UpdateDialog::~UpdateDialog()
{
    cancelDownload();
    m_manager->deleteLater();
    delete ui;
}

bool UpdateDialog::compareVersion(QString verNetwork, QString verLocal)
{
    QStringList list1 = verLocal.replace(QRegExp("[a-zA-Z]"), "").split(".");
    QStringList list2 = verNetwork.replace(QRegExp("[a-zA-Z]"), "").split(".");

    if (list1.size() == 3 && list2.size() == 3) {
        qint32 ver1 = (list1.at(0).toInt() << 16) | (list1.at(1).toInt() << 8) | list1.at(2).toInt();
        qint32 ver2 = (list2.at(0).toInt() << 16) | (list2.at(1).toInt() << 8) | list2.at(2).toInt();
        return ver2 > ver1;
    }
    return false;
}

void UpdateDialog::checkUpdateFinished(QNetworkReply *reply)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(reply->readAll(), &jsonError);

    m_manager->disconnect();
    reply->deleteLater();
    if (!doucment.isNull() && jsonError.error == QJsonParseError::NoError) {
        QString updatedDate;
        QJsonObject object = doucment.object();
        QString newVer = object.value("name").toString();
        QJsonArray array = object.value("assets").toArray();

        for(int i = 0; i < array.size(); ++i) {
            QJsonObject object = array.at(i).toObject();

            if (!object.isEmpty() && object.value("name").toString().indexOf(".exe", 0, Qt::CaseInsensitive)) {
                m_downloadUrl = object.value("browser_download_url").toString();
                updatedDate = object.value("updated_at").toString().split('T').at(0);
            }
        }
        ui->textBrowser->setText("-------- " + newVer + "   [ " + updatedDate + " ]" + " --------\n"
                                 + object.value("body").toString());
        if (compareVersion(newVer, SOFTWARE_VERSION)) {
            if (!m_downloadUrl.isEmpty()) {
                m_revInfo = tr("<b>SerialTool</b><br>Current Version: V") + SOFTWARE_VERSION +
                            tr("<br>Find new version: ") + newVer +
                            tr("<br>Updated Date: ") + updatedDate;
                ui->label->setText(m_revInfo + tr("<br>Press \"Update\" button to upgrade."));
                ui->updateBtn->setEnabled(true);
                connect(ui->updateBtn, SIGNAL(released()), this, SLOT(startUpdate()));
            }
        } else {
            m_revInfo = tr("<b>SerialTool</b><br>Current Version: V") + SOFTWARE_VERSION +
                        tr("<br>This is the latest version.<br><br>");
            ui->label->setText(m_revInfo);
        }
    } else {
        m_revInfo = tr("<b>SerialTool</b><br>Current Version: V") + SOFTWARE_VERSION +
                    tr("<br>Network error.<br><br>");
        ui->label->setText(m_revInfo);
    }
}

void UpdateDialog::startUpdate()
{
    QEventLoop eventLoop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(m_downloadUrl)));

    ui->label->setText(m_revInfo + tr("<br>Downloading..."));
    ui->updateBtn->setEnabled(false);
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    QUrl url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    m_downloadReply = m_manager->get(QNetworkRequest(url));
    if (m_downloadReply) {
        m_updateFile = new QFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/update.exe");
        m_updateFile->open(QIODevice::WriteOnly);
        connect(m_downloadReply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
        connect(m_downloadReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
        connect(m_downloadReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
        connect(m_downloadReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError()));
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Download error."), QMessageBox::Ok);
    }
}

void UpdateDialog::downloadReadyRead()
{
    m_updateFile->write(m_downloadReply->readAll());
}

void UpdateDialog::downloadFinished()
{
    m_updateFile->close();
    delete m_updateFile;
    m_updateFile = NULL;
    if (m_downloadReply->error() == QNetworkReply::NoError) {
        if (QProcess::startDetached(QStandardPaths::writableLocation(
                                    QStandardPaths::AppConfigLocation) + "/update.exe")) {
            exit(0);
        }
        QMessageBox::critical(this, tr("Error"), tr("Installation file corruption."), QMessageBox::Ok);
    }
    ui->updateBtn->setEnabled(true);
    m_downloadReply->deleteLater();
    m_downloadReply = NULL;
}

void UpdateDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setValue(bytesReceived * 100 / bytesTotal);
}

void UpdateDialog::downloadError()
{
    cancelDownload();
    QMessageBox::critical(this, tr("Error"), tr("Download error."), QMessageBox::Ok);
}

void UpdateDialog::cancelDownload()
{
    if (m_updateFile) {
        m_updateFile->close();
        delete m_updateFile;
        m_updateFile = NULL;
    }
    if (m_downloadReply) {
        m_downloadReply->disconnect();
        m_downloadReply->abort();
        m_downloadReply->deleteLater();
        m_downloadReply = NULL;
    }
    ui->updateBtn->setEnabled(true);
}
