#include "mainwindow.h"
#include "storagemanager.h"
#include "./ui_mainwindow.h"
#include "user.h"

#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog(this))
    , storageManager(StorageManager::instance())
{
    ui->setupUi(this);

    connect(loginDialog, &LoginDialog::loggedIn, this, &MainWindow::handleLogin);
    currentUserID = -1;
    ui->currentUserLabel->setText("Current user: none");
    ui->actionButton->setText("Login");
    std::vector<User> users = storageManager.getUsers();

    for (int i = 0; i < users.size(); i++)
    {
        User user = users.at(i);
        qDebug() << user.uid << " " << user.username << " " << user.password << " " << user.firstName << " " << user.lastName;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleLogin(bool success, const QString& username, const QString& password) {
    if (success) {
        QMessageBox::information(this, "Login Successful", "Welcome " + username);
        bool success = this->login(username, password);
        if (success) {
            ui->actionButton->setText("Logout");
        }
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
    }
}

bool MainWindow::login(QString username, QString password)
{
    User candidate = storageManager.getUserByUsername(username.toStdString());

    if (candidate.uid != -1 && candidate.password == password.toStdString())
    {
        currentUserID = candidate.uid;
        ui->currentUserLabel->setText("Current user: " + QString::fromStdString(candidate.firstName) + " " + QString::fromStdString(candidate.lastName)); 
        return true;
    }
    return false;
}

void MainWindow::logout()
{
    currentUserID = -1;
    ui->currentUserLabel->setText("Current user: none");
    ui->actionButton->setText("Login");
}

void MainWindow::on_actionButton_clicked()
{
    if (currentUserID != -1)
    {
        this->logout();
        return;
    }

    loginDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    loginDialog->show();
}
