#include "createuserdialog.h"
#include "ui_createuserdialog.h"

#include <QMessageBox>

CreateUserDialog::CreateUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateUserDialog)
{
    ui->setupUi(this);
}

CreateUserDialog::~CreateUserDialog()
{
    delete ui;
}

void CreateUserDialog::on_buttonBox_accepted()
{
    std::string username = ui->usernameLineEdit->text().toStdString();
    std::string firstname = ui->firstnameLineEdit->text().toStdString();
    std::string lastname = ui->lastnameLineEdit->text().toStdString();
    std::string password = ui->passwordLineEdit->text().toStdString();

    // bool userAlreadyExists = StorageManager::getInstance().getUserByUsername(username).uid != -1;

    if (username.empty() || firstname.empty() || lastname.empty() || password.empty()) {
        QMessageBox::warning(this, "Empty fields", "All fields are required.");
        return;
    }

    // I've tried so hard, and got so far
    // But in the end, it doesn't even matter..
    //
    // if (userAlreadyExists) {
    //     QMessageBox::warning(this, "User already exists", "User with this username already exists.");
    //     return;
    // }

    emit submit(username, firstname, lastname, password);
}

