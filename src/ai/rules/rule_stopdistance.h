#ifndef RULE_STOPDISTANCE_H
#define RULE_STOPDISTANCE_H

#include <QObject>
#include "rule.h"

class Rule_StopDistance : public Rule
{
    Q_OBJECT
public:
    explicit Rule_StopDistance(WorldModel* wm, refStatus* rS, QObject *parent = 0);
    virtual bool isValid();
    virtual void execute();

signals:

public slots:
};

#endif // RULE_STOPDISTANCE_H
