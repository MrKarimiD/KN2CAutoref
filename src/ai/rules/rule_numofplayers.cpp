#include "rule_numofplayers.h"

Rule_NumOfPlayers::Rule_NumOfPlayers(WorldModel *wm, refStatus* rS, QObject *parent) :
    Rule("Number Of Players", wm, rS, parent)
{

}

bool Rule_NumOfPlayers::isValid()
{
    if( wm->cmgs.timeout() )
        return false;
    return true;
}

void Rule_NumOfPlayers::execute()
{
    if( wm->kn->CountBlueAgents() > 6)
        rS->set_numOfPlayer(TeamColorType::COLOR_BLUE,playerNum::More);
    else if( wm->kn->CountBlueAgents() == 0 )
        rS->set_numOfPlayer(TeamColorType::COLOR_BLUE,playerNum::Zero);
    else
        rS->set_numOfPlayer(TeamColorType::COLOR_BLUE,playerNum::Enough);

    if( wm->kn->CountYellowAgents() > 6)
        rS->set_numOfPlayer(TeamColorType::COLOR_YELLOW,playerNum::More);
    else if( wm->kn->CountBlueAgents() == 0 )
        rS->set_numOfPlayer(TeamColorType::COLOR_YELLOW,playerNum::Zero);
    else
        rS->set_numOfPlayer(TeamColorType::COLOR_YELLOW,playerNum::Enough);
}

