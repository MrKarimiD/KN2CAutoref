#include "sslrefbox.h"

SSLRefBox::SSLRefBox(QString ip, int port, TeamColorType color, float ball_min, WorldModel* wm, QObject *parent) :
    SSLReceiver(ip,port,parent),
    _cmdcounter(0),
    _color(color),
    _lastCMDCounter(0),
    _ball_min(ball_min),
    _wm(wm)
{
    paused = false;
    refgs.cmd = 0;
    refgs.cmd_counter = 0;
    wm->cmgs.init(color);
    connect(this, SIGNAL(newReceivedPacket(QByteArray,QString,int)), this, SLOT(readPendingPacket(QByteArray,QString,int)));
    qDebug() << "SSLRefBox Initialization...";
    qDebug() << "IP: " << ip;
    qDebug() << "Port: " << port;
    qDebug() << "Color: " << (_color==COLOR_BLUE?"Blue":"Yellow");
    qDebug() << "Ball Min: " << _ball_min;
}

void SSLRefBox::readPendingPacket(QByteArray data, QString ip, int port)
{
    // check for server ip (& port)
    if(ip=="" && port==0) return;

    GameStatePacket pck;
    pck.cmd=data[0];
    pck.cmd_counter=data[1];
    pck.goals_blue=data[2];
    pck.goals_yellow=data[3];
    pck.time_remaining=data[4]*256 + data[5];

    // parse it
    parse(pck);
}

void SSLRefBox::parse(GameStatePacket pck)
{
    // save last gs packet
    refgs = pck;
    if(paused) return;
    bool ball_moved = _lastBallpos.loc.dist(_wm->ball.pos.loc) > _ball_min;
    _wm->cmgs.transition(pck.cmd, ball_moved);
    updategs(pck.cmd, ball_moved);

    if(pck.cmd_counter != _lastCMDCounter) // new cmd
    {
        // save ball position
        _lastBallpos = _wm->ball.pos;
        // update command counter
        _lastCMDCounter = pck.cmd_counter;
        // send signal
        emit newRefreeCommand();
    }
    else    // no new cmd
    {
    }
}

void SSLRefBox::logParse(GameStatePacket pck)
{
    // save last gs packet
    refgs = pck;

    bool ball_moved = _lastBallpos.loc.dist(_wm->ball.pos.loc) > _ball_min;
    _wm->cmgs.transition(pck.cmd, ball_moved);
    updategs(pck.cmd, ball_moved);

    if(pck.cmd_counter != _lastCMDCounter) // new cmd
    {
        // save ball position
        _lastBallpos = _wm->ball.pos;
        // update command counter
        _lastCMDCounter = pck.cmd_counter;
        // send signal
        emit newRefreeCommand();
    }
    else    // no new cmd
    {
    }
}

void SSLRefBox::updategs(char cmd, bool)
{
    _wm->gs_last=_wm->gs;

    switch(cmd)
    {
    case COMM_HALT:
        _wm->gs=STATE_Halt;
        break;
    case COMM_STOP:
        _wm->gs=STATE_Stop;
        break;
    case COMM_READY:
        _wm->gs=STATE_Start;
        break;
    case COMM_START:
        _wm->gs=STATE_ForceStart;
        break;
    case COMM_KICKOFF_BLUE:
        _wm->gs = STATE_Kick_off_Blue;
        break;
    case COMM_KICKOFF_YELLOW:
        _wm->gs = STATE_Kick_off_Yellow;
        break;
    case COMM_PENALTY_BLUE:
        _wm->gs = STATE_Penalty_Blue;
        break;
    case COMM_PENALTY_YELLOW:
        _wm->gs = STATE_Penalty_Yellow;
        break;
    case COMM_DIRECT_BLUE:
        _wm->gs = STATE_Free_kick_Blue;
        break;
    case COMM_DIRECT_YELLOW:
        _wm->gs = STATE_Free_kick_Yellow;
        break;
    case COMM_INDIRECT_BLUE:
        _wm->gs = STATE_Indirect_Free_kick_Blue;
        break;
    case COMM_INDIRECT_YELLOW:
        _wm->gs = STATE_Indirect_Free_kick_Yellow;
        break;

    case COMM_TIMEOUT_BLUE:
    case COMM_TIMEOUT_YELLOW:
        _wm->gs = STATE_Halt;
        break;

    default:
        _wm->gs=STATE_Null;
        break;
    }
}
