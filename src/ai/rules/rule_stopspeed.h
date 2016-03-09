#ifndef RULE_STOPSPEED_H
#define RULE_STOPSPEED_H

#include <QObject>
#include "rule.h"

#define VEL_telorance 0.2
#define VEL_counter 100

class Rule_StopSpeed : public Rule
{
    Q_OBJECT
public:
    explicit Rule_StopSpeed(WorldModel* wm, refStatus* rS, QObject *parent = 0);
    virtual bool isValid();
    virtual void execute();

private:
    int blue_unvalidity[PLAYERS_MAX_NUM];
    int yellow_unvalidity[PLAYERS_MAX_NUM];

signals:

public slots:
};

#endif // RULE_STOPSPEED_H
