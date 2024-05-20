#include "mainwindow.h"
#include "storagemanager.h"
#include "./ui_mainwindow.h"
#include "createfiledialog.h"
#include "user.h"

#include <QInputDialog>
#include <QDebug>
#include <QtCore>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog(this))
    , createDirectoryDialog(new CreateDirectoryDialog(this))
    , createFileDialog(new CreateFileDialog(this))
    , createUserDialog(new CreateUserDialog(this))
    , storageManager(StorageManager::getInstance())
{
    ui->setupUi(this);

    connect(loginDialog, &LoginDialog::loggedIn, this, &MainWindow::handleLogin);
    connect(createDirectoryDialog, &CreateDirectoryDialog::submit, this, &MainWindow::handleCreateNewDirectory);
    connect(createFileDialog, &CreateFileDialog::submit, this, &MainWindow::handleCreateNewFile);
    connect(createUserDialog, &CreateUserDialog::submit, this, &MainWindow::handleCreateNewUser);

    currentUserID = -1;

    std::string location = "/";

    ui->currentUserLabel->setText("Current user: none");
    ui->actionButton->setText("Login");
    std::vector<User> users = storageManager.getUsers();

    for (int i = 0; i < users.size(); i++)
    {
        User user = users.at(i);
        qDebug() << user.uid << " " << user.username << " " << user.password << " " << user.firstName << " " << user.lastName;
    }

    changeLocation("/");
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
    renderLocation(location);
}

void MainWindow::handleCreateNewUser(const std::string username, const std::string firstname, const std::string lastname, const std::string password)
{
    User newUser;
    newUser.uid = -1;
    strcpy(newUser.username, username.c_str());
    strcpy(newUser.firstName, firstname.c_str());
    strcpy(newUser.lastName, lastname.c_str());
    strcpy(newUser.password, password.c_str());

    if (storageManager.addUser(newUser))
    {
        QMessageBox::information(this, "Create User Successful", "User created successfully");
    }
    else
    {
        QMessageBox::warning(this, "Create User Failed", "User creation failed");
    }
}

void MainWindow::logout()
{
    currentUserID = -1;
    ui->currentUserLabel->setText("Current user: none");
    ui->actionButton->setText("Login");
    renderLocation(location);
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

void MainWindow::renderDirectories(const std::string& location)
{
    std::vector<Inode> directoriesInodes = storageManager.getDirectoriesForLocation(location);

    QStringListModel *directoriesModel = new QStringListModel();
    QStringList directoriesList;

    for (int i = 0; i < directoriesInodes.size(); i++)
    {
        Inode inode = directoriesInodes.at(i);
        bool isOwner = inode.ownerUid == currentUserID;

        if (isOwner || inode.accessRights.read) {
            directoriesList << QString::fromStdString(inode.name);
        }
    }

    directoriesModel->setStringList(directoriesList);
    ui->directoriesListView->setModel(directoriesModel);
}

void MainWindow::renderFiles(const std::string& location)
{
    std::vector<Inode> filesInodes = storageManager.getFilesForLocation(location);

    QStringListModel *filesModel = new QStringListModel();
    QStringList filesList;

    for (int i = 0; i < filesInodes.size(); i++)
    {
        Inode inode = filesInodes.at(i);
        bool isOwner = inode.ownerUid == currentUserID;

        if (isOwner || inode.accessRights.read) {
            filesList << QString::fromStdString(inode.name);
        }
    }

    filesModel->setStringList(filesList);
    ui->filesListView->setModel(filesModel);
}

void MainWindow::renderLocation(const std::string& location)
{
    std::vector<Inode> inodes = storageManager.getInodes();

    qDebug() << " ";
    for (int i = 0; i < inodes.size(); i++)
    {
        Inode inode = inodes.at(i);
        qDebug() << inode.uid << " " << inode.name << " " << inode.location << " " << inode.isDirectory << " " << inode.size << " " << inode.ownerUid << " " << inode.accessRights.read << " " << inode.accessRights.write;
    }

    renderDirectories(location);
    renderFiles(location);
}

void MainWindow::changeLocation(const std::string& location)
{
    this->location = location == "" ? "/" : location;
    this->ui->locationLabel->setText(QString::fromStdString(location));
    renderLocation(location);
}

void MainWindow::handleCreateNewDirectory(const std::string filename, bool canRead, bool canWrite)
{
    if (currentUserID == -1)
    {
        QMessageBox::warning(this, "Create Directory Failed", "You must be logged in to create a directory");
        return;
    }

    if (!filename.size())
    {
        return;
    }

    Inode newDirectory;
    newDirectory.uid = -1;
    newDirectory.isDirectory = true;
    newDirectory.size = 0;
    newDirectory.ownerUid = currentUserID;
    newDirectory.accessRights.read = canRead;
    newDirectory.accessRights.write = canWrite;
    strcpy(newDirectory.name, filename.c_str());
    strcpy(newDirectory.location, location.c_str());


    if (storageManager.addInode(newDirectory))
    {
        QMessageBox::information(this, "Create Directory Successful", "Directory created successfully");
        renderLocation(location);
    }
    else
    {
        QMessageBox::warning(this, "Create Directory Failed", "Directory creation failed");
    }
}

void MainWindow::on_createDirectoryPushButton_clicked()
{
    if (currentUserID == -1)
    {
        QMessageBox::warning(this, "Create Directory Failed", "You must be logged in to create a directory");
        return;
    }

    createDirectoryDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    createDirectoryDialog->show();
}

void MainWindow::on_directoriesListView_doubleClicked(const QModelIndex &index)
{
    QString directoryName = index.data().toString();

    if (this->location == "/")
    {
        changeLocation("/" + directoryName.toStdString());
        return;
    }

    std::string newLocation = this->location + "/" + directoryName.toStdString();

    changeLocation(newLocation);
}


void MainWindow::on_backPushButton_clicked()
{
    if (location == "/")
    {
        return;
    }

    std::string newLocation = location.substr(0, location.find_last_of("/"));
    changeLocation(newLocation.size() > 0 ? newLocation : "/");
}

void MainWindow::handleCreateNewFile(const std::string filename, int size, bool canRead, bool canWrite)
{
    if (!filename.size())
    {
        return;
    }

    Inode newFile;
    newFile.isDirectory = false;
    newFile.size = size;
    newFile.ownerUid = currentUserID;
    newFile.accessRights.read = canRead;
    newFile.accessRights.write = canWrite;
    strcpy(newFile.name, filename.c_str());
    strcpy(newFile.location, location.c_str());

    if (storageManager.addInode(newFile))
    {
        QMessageBox::information(this, "Create File Successful", "File created successfully");
        renderLocation(location);
    }
    else
    {
        QMessageBox::warning(this, "Create File Failed", "File creation failed");
    }
}

void MainWindow::on_createFilePushButton_clicked()
{
    if (currentUserID == -1)
    {
        QMessageBox::warning(this, "Create File Failed", "You must be logged in to create a file");
        return;
    }

    createFileDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    createFileDialog->show();
}


void MainWindow::on_directoriesListView_clicked(const QModelIndex &index)
{
    QString directoryName = index.data().toString();

    std::vector<Inode> directoriesInodes = storageManager.getDirectoriesForLocation(location);

    for (int i = 0; i < directoriesInodes.size(); i++)
    {
        Inode inode = directoriesInodes.at(i);

        if (QString::fromStdString(inode.name) == directoryName)
        {
            QStandardItemModel *model = new QStandardItemModel(5, 2, this);
            model->setHorizontalHeaderItem(0, new QStandardItem("Property"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

            User owner = storageManager.getUserByUid(inode.ownerUid);

            model->setItem(0, 0, new QStandardItem("Name"));
            model->setItem(0, 1, new QStandardItem(QString::fromStdString(inode.name)));

            model->setItem(1, 0, new QStandardItem("Location"));
            model->setItem(1, 1, new QStandardItem(QString::fromStdString(inode.location)));

            model->setItem(2, 0, new QStandardItem("Owner"));
            model->setItem(2, 1, new QStandardItem(QString::fromStdString(owner.firstName) + " " + QString::fromStdString(owner.lastName)));

            model->setItem(3, 0, new QStandardItem("Can read (For non owner)"));
            model->setItem(3, 1, new QStandardItem(inode.accessRights.read ? "Yes" : "No"));

            model->setItem(4, 0, new QStandardItem("Can write (For non owner)"));
            model->setItem(4, 1, new QStandardItem(inode.accessRights.write ? "Yes" : "No"));

            this->ui->infoTableView->setModel(model);
            return;
        }
    }
}


void MainWindow::on_filesListView_clicked(const QModelIndex &index)
{
    QString fileName = index.data().toString();

    std::vector<Inode> filesInodes = storageManager.getFilesForLocation(location);

    for (int i = 0; i < filesInodes.size(); i++)
    {
        Inode inode = filesInodes.at(i);

        if (QString::fromStdString(inode.name) == fileName)
        {
            QStandardItemModel *model = new QStandardItemModel(7, 2, this);
            model->setHorizontalHeaderItem(0, new QStandardItem("Property"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

            User owner = storageManager.getUserByUid(inode.ownerUid);

            model->setItem(0, 0, new QStandardItem("Name"));
            model->setItem(0, 1, new QStandardItem(QString::fromStdString(inode.name)));

            model->setItem(1, 0, new QStandardItem("Location"));
            model->setItem(1, 1, new QStandardItem(QString::fromStdString(inode.location)));

            model->setItem(2, 0, new QStandardItem("Owner"));
            model->setItem(2, 1, new QStandardItem(QString::fromStdString(owner.firstName) + " " + QString::fromStdString(owner.lastName)));

            model->setItem(3, 0, new QStandardItem("Can read (For non owner)"));
            model->setItem(3, 1, new QStandardItem(inode.accessRights.read ? "Yes" : "No"));

            model->setItem(4, 0, new QStandardItem("Can write (For non owner)"));
            model->setItem(4, 1, new QStandardItem(inode.accessRights.write ? "Yes" : "No"));

            model->setItem(5, 0, new QStandardItem("Size"));
            model->setItem(5, 1, new QStandardItem(QString::number(inode.size)));

            model->setItem(6, 0, new QStandardItem("Size on disk"));
            model->setItem(6, 1, new QStandardItem(QString::number(inode.size && StorageManager::BLOCK_SIZE - (inode.size % StorageManager::BLOCK_SIZE) ? (inode.size / StorageManager::BLOCK_SIZE + 1) * StorageManager::BLOCK_SIZE : inode.size)));

            this->ui->infoTableView->setModel(model);
            return;
        }
    }
}


void MainWindow::on_createUserButton_clicked()
{
    createUserDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    createUserDialog->show();
}

