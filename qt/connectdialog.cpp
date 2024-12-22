#include "connectdialog.hpp"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog() { delete ui; }

int ConnectDialog::exec() {
    int ret = QDialog::exec();

    if (ret == Accepted) {
        ConnectionInfo *info = new ConnectionInfo {
            ui->lineEdit->text().toStdString(),
            (uint16_t)ui->spinBox->value(),
            ui->lineEdit_3->text().toStdString()
        };

        emit this->openConnection(info);
    }
    return ret;
}
