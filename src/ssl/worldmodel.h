#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#define NUMBEROFFREEKICKS 13
#include <QList>
#include <QDebug>
#include "constants.h"
#include "base.h"
#include "ball.h"
#include "robot.h"
#include "geom.h"
#include "game_state.h"
#include "ref_protocol.h"
#include "knowledge.h"

class WorldModel
{
public:
    explicit WorldModel();
    double time;
    Ball ball;
    Robot blueRobot[PLAYERS_MAX_NUM];
    Robot yellowRobot[PLAYERS_MAX_NUM];
    GameStateType gs;
    GameStateType gs_last;
    GameState cmgs; //referee
    int ref_goalie_our;
    int ref_goalie_opp;
    bool isSim;
    Knowledge *kn;

    //----Graphical Debug-----
    QList<Vector2D> navigation_pos;
    QList<Vector2D> navigation_result;

    bool showAstarOut;
    int indexOfAstarRobot;

    QList<Segment2D> voronoi;
    bool showVoronoi;
    QList<Vector2D> selected;

    bool showDebug;
    QList<Vector2D> debug_pos;
    int debug_type;

    bool showDefenderDebug;

    static double var[10];
    // 0: kick activation dist
    // 1: kick activation angle
    // 2: kick precision angle
    // 3: prediction sliding factor

    //Machine Learning Parameters
    QString ml_type;
    //Hill Climbing
    QString climbing_status;
    int policy_index;
    int iteration_index;
    bool climbing_start;
    bool save_policies;
    bool load_policies;
    QString addressOfPolicyFile;
};

#endif // WORLDMODEL_H
