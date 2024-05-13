#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->volkswagenPassat, &QDialogButtonBox::accepted, this, &LoginDialog::attemptLogin);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::attemptLogin()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    // bool success = userManager.authenticate(username.toStdString(), password.toStdString());

    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();

    emit loggedIn(true, username, password);
}

