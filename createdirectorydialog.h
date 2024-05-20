#ifndef CREATEDIRECTORYDIALOG_H
#define CREATEDIRECTORYDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirectoryDialog;
}

class CreateDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirectoryDialog(QWidget *parent = nullptr);
    ~CreateDirectoryDialog();

signals:
    void submit(const std::string filename, bool canRead, bool canWrite);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateDirectoryDialog *ui;
};

#endif // CREATEDIRECTORYDIALOG_H
