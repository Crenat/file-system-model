#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include <QDialog>

namespace Ui {
class CreateUserDialog;
}

class CreateUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateUserDialog(QWidget *parent = nullptr);
    ~CreateUserDialog();

signals:
    void submit(const std::string username, const std::string firstname, const std::string lastname, const std::string password);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateUserDialog *ui;
};

#endif // CREATEUSERDIALOG_H
