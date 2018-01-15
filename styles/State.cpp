#include "State.h"
using namespace Ui;

State::State(QObject *parent) : QObject(parent)
{
    hover = false;
    checked = false;
}
State::~State() {}

bool State::isNormal() const {
    return (!hover && !checked);
}
void State::setNormal() {
    hover = false;
    checked = false;
    emit changed();
}
bool State::isHover() const {
    return hover;
}
void State::setHover(bool v) {
    hover = v;
    emit changed();
}
bool State::isChecked() const {
    return checked;
}
void State::setChecked(bool v) {
    checked = v;
    emit changed();
}
