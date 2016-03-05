#ifndef PLAYAUTOREPLACEMENT_H
#define PLAYAUTOREPLACEMENT_H

#include <QObject>
#include "play.h"
#include "tactic/tacticautoreplacement.h"

class PlayAutoReplacement : public Play
{
    Q_OBJECT
public:
    explicit PlayAutoReplacement(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    virtual int enterCondition();
    virtual Tactic* getTactic();

private:
    TacticAutoReplacement* tAuto;

    virtual void initRole();
};

#endif // PLAYAUTOREPLACEMENT_H
