#ifndef QVTLAYOUT_H
#define QVTLAYOUT_H


#include "qvtline.h"

class QVTLayout
{
public:
    QVTLayout();

    int lineCount() const;
    QVTLine &lineAt(int i);

    void appendLine(int count = 1);

protected:
    QList< QVTLine > _lines;
};

#endif // QVTLAYOUT_H
