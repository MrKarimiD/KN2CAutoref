#include "worldmodel.h"

double WorldModel::var[10];

WorldModel::WorldModel()
{
    time = 0;
    ref_goalie_our = 0;
    ref_goalie_opp = 0;
    gs = STATE_Null;
    gs_last = STATE_Null;
    isSim = false;
    kn = new Knowledge(this);

    for(int i=0; i<10; i++)
        var[i] = 0;

    climbing_status = "stopped";
    policy_index = 0;
    iteration_index = 0;
    climbing_start = false;
    save_policies = false;
    load_policies = false;
}
