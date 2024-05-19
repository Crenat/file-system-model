#include "logindialog.h"
#include "ui_logindialog.h"
#include "authenticationmanager.h"
#include "storagemanager.h"

#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , authenticationManager(StorageManager::getInstance())
{
    ui->setupUi(this);
    ui->usernameLineEdit->setFocus();

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
    bool isAuthenticationSuccessful = authenticationManager.authenticate(username.toStdString(), password.toStdString());

    User user = StorageManager::getInstance().getUserByUsername(username.toStdString());

    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();

    emit loggedIn(isAuthenticationSuccessful, user.uid);
}

