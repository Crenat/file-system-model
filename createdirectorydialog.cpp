#include "createdirectorydialog.h"
#include "ui_createdirectorydialog.h"

CreateDirectoryDialog::CreateDirectoryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDirectoryDialog)
{
    ui->setupUi(this);

    ui->nameLineEdit->setFocus();

    ui->canReadCheckBox->setChecked(true);
    ui->canWriteCheckBox->setChecked(true);
}

CreateDirectoryDialog::~CreateDirectoryDialog()
{
    delete ui;
}

void CreateDirectoryDialog::on_buttonBox_accepted()
{
    std::string filename = ui->nameLineEdit->text().toStdString();
    bool canRead = ui->canReadCheckBox->isChecked();
    bool canWrite = ui->canWriteCheckBox->isChecked();

    ui->nameLineEdit->clear();

    emit submit(filename, canRead, canWrite);
}


