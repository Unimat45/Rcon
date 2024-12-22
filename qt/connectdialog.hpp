#ifndef CONNECTDIALOG_HPP
#define CONNECTDIALOG_HPP

#include <cstdint>
#include <QDialog>

struct ConnectionInfo {
    std::string address;
    uint16_t port;
    std::string password;
};

namespace Ui
{
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

    int exec() override;

  signals:
    void openConnection(const ConnectionInfo *info);

  private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_HPP
