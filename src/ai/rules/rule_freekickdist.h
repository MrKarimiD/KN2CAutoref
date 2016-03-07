#ifndef RULE_FREEKICKDIST_H
#define RULE_FREEKICKDIST_H

#include <QObject>
#include "rule.h"

class Rule_freeKickDist : public Rule
{
    Q_OBJECT
public:
    explicit Rule_freeKickDist(WorldModel* wm, refStatus* rS, QObject *parent = 0);
    virtual bool isValid();
    virtual void execute();

signals:

public slots:
};

#endif // RULE_FREEKICKDIST_H
