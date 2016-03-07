#include "ai.h"

AI::AI(WorldModel *worldmodel, QString field_size, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    Field::setup_consts(field_size);

    firstWait = 0;
    recordPermission = false;
    logplayer = new AI_logPlayer();

    rS = new refStatus();

    rules.append(new Rule_NumOfPlayers(wm,rS));
    rules.append(new Rule_StopDistance(wm,rS));
    rules.append(new Rule_freeKickDist(wm,rS));
}

void AI::Start()
{
    qDebug() << "AI::Start";
    timer.start(AI_TIMER);
}

void AI::Stop()
{
    qDebug() << "AI::Stop";
    timer.stop();
}

void AI::startRecording()
{
    recordPermission = true;
    // logplayer->restartTime();
}

SSL_log AI::stopRecording()
{
    recordPermission = false;
    return logplayer->saveLog();
}

void AI::startPlaying()
{
    //this->Stop();
    logplayer->setPlayPermission(true);
    logplayer->setPauseStatus(false);
    logplayer->playLog();
}

void AI::stopPlaying()
{
    logplayer->setPlayPermission(false);
    logplayer->setPauseStatus(false);
    this->Start();
}

void AI::pausePlaying()
{
    logplayer->setPlayPermission(false);
    logplayer->setPauseStatus(true);
    logplayer->pauseLog();
}

void AI::loadPlaying(SSL_log logs)
{
    logplayer->loadLog(logs);
}

void AI::setLogFrame(int msec)
{
    logplayer->setFrameNumber(msec);
}

void AI::timer_timeout()
{
    if( firstWait > 5 )
    {
        rS->clearStatusOfRules();
        for(int i = 0;i<rules.size();i++)
        {
            Rule* rule = rules.at(i);
            if( rule->isValid() )
            {
                rS->appendRuleStatus(rule->getRuleName(),true);
                rule->execute();
            }
            else
                rS->appendRuleStatus(rule->getRuleName(),false);
        }

        fps.Pulse();
    }
    else
        firstWait++;
}
