#include "rule_stopdistance.h"

Rule_StopDistance::Rule_StopDistance(WorldModel *wm, refStatus* rS, QObject *parent) :
    Rule("Stop Speed", wm, rS, parent)
{

}

bool Rule_StopDistance::isValid()
{
    if(wm->gs == STATE_Stop)
        return true;
    return false;
}

void Rule_StopDistance::execute()
{
    QList<int> blueIDs = wm->kn->findNearestBlueTo(wm->ball.pos.loc);
    rS->set_InValidDist(COLOR_BLUE, true);
    for(int i = 0; i < blueIDs.size(); i++)
    {
        if( wm->blueRobot[blueIDs.at(i)].pos.loc.dist(wm->ball.pos.loc) < 500 )
        {
            rS->set_InValidDist(COLOR_BLUE, false);
            break;
        }
    }

    QList<int> yellowIDs = wm->kn->findNearestBlueTo(wm->ball.pos.loc);
    rS->set_InValidDist(COLOR_YELLOW, true);
    for(int i = 0; i < yellowIDs.size(); i++)
    {
        if( wm->yellowRobot[yellowIDs.at(i)].pos.loc.dist(wm->ball.pos.loc) < 500 )
        {
            rS->set_InValidDist(COLOR_YELLOW, false);
            break;
        }
    }
}
