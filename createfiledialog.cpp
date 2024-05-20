#include "createfiledialog.h"
#include "ui_createfiledialog.h"

#include "storagemanager.h"

CreateFileDialog::CreateFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateFileDialog)
{
    ui->setupUi(this);

    StorageManager &storageManager = StorageManager::getInstance();

    ui->nameLineEdit->setFocus();

    ui->sizeSpinBox->setMinimum(1);
    ui->sizeSpinBox->setMaximum(storageManager.getAvailableSpace());

    ui->canReadCheckBox->setChecked(true);
    ui->canWriteCheckBox->setChecked(true);
}

CreateFileDialog::~CreateFileDialog()
{
    delete ui;
}

void CreateFileDialog::on_buttonBox_accepted()
{
    std::string filename = ui->nameLineEdit->text().toStdString();
    int size = ui->sizeSpinBox->value();
    bool canRead = ui->canReadCheckBox->isChecked();
    bool canWrite = ui->canWriteCheckBox->isChecked();

    ui->nameLineEdit->clear();
    ui->sizeSpinBox->clear();

    emit submit(filename, size, canRead, canWrite);
}

