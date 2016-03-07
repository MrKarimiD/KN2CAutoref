#include "rule_freekickdist.h"

Rule_freeKickDist::Rule_freeKickDist(WorldModel* wm, refStatus* rS, QObject *parent) :
    Rule("FreeKick Distance", wm, rS, parent)
{

}

bool Rule_freeKickDist::isValid()
{
    if( wm->cmgs.freeKick() || wm->cmgs.kickoff() )
        return true;
    return false;
}

void Rule_freeKickDist::execute()
{
    QList<int> IDs;
    if( wm->cmgs.blueFreeKick() || wm->cmgs.blueKickoff() )
    {
        rS->set_FreekickValidDist(COLOR_BLUE, true);
        rS->set_FreekickValidDist(COLOR_YELLOW, true);

        IDs = wm->kn->findNearestYellowTo(wm->ball.pos.loc);
        for( int i = 0; i < IDs.size(); i++ )
        {
            if( wm->yellowRobot[IDs.at(i)].pos.loc.dist(wm->ball.pos.loc) )
            {
                rS->set_FreekickValidDist(COLOR_YELLOW, false);
                break;
            }
        }
    }
    else if( wm->cmgs.yellowFreeKick() || wm->cmgs.yellowKickoff() )
    {
        rS->set_FreekickValidDist(COLOR_YELLOW, true);
        rS->set_FreekickValidDist(COLOR_BLUE, true);

        IDs = wm->kn->findNearestBlueTo(wm->ball.pos.loc);
        for( int i = 0; i < IDs.size(); i++ )
        {
            if( wm->blueRobot[IDs.at(i)].pos.loc.dist(wm->ball.pos.loc) )
            {
                rS->set_FreekickValidDist(COLOR_BLUE, false);
                break;
            }
        }
    }
}

