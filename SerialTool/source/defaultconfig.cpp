#include "defaultconfig.h"
#include <QSettings>
#include <QFile>

void syncDefaultConfig(const QString &iniName)
{
    QFile file(iniName);

    if (!file.exists()) { // 文件不存在
        QFile::copy(":/config/default.ini", iniName);
        file.setPermissions(QFile::WriteOther);
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
