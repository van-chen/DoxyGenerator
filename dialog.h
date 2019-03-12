#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "doxytemplate.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pbSave_clicked();

    void on_pbDoxyTemplate_clicked();

    void on_leProjectName_textChanged(const QString &arg1);

    void on_tbHeaders_textChanged();

    void on_pbGenerateFileDescription_clicked();

    void on_pbSelectProjectDir_clicked();

    void on_pbSelectSolutionDir_clicked();

    void on_pbSelectDocDir_clicked();

    void on_pbShowWarnings_clicked();

private:
    Ui::Dialog *ui;
    QStringList result;
    DoxyTemplate DT;
    QString m_savedFileName;

    QString PreapreTemplate();
};

#endif // DIALOG_H
