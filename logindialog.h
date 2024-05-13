#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void loggedIn(bool success, const QString& username, const QString& password);

private slots:
    void attemptLogin();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
