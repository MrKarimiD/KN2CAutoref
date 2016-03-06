#include "rule.h"

Rule::Rule(QString name, WorldModel *wm, refStatus *rS, QObject *parent) :
    QObject(parent),
    wm(wm),
    rS(rS)
{
    this->name = name;
}

QString Rule::getRuleName()
{
    return this->name;
}

