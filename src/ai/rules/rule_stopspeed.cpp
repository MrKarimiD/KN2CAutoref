#include "rule_stopspeed.h"

Rule_StopSpeed::Rule_StopSpeed(WorldModel* wm, refStatus* rS, QObject *parent) :
    Rule("Stop Speed", wm, rS, parent)
{
    for(int i = 0; i < PLAYERS_MAX_NUM; i++)
    {
        blue_unvalidity[i] = 0;
        yellow_unvalidity[i] = 0;
    }
}

bool Rule_StopSpeed::isValid()
{
    if(wm->gs == STATE_Stop)
        return true;
    return false;
}

void Rule_StopSpeed::execute()
{
    for(int i = 0; i < PLAYERS_MAX_NUM; i++)
    {
        if( wm->blueRobot[i].isValid)
            if( abs(wm->blueRobot[i].vel.loc.length()) > 1.5 + VEL_telorance )
                blue_unvalidity[i]++;

        if(wm->yellowRobot[i].isValid)
            if( abs(wm->yellowRobot[i].vel.loc.length()) > 1.5 + VEL_telorance )
                yellow_unvalidity[i]++;
    }

    rS->set_StopSpeed(COLOR_BLUE, true);
    rS->set_StopSpeed(COLOR_YELLOW, true);

    for(int i = 0; i < PLAYERS_MAX_NUM; i++)
    {
        if( blue_unvalidity[i] > VEL_counter )
            rS->set_StopSpeed(COLOR_BLUE, false);

        if( yellow_unvalidity[i] > VEL_counter )
            rS->set_StopSpeed(COLOR_YELLOW, false);
    }
}

