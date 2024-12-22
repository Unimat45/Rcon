#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "quickconnectaction.hpp"

#include <rcon.h>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConnectionInfo, address, port, password)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cd(nullptr), isOpen(false)
{
    ui->setupUi(this);

    cd = new ConnectDialog(this);

    loadAddresses();

    if (this->infos.size() == 0) {
        ui->menuQuick_Connect->deleteLater();
    }

    for (size_t i = 0; i < this->infos.size(); i++) {
        ConnectionInfo info = this->infos[i];

        QuickConnectAction *act = new QuickConnectAction(i, ui->menuQuick_Connect);
        act->setText(QString("%1:%2").arg(info.address.c_str()).arg(info.port));
        connect(act, &QuickConnectAction::clicked, this, &MainWindow::quickConnect);

        ui->menuQuick_Connect->addAction(act);
    }

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConnect, &QAction::triggered, cd, &ConnectDialog::exec);
    connect(cd, &ConnectDialog::openConnection, this, &MainWindow::openConnection);
    connect(ui->leCmd, &QLineEdit::returnPressed, this, &MainWindow::onSendClicked);
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
}

MainWindow::~MainWindow() {
    delete ui;
    delete cd;
    if (this->isOpen) {
        free_client();
    }

    json j = this->infos;
    std::ofstream f("addresses.json");
    f << j;
    f.close();
}

void MainWindow::loadAddresses() {
    try {
        std::ifstream f("addresses.json");

        json root;
        f >> root;

        f.close();

        this->infos = root.template get<std::vector<ConnectionInfo>>();
    }
    catch (...) {
        // If the file is not found
    }
}

void MainWindow::quickConnect(size_t i) {
    ConnectionInfo info = this->infos[i];

    if (this->isOpen) {
        free_client();
    }

    if (!connect_to_server(info.address.c_str(), (uint16_t)info.port)) {
        ui->teRes->setPlainText("Cannot connect to server");
        this->isOpen = false;
        return;
    }

    if (!rcon_authenticate(info.password.c_str())) {
        ui->teRes->setPlainText("Invalid credentials");
        this->isOpen = false;
        return;
    }

    ui->teRes->setPlainText("Connected!");
    this->isOpen = true;
}

void MainWindow::openConnection(const ConnectionInfo *info) {
    if (this->isOpen) {
        free_client();
    }

    if (!connect_to_server(info->address.c_str(), (uint16_t)info->port)) {
        ui->teRes->setPlainText("Cannot connect to server");
        this->isOpen = false;
        delete info;
        return;
    }

    if (!rcon_authenticate(info->password.c_str())) {
        ui->teRes->setPlainText("Invalid credentials");
        this->isOpen = false;
        delete info;
        return;
    }

    this->infos.push_back(*info);

    ui->teRes->setPlainText("Connected!");
    this->isOpen = true;
    delete info;
}

void MainWindow::onSendClicked() {
    QString cmd = ui->leCmd->text();

    if (cmd.isEmpty() || !this->isOpen) {
        return;
    }

    Message *m = send_command((char*)cmd.toStdString().c_str(), Strip);

    if (m == nullptr) {
        ui->teRes->appendPlainText("Error sending command");
        return;
    }

    if (cmd.toLower() == "stop") {
        this->isOpen = false;
        free_client();
        ui->teRes->setPlainText("Disconnected");
    }
    else {
        ui->teRes->appendPlainText(m->payload);
    }

    ui->leCmd->setText("");

    free_message(m);
}
