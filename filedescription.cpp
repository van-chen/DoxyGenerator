#include "filedescription.h"
#include "ui_filedescription.h"

FileDescription::FileDescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDescription)
{
    ui->setupUi(this);
}

FileDescription::~FileDescription()
{
    delete ui;
}


void FileDescription::SetText(const QString &text)
{
    ui->textEdit->setText(text);
}

void FileDescription::AppendText(const QString &text)
{
    ui->textEdit->append(text);
}
