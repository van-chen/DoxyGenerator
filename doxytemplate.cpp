#include "doxytemplate.h"
#include "ui_doxytemplate.h"
#include <QFile>
#include <qdebug.h>
#include <QSettings>

DoxyTemplate::DoxyTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoxyTemplate)
{
    ui->setupUi(this);

    QSettings s("GIS", "doxyGenerator");
    auto saved = s.value("template");
    if (saved.toString().isEmpty()) {
        QFile f(":/new/prefix1/template.txt");
        f.open(QIODevice::ReadOnly | QIODevice::Text);
        ui->textEdit->setText(f.readAll());
    }
    else
        ui->textEdit->setText(saved.toString());
}

QString DoxyTemplate::Template() {
    return ui->textEdit->toPlainText();
}

DoxyTemplate::~DoxyTemplate()
{
    QSettings s("GIS", "doxyGenerator");
    s.setValue("template", ui->textEdit->toPlainText());
    delete ui;
}

void DoxyTemplate::on_pushButton_clicked()
{
    QFile f(":/new/prefix1/template.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    ui->textEdit->setText(f.readAll());
}
