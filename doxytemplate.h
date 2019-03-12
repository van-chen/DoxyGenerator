#ifndef DOXYTEMPLATE_H
#define DOXYTEMPLATE_H

#include <QDialog>
#include <QWidget>
namespace Ui {
class DoxyTemplate;
}

class DoxyTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit DoxyTemplate(QWidget *parent = nullptr);
    ~DoxyTemplate();
    QString Template();
private slots:
    void on_pushButton_clicked();

private:
    Ui::DoxyTemplate *ui;
};

#endif // DOXYTEMPLATE_H
