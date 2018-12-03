#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QMap>

class Translate
{
public:
    Translate(const QString &fileName = QString());
    void setTranslateFile(const QString &fileName);
    QString translate(const QString &str) const;
    inline QString tr(const QString &str) const { return translate(str); }

private:
    QMap<QString, QString> m_map;
};

#endif // TRANSLATE_H
