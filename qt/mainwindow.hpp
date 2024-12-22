#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <vector>
#include <QMainWindow>
#include "connectdialog.hpp"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    ConnectDialog *cd;
    std::vector<ConnectionInfo> infos;
    bool isOpen;

  private:
    void loadAddresses();

  private slots:
    void quickConnect(size_t i);
    void openConnection(const ConnectionInfo *info);
    void onSendClicked();
};

#endif // MAINWINDOW_HPP
