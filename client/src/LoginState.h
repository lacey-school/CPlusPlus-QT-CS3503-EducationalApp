#ifndef LOGINSTATE_H
#define LOGINSTATE_H

#include <QWidget>

#include "State.h"
#include "StateStack.h"
#include <SFML/Network.hpp>
#include "ServerConnection.h"

namespace Ui {
class LoginState;
}

class LoginState : public State
{
    Q_OBJECT

public:
    explicit LoginState(StateStack &stack, Context &context, QWidget *parent = 0);
    ~LoginState();

    virtual bool        update(const sf::Time &deltaTime);

private:
    Ui::LoginState *ui;
    sf::Socket::Status status;

private slots:
    void createSlot();
    void loginSlot();
    void cancelSlot();

};

#endif // LOGINSTATE_H
