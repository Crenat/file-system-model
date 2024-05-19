#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "authenticationmanager.h"

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
    void loggedIn(bool success, int uid);

private slots:
    void attemptLogin();

private:
    Ui::LoginDialog *ui;
    AuthenticationManager authenticationManager;
};

#endif // LOGINDIALOG_H
