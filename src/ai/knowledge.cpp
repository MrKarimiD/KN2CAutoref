#include "knowledge.h"
#include "worldmodel.h"

Knowledge::Knowledge(WorldModel *wm, QObject *parent) :
    QObject(parent),
    _wm(wm)
{
    kickPermission=false;
}

int Knowledge::CountBlueAgents()
{
    int counter=0;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->blueRobot[i].isValid) counter++;
    return counter;
}

int Knowledge::CountYellowAgents()
{
    int counter=0;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->yellowRobot[i].isValid) counter++;
    return counter;
}

QList<int> Knowledge::ActiveBlueAgents()
{
    QList<int> ans;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->blueRobot[i].isValid)
            ans.append(i);
    return ans;
}

QList<int> Knowledge::ActiveYellowAgents()
{
    QList<int> ans;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->yellowRobot[i].isValid)
            ans.append(i);
    return ans;
}

QList<int> Knowledge::findNearestBlueTo(Vector2D loc)
{
    QMap<double, int> smap;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->blueRobot[i].isValid==false) continue;
        double dist=(_wm->blueRobot[i].pos.loc-loc).length();
        smap.insert(dist, i);
    }
    QList<int> ans;
    for(auto i=smap.begin(); i!=smap.end(); i++)
        ans.append(i.value());
    return ans;
}

QList<int> Knowledge::findNearestBlueTo(QList<int> ours, Vector2D loc)
{
    QMap<double, int> smap;
    for(int i=0; i< ours.size(); i++)
    {
        if(_wm->blueRobot[ours.at(i)].isValid==false) continue;
        double dist=(_wm->blueRobot[ours.at(i)].pos.loc-loc).length();
        smap.insert(dist, ours.at(i));
    }
    QList<int> ans;
    for(auto i=smap.begin(); i!=smap.end(); i++)
        ans.append(i.value());
    return ans;
}

QList<int> Knowledge::findNearestYellowTo(Vector2D loc)
{
    QMap<double, int> smap;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->yellowRobot[i].isValid==false) continue;
        double dist=(_wm->yellowRobot[i].pos.loc-loc).length();
        smap.insert(dist, i);
    }
    QList<int> ans;
    for(auto i=smap.begin(); i!=smap.end(); i++)
        ans.append(i.value());
    return ans;
}

QList<int> Knowledge::findNearestYellowTo(QList<int> opps, Vector2D loc)
{
    QMap<double, int> smap;
    for(int i=0; i< opps.size(); i++)
    {
        if(_wm->yellowRobot[opps.at(i)].isValid==false) continue;
        double dist=(_wm->yellowRobot[opps.at(i)].pos.loc-loc).length();
        smap.insert(dist, opps.at(i));
    }
    QList<int> ans;
    for(auto i=smap.begin(); i!=smap.end(); i++)
        ans.append(i.value());
    return ans;
}

int Knowledge::findYellowAttacker()
{
    int ans=0;
    double mindist=1000000;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->yellowRobot[i].isValid==false) continue;
        double dist=(_wm->yellowRobot[i].pos.loc-_wm->ball.pos.loc).length();
        if(dist<mindist)
        {
            mindist=dist;
            ans=i;
        }
    }
    return ans;
}

int Knowledge::findBlueAttacker()
{
    int ans=0;
    double mindist=1000000;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->blueRobot[i].isValid==false) continue;
        double dist=(_wm->blueRobot[i].pos.loc-_wm->ball.pos.loc).length();
        if(dist<mindist)
        {
            mindist=dist;
            ans=i;
        }
    }
    return ans;
}

void Knowledge::sortPlayers(QString tColor, QList<int> &players, Vector2D point, bool ascending)
{
    if( tColor == "Blue" )
    {
        if( ascending )
        {
            for(int i=0;i<players.size();i++)
            {
                for(int j=i;j<players.size();j++)
                {
                    if( (_wm->blueRobot[players.at(j)].pos.loc - point).length()
                            < (_wm->blueRobot[players.at(i)].pos.loc - point).length() )
                    {
                        int tmp = players.at(i);
                        players.replace(i,players.at(j));
                        players.replace(j,tmp);
                    }
                }
            }
        }
        else
        {
            for(int i=0;i<players.size();i++)
            {
                for(int j=i;j<players.size();j++)
                {
                    if( (_wm->blueRobot[players.at(j)].pos.loc - point).length()
                            > (_wm->blueRobot[players.at(i)].pos.loc - point).length() )
                    {
                        int tmp = players.at(i);
                        players.replace(i,players.at(j));
                        players.replace(j,tmp);
                    }
                }
            }
        }
    }
    else
    {
        if( ascending )
        {
            for(int i=0;i<players.size();i++)
            {
                for(int j=i;j<players.size();j++)
                {
                    if( (_wm->yellowRobot[players.at(j)].pos.loc - point).length()
                            < (_wm->yellowRobot[players.at(i)].pos.loc - point).length() )
                    {
                        int tmp = players.at(i);
                        players.replace(i,players.at(j));
                        players.replace(j,tmp);
                    }
                }
            }
        }
        else
        {
            for(int i=0;i<players.size();i++)
            {
                for(int j=i;j<players.size();j++)
                {
                    if( (_wm->yellowRobot[players.at(j)].pos.loc - point).length()
                            > (_wm->yellowRobot[players.at(i)].pos.loc - point).length() )
                    {
                        int tmp = players.at(i);
                        players.replace(i,players.at(j));
                        players.replace(j,tmp);
                    }
                }
            }
        }
    }
}

bool Knowledge::IsInsideRect(Vector2D pos, Vector2D topLeft, Vector2D bottomRight)
{
    return (pos.x > topLeft.x && pos.x < bottomRight.x &&
            pos.y > bottomRight.y && pos.y < topLeft.y);
}

bool Knowledge::IsInsideCircle(Vector2D pos, Vector2D center, double radios)
{
    if( (pos-center).length() < radios )
        return true;
    else
        return false;
}

void Knowledge::ClampToRect(Vector2D *pos, Vector2D topLeft, Vector2D bottomRight)
{
    if(pos->x > bottomRight.x)
    {
        pos->x = bottomRight.x;
    }
    else if(pos->x < topLeft.x)
    {
        pos->x = topLeft.x;
    }

    if(pos->y > topLeft.y)
    {
        pos->y = topLeft.y;
    }
    else if(pos->y < bottomRight.y)
    {
        pos->y = bottomRight.y;
    }
}

bool Knowledge::IsInsideField(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(Field::MinX, Field::MaxY), Vector2D(Field::MaxX, Field::MinY));
}

bool Knowledge::IsInsideOurField(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(Field::MinX,Field::MaxY), Vector2D(0,Field::MinY));
}

bool Knowledge::IsInsideOppField(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(0,Field::MaxY), Vector2D(Field::MaxX,Field::MinY));
}

bool Knowledge::IsInsideGoalShape(Vector2D pos, double goalLeftX, double goalRadius, double goalCcOffset)
{
    double x = pos.x - goalLeftX;
    Vector2D ccl(goalLeftX, goalCcOffset / 2), ccr(goalLeftX, -goalCcOffset / 2);

    return ( (pos-ccl).length() <= goalRadius || (pos-ccr).length() <= goalRadius ||
             (x >= 0 && x <= goalRadius && fabs(pos.y) <= goalCcOffset / 2));
}

bool Knowledge::IsInsideFarArea(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(0.25*Field::MaxX,Field::MaxY), Vector2D(Field::MaxX,Field::MinY));
}

bool Knowledge::IsInsideNearArea(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(Field::MinX,Field::MaxY), Vector2D(0.8*Field::MinX,0.6*Field::MaxY))
            ||
            IsInsideRect(pos, Vector2D(Field::MinX,0.6*Field::MinY), Vector2D(0.8*Field::MinX,Field::MinY));
}

bool Knowledge::IsNearOurGoal(Vector2D pos)
{
    return IsInsideRect(pos, Vector2D(Field::MinX,Field::MaxY), Vector2D(0.25*Field::MinX,Field::MinY));
}

bool Knowledge::IsInsideGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircle_R+2*BALL_RADIUS,
                             (Field::defenceLineLinear_L-Field::defenceLineLinear_R).length());
}

bool Knowledge::IsInsideSecureArea(Vector2D pos, Vector2D ball)
{
    return IsInsideCircle(pos,ball,ALLOW_NEAR_BALL_RANGE);
}

bool Knowledge::IsInsideOppGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::oppGoalCenter.x, Field::goalCircle_R,350);
}

Vector2D Knowledge::PredictDestination(Vector2D sourcePos, Vector2D targetPos, double sourceSpeed, Vector2D targetSpeed)
{
    double factor = _wm->var[3] / 250.0;
    if(factor < 0)
    {
        factor = 0;
    }

    double Vm = sourceSpeed;
    double k = Vm / targetSpeed.length();
    double gamaT = targetSpeed.dir().radian();
    Vector2D delta;

    delta = targetPos - sourcePos;
    double landa = atan2(delta.y, delta.x);
    double theta = gamaT - landa;

    if (theta > AngleDeg::PI)
    {
        theta -= 2 * AngleDeg::PI;
    }

    if (theta < - AngleDeg::PI)
    {
        theta += 2 * AngleDeg::PI;
    }

    double dlta = 0;
    if(k != 0 && fabs(sin(theta) / k) < 1)
    {
        dlta = asin(sin(theta)/k);
    }
    // No solution.
    else
    {
        //qDebug() << "Prediction: No solution.";
        return targetPos;//Vector2D::INVALIDATED;
    }

    double tf = factor * (delta.length() / 1000) / (Vm*cos(dlta) - targetSpeed.length() * cos(theta));

    // No solution.
    if(tf < 0)
    {
        //qDebug() << "Prediction: No solution.";
        return targetPos;   //Vector2D(0,0); //INVALIDATED;
    }

    double catchDist = targetSpeed.length() * tf * 1000;
    Vector2D catchDiff(catchDist * cos(gamaT), catchDist * sin(gamaT));
    Vector2D Pred_pos=targetPos + catchDiff;

    //_wm->predict_pos.append(Pred_pos);
    return Pred_pos;
}

bool Knowledge::CanKick(Position robotPos, Vector2D ballPos)
{
    if(!_wm->ball.isValid) return false;

    double distThreshold = _wm->var[0], degThreshold = _wm->var[1] / 10;
    //@kamin
    AngleDeg d1((ballPos + _wm->ball.vel.loc * .015 - robotPos.loc ).dir());
    //@kmout
    AngleDeg d2(robotPos.dir * AngleDeg::RAD2DEG);
    if(fabs((d1 - d2).degree()) < degThreshold || (360.0 - fabs((d1 - d2).degree())) < degThreshold)
    {
        if((robotPos.loc-ballPos).length() < distThreshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Knowledge::IsReadyForKick(Position current, Position desired, Vector2D ballPos)
{
    double degThreshold = _wm->var[2] / 10;

    if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
            (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
    {
        return CanKick(current, ballPos);

    }
    else
    {
        return false;
    }
}

bool Knowledge::ReachedToPos(Vector2D current, Vector2D desired, double distThreshold)
{
    if( (current-desired).length() < distThreshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Knowledge::isOccupied(int id, Vector2D input)
{
    for(int i = 0;i<PLAYERS_MAX_NUM;i++)
    {
        if(_wm->blueRobot[i].isValid)
        {
            if( (_wm->blueRobot[i].pos.loc - input).length() < ROBOT_RADIUS+100 && ( id != i) )
            {
                return true;
            }
        }
    }

    for(int i = 0;i<PLAYERS_MAX_NUM;i++)
    {
        if(_wm->yellowRobot[i].isValid)
        {
            if( (_wm->yellowRobot[i].pos.loc - input).length() < ROBOT_RADIUS+100)
            {
                return true;
            }
        }
    }

    return false;
}

bool Knowledge::ReachedToPos(Position current, Position desired, double distThreshold, double degThreshold)
{
    if( (current.loc-desired.loc).length() < distThreshold)
    {
        if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
                (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

Position Knowledge::AdjustKickPoint(Vector2D ballPos, Vector2D target, int kickSpeed)
{
    Position p;
    Vector2D dir = (ballPos - target);//.normalizedVector();
    dir.setLength( ROBOT_RADIUS);

    p.loc = ballPos + dir;
    p.dir = (-dir).dir().radian();


    return p;
}
