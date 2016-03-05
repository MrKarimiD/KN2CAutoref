#include "tacticautoreplacement.h"

TacticAutoReplacement::TacticAutoReplacement(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticAutoReplacment", worldmodel, parent)
{
}

RobotCommand TacticAutoReplacement::getCommand()
{
    RobotCommand rc;
    if(!wm->autoReplacer.isValid) return rc;


    return rc;

}

