#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <QtCore>
#include "geom.h"
#include "position.h"

class WorldModel;

class Knowledge : public QObject
{
    Q_OBJECT
public:
    explicit Knowledge(WorldModel* wm, QObject *parent = 0);
    int CountBlueAgents();
    int CountYellowAgents();
    QList<int> ActiveBlueAgents();
    QList<int> ActiveYellowAgents();
    QList<int> findNearestBlueTo(Vector2D loc);
    QList<int> findNearestBlueTo(QList<int> ours,Vector2D loc);
    QList<int> findNearestYellowTo(Vector2D loc);
    QList<int> findNearestYellowTo(QList<int> opps,Vector2D loc);
    int findBlueAttacker();
    int findYellowAttacker();

    void sortPlayers(QString tColor, QList<int> &players,Vector2D point,bool ascending);

    void ClampToRect(Vector2D *pos, Vector2D topLeft, Vector2D bottomRight);
    bool IsInsideRect(Vector2D pos, Vector2D topLeft, Vector2D bottomRight);
    bool IsInsideCircle(Vector2D pos,Vector2D center,double radios);
    bool IsInsideField(Vector2D pos);
    bool IsInsideOurField(Vector2D pos);
    bool IsInsideOppField(Vector2D pos);
    bool IsInsideFarArea(Vector2D pos);
    bool IsInsideNearArea(Vector2D pos);
    bool IsNearOurGoal(Vector2D pos);

    bool IsInsideGoalShape(Vector2D pos, double goalLeftX, double goalRadius, double goalCcOffset);
    bool IsInsideGolieArea(Vector2D pos);
    bool IsInsideSecureArea(Vector2D pos,Vector2D ball);

    bool IsInsideOppGolieArea(Vector2D pos);

    bool CanKick(Position robotPos, Vector2D ballPos);
    bool IsReadyForKick(Position current, Position desired, Vector2D ballPos);
    Position AdjustKickPoint(Vector2D ballPos, Vector2D target, int kickSpeed = 5);

    Vector2D PredictDestination(Vector2D sourcePos, Vector2D targetPos, double sourceSpeed, Vector2D targetSpeed);
    bool ReachedToPos(Position current, Position desired, double distThreshold, double degThreshold);
    bool ReachedToPos(Vector2D current, Vector2D desired, double distThreshold);

    bool isOccupied(int id, Vector2D input);

private:
    WorldModel* _wm;
    bool kickPermission;//reset:check
    Vector2D last_ball_vell;
};

#endif // KNOWLEDGE_H
