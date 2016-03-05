#ifndef PLAY_H
#define PLAY_H

#define attackernoticeabledistance 200
#define desireddefenderdistance 300
#define desiredleftdefenderdistance 150
#define desiredrightdefenderdistance 150

#include <QObject>
#include "worldmodel.h"
#include "tactic.h"

class Play : public QObject
{
    Q_OBJECT
public:
    explicit Play(QString name, WorldModel *worldmodel, QObject *parent = 0);
    QString getName();
    virtual void execute() = 0;
    virtual Tactic* getTactic() = 0;
    virtual int enterCondition() = 0;

protected:
    WorldModel *wm;
    QString param;
    Tactic *tactics[PLAYERS_MAX_NUM];
    virtual void initRole() = 0;

private:
    QString name;
};

#endif // PLAY_H
