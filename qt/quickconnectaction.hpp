#ifndef QUICKCONNECTACTION_HPP
#define QUICKCONNECTACTION_HPP

#include <QAction>

class QuickConnectAction : public QAction
{
    Q_OBJECT

  public:
    explicit QuickConnectAction(size_t i, QObject *parent = nullptr) : QAction(parent), i(i) {
        connect(this, &QuickConnectAction::triggered, this, &QuickConnectAction::clicked);
    }

  private:
    size_t i;

  signals:
    void clicked(size_t i);
};

#endif // QUICKCONNECTACTION_HPP
