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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void logout();
    bool login(QString username, QString password);

private slots:
    void on_actionButton_clicked();
    void handleLogin(bool success, const QString& username, const QString& password);

private:
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    StorageManager& storageManager;
};
#endif // MAINWINDOW_H
