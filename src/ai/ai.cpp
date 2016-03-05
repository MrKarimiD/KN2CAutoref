#include "ai.h"

AI::AI(WorldModel *worldmodel, QString field_size, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    Field::setup_consts(field_size);

    udp = new QUdpSocket();
    ip.setAddress("192.168.4.113");

    current_play = 0;
    for(int i=0; i<PLAYERS_MAX_NUM; i++)
        current_tactic[i] = 0;

    firstWait = 0;
    recordPermission = false;
    logplayer = new AI_logPlayer();

    playAuto = new PlayAutoReplacement(wm);
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

Play* AI::getCurrentPlay()
{
    return current_play;
}

Tactic* AI::getCurrentTactic(int i)
{
    return current_tactic[i];
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
        //Ready for Auto Replacer
//        current_play = playAuto;
//        playAuto->execute();

//        Tactic *tactic = playAuto->getTactic();
//        RobotCommand rc = tactic->getCommand();
//        wm->autoReplacer.SendCommand(rc);

        // if( recordPermission )
        //logplayer->recordLog(wm, RCs);
        fps.Pulse();
    }
    else
        firstWait++;
}