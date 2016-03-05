#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Soccer *soccer, QWidget *parent) :
    QMainWindow(parent),
    timer(this),
    sc(soccer),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->txtLog->append(QSerialPort::);
    _render = new RenderArea(soccer);
    ui->gridRender->addWidget(_render);
    QStringList indexses;
    indexses<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"All";
    ui->astarIndex_comboBox->addItems(indexses);
    QStringList output_types;
    output_types<<"1"<<"2";
    ui->debug_output_type->addItems(output_types);
    this->on_btnLoadVars_clicked();
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    timer.start(100);

    logTimer = new QTimer();
    connect(logTimer, SIGNAL(timeout()), this, SLOT(logTimer_timeout()));

    //this->setWindowState(Qt::WindowFullScreen);

    this->counter = 0;
    this->logPlayer_status = "Stop";
    this->logIsPlayed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::returnRole(AgentRole Role)
{
    QString out;
    switch (Role) {
    case AgentRole::ArcLeft :
        out = "ArcLeft";
        break;
    case AgentRole::ArcRight :
        out = "ArcRight";
        break;
    case AgentRole::ArcMid :
        out = "ArcMid";
        break;
    case AgentRole::AttackerLeft :
        out = "AttackerLeft";
        break;
    case AgentRole::AttackerMid :
        out = "AttackerMid";
        break;
    case AgentRole::AttackerRight :
        out = "AttackerRight";
        break;
    case AgentRole::Blocker :
        out = "Blocker";
        break;
    case AgentRole::DefenderLeft :
        out = "DefenderLeft";
        break;
    case AgentRole::DefenderMid :
        out = "DefenderMid";
        break;
    case AgentRole::DefenderRight :
        out = "DefenderRight";
        break;
    case AgentRole::FixedPositionLeft :
        out = "FixedPositionLeft";
        break;
    case AgentRole::FixedPositionMid :
        out = "FixedPositionMid";
        break;
    case AgentRole::FixedPositionRight :
        out = "FixedPositionRight";
        break;
    case AgentRole::Golie :
        out = "Golie";
        break;
    default:
        out = "Not Set";
    }
    return out;
}

QString MainWindow::returnStatus(AgentStatus Status)
{
    QString out;
    switch (Status) {
    case AgentStatus::Idle :
        out = "Idle";
        break;
    case AgentStatus::Passing :
        out = "Passing";
        break;
    case AgentStatus::Kicking :
        out = "Kicking";
        break;
    case AgentStatus::Chiping :
        out = "Chiping";
        break;
    case AgentStatus::FollowingBall :
        out = "FollowingBall";
        break;
    case AgentStatus::BlockingBall :
        out = "BlockingBall";
        break;
    case AgentStatus::RecievingPass :
        out = "RecievingPass";
        break;
    case AgentStatus::BlockingRobot :
        out = "BlockingRobot";
        break;
    default:
        out = "Not Set";
    }
    return out;
}

QString MainWindow::convertTime2String(int time)
{
    int min = (time / 1000) / 60;
    int sec = (time / 1000) % 60;
    return QString::number(min) + ":" + QString::number(sec);
}

void MainWindow::timelineReset()
{
    sc->stopGameLog();
    logTimer->stop();
    this->counter = 0;
    logIsPlayed = false;

    QString timeLabel = "0:0 / " + convertTime2String(logLength);
    ui->timeLabel->setText(timeLabel);
    ui->timeLine_slider->setValue(0);
}

void MainWindow::timer_timeout()
{
    ui->txtLog->setText(sc->log);

    ui->txtVisionSpeed->setText(QString::number(sc->sslvision->FPS()));
    ui->txtVisionSpeed_0->setText(QString::number(sc->sslvision->getFPS(0)));
    ui->txtVisionSpeed_1->setText(QString::number(sc->sslvision->getFPS(1)));
    ui->txtAIfps->setText(QString::number(sc->ai->fps.FPS()));
    if(sc->transmitter)
        ui->txtTrans->setText(QString::number(sc->transmitter->fps.FPS()));
    if(sc->grsim)
        ui->txtgrSim->setText(QString::number(sc->grsim->fps.FPS()));

    bool pause1;
    bool pause2;
    if(ui->rdoRefMain->isChecked())
    {
        pause1 = false;
        pause2 = true;
    }
    if(ui->rdoRefManual->isChecked())
    {
        pause1 = true;
        pause2 = false;
    }

    GameStatePacket refgs;
    GameStatePacket refgs2;
    if(sc->sslrefbox)
    {
        sc->sslrefbox->paused = pause1;
        ui->txtRefreeSpeed->setText("old : " + QString::number(sc->sslrefbox->FPS()));
        refgs = sc->sslrefbox->refgs;
    }
    if(sc->sslrefboxnew)
    {
        sc->sslrefboxnew->paused = pause1;
        ui->txtRefreeSpeed->setText("new : " + QString::number(sc->sslrefboxnew->FPS()));
        refgs = sc->sslrefboxnew->refgs;
    }
    if(sc->sslrefbox2)
    {
        sc->sslrefbox2->paused = pause2;
        ui->txtRefreeSpeed_2->setText("old : " + QString::number(sc->sslrefbox2->FPS()));
        refgs2 = sc->sslrefbox2->refgs;
    }
    if(sc->sslrefboxnew2)
    {
        sc->sslrefboxnew2->paused = pause2;
        ui->txtRefreeSpeed_2->setText("new : " + QString::number(sc->sslrefboxnew2->FPS()));
        refgs2 = sc->sslrefboxnew2->refgs;
    }

    ui->txtTime->setText(QString::number(sc->wm->time));
    ui->txtTimeBall->setText(QString::number(sc->wm->ball.time));

    QString str_refgs = QString("") + refgs.cmd +
            " : " +QString::number(refgs.cmd_counter) +
            " : " + QString::number(refgs.goals_blue) +
            " : " + QString::number(refgs.goals_yellow) +
            " : " + QString::number(refgs.time_remaining);
    ui->txtrefgs->setText(str_refgs);

    QString str_refgs2 = QString("") + refgs2.cmd +
            " : " +QString::number(refgs2.cmd_counter) +
            " : " + QString::number(refgs2.goals_blue) +
            " : " + QString::number(refgs2.goals_yellow) +
            " : " + QString::number(refgs2.time_remaining);
    ui->txtrefgs_2->setText(str_refgs2);

    ui->txtcmgs_1->setText(QString("gameOn : ") + (sc->wm->cmgs.gameOn()?"1":"0"));
    ui->txtcmgs_2->setText(QString("allowedNearBall : ") + (sc->wm->cmgs.allowedNearBall()?"1":"0"));
    ui->txtcmgs_3->setText(QString("canKickBall : ") + (sc->wm->cmgs.canKickBall()?"1":"0"));

    ui->txtcmgs_4->setText(QString("restart : ") + (sc->wm->cmgs.restart()?"1":"0"));
    ui->txtcmgs_5->setText(QString("ourRestart : ") + (sc->wm->cmgs.ourRestart()?"1":"0"));
    ui->txtcmgs_6->setText(QString("theirRestart : ") + (sc->wm->cmgs.theirRestart()?"1":"0"));

    ui->txtcmgs_7->setText(QString("kickoff : ") + (sc->wm->cmgs.kickoff()?"1":"0"));
    ui->txtcmgs_8->setText(QString("ourKickoff : ") + (sc->wm->cmgs.ourKickoff()?"1":"0"));
    ui->txtcmgs_9->setText(QString("theirKickoff : ") + (sc->wm->cmgs.theirKickoff()?"1":"0"));

    ui->txtcmgs_10->setText(QString("penaltyKick : ") + (sc->wm->cmgs.penaltyKick()?"1":"0"));
    ui->txtcmgs_11->setText(QString("ourPenaltyKick : ") + (sc->wm->cmgs.ourPenaltyKick()?"1":"0"));
    ui->txtcmgs_12->setText(QString("theirPenaltyKick : ") + (sc->wm->cmgs.theirPenaltyKick()?"1":"0"));

    ui->txtcmgs_13->setText(QString("directKick : ") + (sc->wm->cmgs.directKick()?"1":"0"));
    ui->txtcmgs_14->setText(QString("ourDirectKick : ") + (sc->wm->cmgs.ourDirectKick()?"1":"0"));
    ui->txtcmgs_15->setText(QString("theirDirectKick : ") + (sc->wm->cmgs.theirDirectKick()?"1":"0"));

    ui->txtcmgs_16->setText(QString("indirectKick : ") + (sc->wm->cmgs.indirectKick()?"1":"0"));
    ui->txtcmgs_17->setText(QString("ourIndirectKick : ") + (sc->wm->cmgs.ourIndirectKick()?"1":"0"));
    ui->txtcmgs_18->setText(QString("theirIndirectKick : ") + (sc->wm->cmgs.theirIndirectKick()?"1":"0"));

    ui->txtcmgs_19->setText(QString("freeKick : ") + (sc->wm->cmgs.freeKick()?"1":"0"));
    ui->txtcmgs_20->setText(QString("ourFreeKick : ") + (sc->wm->cmgs.ourFreeKick()?"1":"0"));
    ui->txtcmgs_21->setText(QString("theiFreeKick : ") + (sc->wm->cmgs.theirFreeKick()?"1":"0"));

    ui->txtcmgs_22->setText(QString("canMove : ") + (sc->wm->cmgs.canMove()?"1":"0"));

    QString ball  = QString::number(sc->wm->ball.pos.loc.x,'f',2) + " , " + QString::number(sc->wm->ball.pos.loc.y,'f',2);
    QString ballv = QString::number(sc->wm->ball.vel.loc.x,'f',2) + " , " + QString::number(sc->wm->ball.vel.loc.y,'f',2);
    ball = "( " + ball + " ) , ( " + ballv + " ) ";

    // WM
    ui->txtWM->setText("");
    ui->txtWM->append("time : " + QString::number(sc->wm->time));
    ui->txtWM->append("ball : " + ball + " [" + QString::number(sc->wm->ball.isValid?1:0) + "] ");

    ui->txtWM->append("");
    for(int i=0; i<PLAYERS_MAX_NUM; ++i)
    {
        QString r = QString::number(sc->wm->blueRobot[i].pos.loc.x,'f',2) + " , " + QString::number(sc->wm->blueRobot[i].pos.loc.y,'f',2) + " : " + QString::number(sc->wm->blueRobot[i].pos.dir,'f',2);
        QString s = QString::number(sc->wm->blueRobot[i].vel.loc.x,'f',2) + " , " + QString::number(sc->wm->blueRobot[i].vel.loc.y,'f',2) + " : " + QString::number(sc->wm->blueRobot[i].vel.dir,'f',2);
        QString id = QString::number(i);
        if(id.length()<2) id = "0" + id;
        ui->txtWM->append("blue[" + id + "]  : ( " + r + " ) , ( " + s + " ) (" + QString::number(sc->wm->blueRobot[i].vel.loc.length(),'f',2) + ") [" + QString::number(sc->wm->blueRobot[i].isValid) + "] ");
    }

    ui->txtWM->append("");
    for(int i=0; i<PLAYERS_MAX_NUM; ++i)
    {
        QString r = QString::number(sc->wm->yellowRobot[i].pos.loc.x,'f',2) + " , " + QString::number(sc->wm->yellowRobot[i].pos.loc.y,'f',2) + " : " + QString::number(sc->wm->yellowRobot[i].pos.dir,'f',2);
        QString s = QString::number(sc->wm->yellowRobot[i].vel.loc.x,'f',2) + " , " + QString::number(sc->wm->yellowRobot[i].vel.loc.y,'f',2) + " : " + QString::number(sc->wm->yellowRobot[i].vel.dir,'f',2);
        QString id = QString::number(i);
        if(id.length()<2) id = "0" + id;
        ui->txtWM->append("yellow[" + id + "] :  ( " + r + " ) , ( " + s + " ) (" + QString::number(sc->wm->yellowRobot[i].vel.loc.length(),'f',2) + ") [" + QString::number(sc->wm->yellowRobot[i].isValid) + "] ");
    }
    ui->txtWM->append("");
    ui->txtWM->append("ref_goalie_our : " + QString::number(sc->wm->ref_goalie_our));
    ui->txtWM->append("ref_goalie_opp : " + QString::number(sc->wm->ref_goalie_opp));
    ui->txtWM->append("");

    //AI
    sc->wm->var[0] = ui->spnvar0->value();
    ui->txtvar0->setText(QString::number(sc->wm->var[0]));
    sc->wm->var[1] = ui->spnvar1->value();
    ui->txtvar1->setText(QString::number(sc->wm->var[1]));
    sc->wm->var[2] = ui->spnvar2->value();
    ui->txtvar2->setText(QString::number(sc->wm->var[2]));
    sc->wm->var[3] = ui->spnvar3->value();
    ui->txtvar3->setText(QString::number(sc->wm->var[3]));
    sc->wm->var[4] = ui->spnvar4->value();
    ui->txtvar4->setText(QString::number(sc->wm->var[4]));
    sc->wm->var[5] = ui->spnvar5->value();
    ui->txtvar5->setText(QString::number(sc->wm->var[5]));
    sc->wm->var[6] = ui->spnvar6->value();
    ui->txtvar6->setText(QString::number(sc->wm->var[6]));
    sc->wm->var[7] = ui->spnvar7->value();
    ui->txtvar7->setText(QString::number(sc->wm->var[7]));
    sc->wm->var[8] = ui->spnvar8->value();
    ui->txtvar8->setText(QString::number(sc->wm->var[8]));
    sc->wm->var[9] = ui->spnvar9->value();
    ui->txtvar9->setText(QString::number(sc->wm->var[9]));

    //---Debuging Tools----------------
    sc->wm->showAstarOut = ui->astar_checkBox->isChecked();
    sc->wm->indexOfAstarRobot = ui->astarIndex_comboBox->currentText().toInt();

    sc->wm->showVoronoi = ui->show_voronoi->isChecked();

    sc->wm->showDebug = ui->showDebugs->isChecked();
    sc->wm->debug_type = ui->debug_output_type->currentText().toInt();

    sc->wm->showDefenderDebug = ui->showDefence->isChecked();
}

void MainWindow::on_btnSaveVars_clicked()
{
    QString address;
    if(sc->wm->isSim)
        address = "vars_sim.ini";
    else
        address = "vars.ini";

    Settings s(address);
    s.SetValue("vars" , "var0", ui->txtvar0->text());
    s.SetValue("vars" , "var1", ui->txtvar1->text());
    s.SetValue("vars" , "var2", ui->txtvar2->text());
    s.SetValue("vars" , "var3", ui->txtvar3->text());
    s.SetValue("vars" , "var4", ui->txtvar4->text());
    s.SetValue("vars" , "var5", ui->txtvar5->text());
    s.SetValue("vars" , "var6", ui->txtvar6->text());
    s.SetValue("vars" , "var7", ui->txtvar7->text());
    s.SetValue("vars" , "var8", ui->txtvar8->text());
    s.SetValue("vars" , "var9", ui->txtvar9->text());
}

void MainWindow::on_btnLoadVars_clicked()
{
    QString address;
    if(sc->wm->isSim)
        address = "vars_sim.ini";
    else
        address = "vars.ini";

    Settings s(address);

    QString var0 = s.Get("vars", "var0");
    ui->txtvar0->setText(var0);
    ui->spnvar0->setValue(var0.toInt());

    QString var1 = s.Get("vars", "var1");
    ui->txtvar1->setText(var1);
    ui->spnvar1->setValue(var1.toInt());

    QString var2 = s.Get("vars", "var2");
    ui->txtvar2->setText(var2);
    ui->spnvar2->setValue(var2.toInt());

    QString var3 = s.Get("vars", "var3");
    ui->txtvar3->setText(var3);
    ui->spnvar3->setValue(var3.toInt());

    QString var4 = s.Get("vars", "var4");
    ui->txtvar4->setText(var4);
    ui->spnvar4->setValue(var4.toInt());

    QString var5 = s.Get("vars", "var5");
    ui->txtvar5->setText(var5);
    ui->spnvar5->setValue(var5.toInt());

    QString var6 = s.Get("vars", "var6");
    ui->txtvar6->setText(var6);
    ui->spnvar6->setValue(var6.toInt());

    QString var7 = s.Get("vars", "var7");
    ui->txtvar7->setText(var7);
    ui->spnvar7->setValue(var7.toInt());

    QString var8 = s.Get("vars", "var8");
    ui->txtvar8->setText(var8);
    ui->spnvar8->setValue(var8.toInt());

    QString var9 = s.Get("vars", "var9");
    ui->txtvar9->setText(var9);
    ui->spnvar9->setValue(var9.toInt());
}

void MainWindow::on_playpause_button_clicked()
{
    logIsPlayed = !logIsPlayed;
    QString pic_address = (logIsPlayed)? ":/resources/images/pause.png":":/resources/images/play.svg";
    ui->playpause_button->setIcon(QIcon(pic_address));

    if( logIsPlayed )
    {
        sc->playGameLog();
        logTimer->start(1000);
        ui->timeLine_slider->setEnabled(true);

        this->logPlayer_status = "Play";

        QString pic_address = ":/resources/images/stop.svg";
        ui->stoprecord_button->setIcon(QIcon(pic_address));
    }
    else
    {
        sc->pauseGameLog();
        logTimer->stop();
    }
}

void MainWindow::on_stoprecord_button_clicked()
{
    if( this->logPlayer_status == "Play" )
    {
        this->logPlayer_status = "Stop";
        sc->stopGameLog();
        timelineReset();
        ui->playpause_button->setDisabled(true);
        ui->timeLine_slider->setDisabled(true);
        QString pic_address = ":/resources/images/record.png";
        ui->stoprecord_button->setIcon(QIcon(pic_address));
        pic_address = ":/resources/images/play.svg";
        ui->playpause_button->setIcon(QIcon(pic_address));
    }
    else if( this->logPlayer_status == "Record" )
    {
        this->logPlayer_status = "Stop";
        sc->saveGameLog();

        logLength = sc->logLength();
        ui->timeLine_slider->setMaximum(logLength);

        timelineReset();

        ui->playpause_button->setEnabled(true);
        ui->timeLine_slider->setEnabled(true);
        QString pic_address = ":/resources/images/record.png";
        ui->stoprecord_button->setIcon(QIcon(pic_address));
        pic_address = ":/resources/images/play.svg";
        ui->playpause_button->setIcon(QIcon(pic_address));
    }
    else
    {
        this->logPlayer_status = "Record";
        sc->recordGameLog();
        ui->playpause_button->setDisabled(true);
        ui->timeLine_slider->setDisabled(true);
        QString pic_address = ":/resources/images/stop.svg";
        ui->stoprecord_button->setIcon(QIcon(pic_address));
    }
}

void MainWindow::on_openLog_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Your Log File"), "/home", tr("Text Files (*.txt)"));
    if( !fileAddress.isEmpty() )
    {
        this->logPlayer_status = "Play";

        sc->loadGameLog(fileAddress);

        logLength = sc->logLength();
        ui->timeLine_slider->setMaximum(logLength);

        timelineReset();

        ui->playpause_button->setEnabled(true);
        ui->timeLine_slider->setEnabled(true);
        QString pic_address = ":/resources/images/stop.svg";
        ui->stoprecord_button->setIcon(QIcon(pic_address));
        pic_address = ":/resources/images/play.svg";
        ui->playpause_button->setIcon(QIcon(pic_address));
    }
}

void MainWindow::on_timeLine_slider_sliderMoved(int position)
{
    sc->setLogFrame(position);
    ui->timeLabel->setText(convertTime2String(position) + " / " + convertTime2String(logLength));
    counter = (position / 1000) * 1000;
}

void MainWindow::logTimer_timeout()
{
    counter += 1000;
    if( counter > logLength )
    {
        counter = ui->timeLine_slider->maximum() ;
        QString pic_address = ":/resources/images/play.svg";
        ui->playpause_button->setIcon(QIcon(pic_address));
        sc->pauseGameLog();
        this->logIsPlayed = false;
        logTimer->stop();
    }

    ui->timeLabel->setText(convertTime2String(counter) + " / " + convertTime2String(logLength));
    ui->timeLine_slider->setValue(counter);
}
