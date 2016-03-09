#ifndef REFSTATUS_H
#define REFSTATUS_H

#include <QObject>
#include "base.h"

struct rule_status
{
    QString name;
    bool status;
};

struct trespasser
{
    TeamColorType color;
    int id;
    QString error;
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

    void set_FreekickValidDist(TeamColorType color, bool input);
    bool get_FreekickValidDist(TeamColorType color);

    void set_StopSpeed(TeamColorType color, bool input);
    bool get_StopSpeed(TeamColorType color);

signals:

public slots:

private:
    playerNum blueNum;
    playerNum yellowNum;

    bool blueInValidDist;
    bool yellowInValidDist;

    bool blue_FreekickValidDist;
    bool yellow_FreekickValidDist;

    bool blue_StopSpeed;
    bool yellow_StopSpeed;

    QList<rule_status> statusOfRules;

};

#endif // REFSTATUS_H
