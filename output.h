#ifndef OUTPUT_H
#define OUTPUT_H

#include <QDialog>

namespace Ui {
class Output;
}

class Output : public QDialog
{
    Q_OBJECT

public:
    explicit Output(QWidget *parent = nullptr);
    ~Output();
    void SetLog(const QString & log);
    void SetWarnings(const QString & warn);

private slots:
    void on_pbRemoveMacros_clicked();

    void on_pbRemoveTypedef_clicked();

private:
    Ui::Output *ui;
};

#endif // OUTPUT_H
