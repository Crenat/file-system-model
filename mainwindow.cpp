#include "mainwindow.h"
#include "storagemanager.h"
#include "./ui_mainwindow.h"
#include "user.h"

#include <QInputDialog>
#include <QDebug>
#include <QtCore>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog(this))
    , storageManager(StorageManager::getInstance())
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

void MainWindow::handleLogin(bool success, int uid) {
    if (!success) {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
        return;
    }

    User user = storageManager.getUserByUid(uid);
    login(user.uid);

    QMessageBox::information(this, "Login Successful", QString("Welcome %1").arg(user.username));
    ui->actionButton->setText("Logout");
}

void MainWindow::login(int uid)
{
    User candidate = storageManager.getUserByUid(uid);

    currentUserID = candidate.uid;
    ui->currentUserLabel->setText("Current user: " + QString::fromStdString(candidate.firstName) + " " + QString::fromStdString(candidate.lastName));
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
