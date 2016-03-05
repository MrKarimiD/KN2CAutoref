#include "playautoreplacement.h"

PlayAutoReplacement::PlayAutoReplacement(WorldModel *worldmodel, QObject *parent) :
    Play("PlayAutoReplacement", worldmodel, parent)
{
    tAuto = new TacticAutoReplacement(wm);
}

int PlayAutoReplacement::enterCondition()
{
    return 100000;
}

Tactic *PlayAutoReplacement::getTactic()
{
    return tAuto;
}

void PlayAutoReplacement::initRole()
{

}

void PlayAutoReplacement::execute()
{
    tactics[11] = tAuto;
}
