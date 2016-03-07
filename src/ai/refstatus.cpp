#include "refstatus.h"

refStatus::refStatus(QObject *parent) : QObject(parent)
{

}

void refStatus::set_numOfPlayer(TeamColorType team, playerNum status)
{
    if( team == TeamColorType::COLOR_BLUE)
        this->blueNum = status;
    else
        this->yellowNum = status;
}

playerNum refStatus::get_numOfPlayer(TeamColorType team)
{
    if( team == TeamColorType::COLOR_BLUE)
        return this->blueNum;

    return this->yellowNum;
}

void refStatus::appendRuleStatus(QString name, bool status)
{
    rule_status rS;
    rS.name = name;
    rS.status = status;
    statusOfRules.append(rS);
}

void refStatus::clearStatusOfRules()
{
    statusOfRules.clear();
}

int refStatus::ruleSize()
{
    return this->statusOfRules.size();
}

QString refStatus::getRuleName(int id)
{
    return this->statusOfRules.at(id).name;
}

bool refStatus::getRuleStatus(int id)
{
    return this->statusOfRules.at(id).status;
}

void refStatus::set_InValidDist(TeamColorType color, bool input)
{
    if(color == COLOR_BLUE)
        this->blueInValidDist = input;
    else
        this->yellowInValidDist = input;
}

bool refStatus::get_InValidDist(TeamColorType color)
{
    if( color == COLOR_BLUE )
        return this->blueInValidDist;
    return this->yellowInValidDist;
}

void refStatus::set_FreekickValidDist(TeamColorType color, bool input)
{
    if( color == COLOR_BLUE )
        this->blue_FreekickValidDist = input;
    else
        this->yellow_FreekickValidDist = input;
}

bool refStatus::get_FreekickValidDist(TeamColorType color)
{
    if( color == COLOR_BLUE )
        return this->blue_FreekickValidDist;
    return this->yellow_FreekickValidDist;
}

