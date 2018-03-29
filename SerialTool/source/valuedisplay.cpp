#include "valuedisplay.h"
#include "ui_valuedisplay.h"

ValueDisplay::ValueDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ValueDisplay)
{
    // 不显示问号
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);

    ui->setupUi(this);

    setAttribute(Qt::WA_WState_WindowOpacitySet);
    setWindowOpacity(0.7);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 140);
    ui->tableWidget->setColumnWidth(1, 140);
}

ValueDisplay::~ValueDisplay()
{
    delete ui;
}

void ValueDisplay::append(const QByteArray &array)
{
    m_array.append(array);

    int postion = m_array.indexOf('\n');
    if (++postion) { // postion of 'n'
        QString str(QString::fromLocal8Bit(m_array.left(postion)));
        m_array = m_array.mid(postion);
        QStringList list(str.split(QRegExp("\\s+"), QString::SkipEmptyParts));

        if (list.at(0).toLower() == "vdisp" && list.size() > 1) {
            QString id = list.at(1);
            QString value = list.at(2);
            QString additional;

            for (int i = 3; i < list.size(); ++i) {
                additional.append(list.at(i) + ' ');
            }

            QList<QTableWidgetItem*> items =
                    ui->tableWidget->findItems(id, Qt::MatchExactly);
            int editRow = -1;
            for (QTableWidgetItem *item : items) {
                if (item->column() == 0) { // column of "value"
                    editRow = item->row();
                    break;
                }
            }
            if (editRow == -1) {
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(rowCount + 1);
                editRow = rowCount;
            }
            ui->tableWidget->setItem(editRow, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(editRow, 1, new QTableWidgetItem(value));
            ui->tableWidget->setItem(editRow, 2, new QTableWidgetItem(additional));
        }
    }
}
