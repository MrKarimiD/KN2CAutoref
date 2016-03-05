#ifndef TACTICAUTOREPLACEMENT_H
#define TACTICAUTOREPLACEMENT_H

#include <QObject>
#include "tactic.h"

class TacticAutoReplacement : public Tactic
{
    Q_OBJECT
public:
    explicit TacticAutoReplacement(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
};

#endif // TACTICAUTOREPLACEMENT_H
