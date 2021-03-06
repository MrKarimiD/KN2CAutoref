#ifndef SOCCER_H
#define SOCCER_H

#include <QtCore>
#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QTimer>
#include <QMap>

#include "settings.h"
#include "worldmodel.h"
#include "sslrefbox.h"
#include "sslrefboxnew.h"
#include "sslvision_single.h"
#include "sslvision_double.h"
#include "ai.h"
#include "proto/log.pb.h"
#include "fstream"

class Soccer : public QObject
{
    Q_OBJECT
public:
    explicit Soccer(QObject *parent = 0);
    void recordGameLog();
    void stopGameLog();
    bool saveGameLog();
    void playGameLog();
    void pauseGameLog();
    void loadGameLog();
    void loadGameLog(QString address);
    int logLength();
    void setLogFrame(int msec);

//private:
    QString log;
    GameModeType gamemode;

    WorldModel* wm;
    SSLVision* sslvision;

    SSLRefBox* sslrefbox;
    SSLRefBoxNew* sslrefboxnew;

    SSLRefBox* sslrefbox2;
    SSLRefBoxNew* sslrefboxnew2;

    AI *ai;
    QString field;

    QString Address;
};

#endif // SOCCER_H
