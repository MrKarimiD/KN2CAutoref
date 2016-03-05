#include "tactic.h"
#define CHIP 3

Tactic::Tactic(QString name, WorldModel *worldmodel, QObject *parent) :
    QObject(parent),
    name(name)
{
    wm = worldmodel;
    id = -1;
    kickPermission=false;
}

QString Tactic::getName()
{
    if(param.size() > 0)
        return name + " => " + param;
    else
        return name;
}

void Tactic::setID(int id)
{
    this->id = id;
}

int Tactic::getID()
{
    return this->id;
}

void Tactic::setPlayerToKeep(int index)
{
    this->playerToKeep = index;
}