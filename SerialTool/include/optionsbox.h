#ifndef __OPTIONSBOX_H
#define __OPTIONSBOX_H

#include <QtWidgets/QDialog>

namespace Ui {
class OptionsBox;
}
class MainWindow;
class QAbstractButton;

class OptionsBox : public QDialog {
    Q_OBJECT

public:
    OptionsBox(MainWindow *parent = Q_NULLPTR);
    ~OptionsBox();

private:
    void setup();
    void loadCommand();
    void saveCommand();

private slots:
    void processOptions(QAbstractButton *button);
    void setTextFontAnsi();
    void setTextFontMulti();
    void setPlotBackgroundColor();
    void setAxisColor();
    void onCmdNewClick();
    void onCmdEditClick();
    void onCmdDeleteClick();
    void onUseOpenGLClick(bool status);
    void onOpacitySilderChanged(int value);
    void onOpacitySpinBoxEdited();
    void setCmdItemColor();
    void setLanguage(int index);

private:
    Ui::OptionsBox *ui;
    MainWindow *m_parent;
    QString fontFamily, fontStyle;
    int fontSize;
    QString rxColor, txColor, bgColor, axColor;
    QString language, theme;
    QVector<QString> languageList;
};

#endif
