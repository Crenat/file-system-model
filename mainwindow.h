#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logindialog.h"
#include "createdirectorydialog.h"
#include "createfiledialog.h"
#include "createuserdialog.h"
#include "storagemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int currentUserID;
    std::string location;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void logout();
    void login(int uid);

private slots:
    void on_actionButton_clicked();
    void handleLogin(bool success, int uid);
    void handleCreateNewDirectory(const std::string filename, bool canRead, bool canWrite);
    void handleCreateNewFile(const std::string filename, int size, bool canRead, bool canWrite);
    void handleCreateNewUser(const std::string username, const std::string firstname, const std::string lastname, const std::string password);

    void on_createDirectoryPushButton_clicked();

    void on_directoriesListView_doubleClicked(const QModelIndex &index);

    void on_backPushButton_clicked();

    void on_createFilePushButton_clicked();

    void on_directoriesListView_clicked(const QModelIndex &index);

    void on_filesListView_clicked(const QModelIndex &index);

    void on_createUserButton_clicked();

private:
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    CreateDirectoryDialog *createDirectoryDialog;
    CreateFileDialog *createFileDialog;
    CreateUserDialog *createUserDialog;
    StorageManager& storageManager;

    void renderDirectories(const std::string& location);
    void renderFiles(const std::string& location);
    void renderLocation(const std::string& location);

    void changeLocation(const std::string& location);
};
#endif // MAINWINDOW_H
