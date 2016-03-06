#ifndef RULE_H
#define RULE_H

#include <QObject>
#include "worldmodel.h"
#include "refstatus.h"

class Rule : public QObject
{
    Q_OBJECT
public:
    explicit Rule(QString name, WorldModel* wm, refStatus* rS, QObject *parent = 0);
    virtual bool isValid() = 0;
    virtual void execute() = 0;
    QString getRuleName();

protected:
    WorldModel* wm;
    refStatus* rS;
    QString name;
};

#endif // RULE_H
