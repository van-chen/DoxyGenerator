#ifndef FILEDESCRIPTION_H
#define FILEDESCRIPTION_H

#include <QDialog>

namespace Ui {
class FileDescription;
}

class FileDescription : public QDialog
{
    Q_OBJECT

public:
    explicit FileDescription(QWidget *parent = nullptr);
    ~FileDescription();
    void SetText(const QString & text);
    void AppendText(const QString & text);
public:
    Ui::FileDescription *ui;
};

#endif // FILEDESCRIPTION_H
