#ifndef REFSTATUS_H
#define REFSTATUS_H

#include <QObject>
#include "base.h"

struct rule_status
{
    QString name;
    bool status;
};

enum playerNum
{
    Zero,
    Enough,
    More
};

class refStatus : public QObject
{
    Q_OBJECT
public:
    explicit refStatus(QObject *parent = 0);
    void set_numOfPlayer(TeamColorType team, playerNum status);
    playerNum get_numOfPlayer(TeamColorType team);
    void appendRuleStatus(QString name, bool status);
    void clearStatusOfRules();
    int ruleSize();
    QString getRuleName(int id);
    bool getRuleStatus(int id);

    void set_InValidDist(TeamColorType color, bool input);
    bool get_InValidDist(TeamColorType color);

signals:

public slots:

private:
    playerNum blueNum;
    playerNum yellowNum;

    bool blueInValidDist;
    bool yellowInValidDist;

    QList<rule_status> statusOfRules;

};

#endif // REFSTATUS_H
