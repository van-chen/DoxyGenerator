#include "output.h"
#include "ui_output.h"
#include <QListIterator>
Output::Output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
}

Output::~Output()
{
    delete ui;
}

void Output::SetLog(const QString & log)
{
    ui->teLog->setText(log);
}

void Output::SetWarnings(const QString & warn)
{
    ui->teWarnings->setText(warn);
}

void Output::on_pbRemoveMacros_clicked()
{
    auto list = ui->teWarnings->toPlainText().split("\n", QString::SkipEmptyParts);
    QMutableListIterator<QString> it(list);

    while (it.hasNext()) {
        it.next();
        if (it.value().contains("(macro definition)", Qt::CaseInsensitive))
            it.remove();
    }

    ui->teWarnings->setText(list.join("\n"));
}

void Output::on_pbRemoveTypedef_clicked()
{
    auto list = ui->teWarnings->toPlainText().split("\n", QString::SkipEmptyParts);
    QMutableListIterator<QString> it(list);

    while (it.hasNext()) {
        it.next();
        if (it.value().contains("(typedef)", Qt::CaseInsensitive))
            it.remove();
    }

    ui->teWarnings->setText(list.join("\n"));
}
