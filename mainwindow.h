#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "logindialog.h"
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

    void on_createDirectoryPushButton_clicked();

    void on_directoriesListView_doubleClicked(const QModelIndex &index);

    void on_backPushButton_clicked();

    void on_createFilePushButton_clicked();

    void on_directoriesListView_clicked(const QModelIndex &index);

    void on_filesListView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    StorageManager& storageManager;

    void renderDirectories(const std::string& location);
    void renderFiles(const std::string& location);
    void renderLocation(const std::string& location);

    void changeLocation(const std::string& location);
};
#endif // MAINWINDOW_H
