#ifndef CREATEFILEDIALOG_H
#define CREATEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateFileDialog;
}

class CreateFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateFileDialog(QWidget *parent = nullptr);
    ~CreateFileDialog();

signals:
    void submit(const std::string filename, int size, bool canRead, bool canWrite);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateFileDialog *ui;
};

#endif // CREATEFILEDIALOG_H
