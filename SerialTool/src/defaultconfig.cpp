#include "defaultconfig.h"
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>

void syncDefaultConfig(const QString &iniName)
{
    QFile file(iniName);

    if (!file.exists()) { // 文件不存在
        QString fullPath = QFileInfo(iniName).absolutePath();
        QDir dir(fullPath);

        if (!dir.exists()) { // 配置文件夹不存在就创建
            dir.mkpath(fullPath); // 创建多级目录
        }

        QFile::copy(":/config/default.ini", iniName);
        file.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
    } else { // 文件存在
        QSettings config(iniName, QSettings::IniFormat);

        // 默认设置,系统设置区
        config.beginGroup("Settings");
        if (config.value("Language").toString().isEmpty()) {
            config.setValue("Language", "en");
        }
        if (config.value("Theme").toString().isEmpty()) {
            config.setValue("Theme", "default");
        }
        config.endGroup();
    }
}
