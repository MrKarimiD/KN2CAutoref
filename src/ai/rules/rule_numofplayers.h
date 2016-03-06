#ifndef RULE_NUMOFPLAYERS_H
#define RULE_NUMOFPLAYERS_H

#include <QObject>
#include "rule.h"

class Rule_NumOfPlayers : public Rule
{
    Q_OBJECT
public:
    explicit Rule_NumOfPlayers(WorldModel* wm, refStatus* rS, QObject *parent = 0);
    virtual bool isValid();
    virtual void execute();

signals:

public slots:
};

#endif // RULE_NUMOFPLAYERS_H
