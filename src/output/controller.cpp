﻿#include "controller.h"
#include "constants.h"

#define ROBOTRADIUS 0.090
#define SpeedToRPMR 5000
#define SpeedToRPM 1375.14
Controller::Controller(QObject *parent) :
    QObject(parent)
  ,out("/home/kn2c/Desktop/Untitled Folder/Data.txt")
{

    qDebug() << "Controller Initialization...";

    timer.start();


    err0 = {0,0};
    err1 = {0,0};
    u1 = {0,0};
    derived0 = {0,0};
    derived1 = {0,0};
    integral = {0,0};
    last_setpoint = {0,0};

    wu1 = 0;
    wu1_last = 0;
    wintegral = 0;
    werr0 = 0;
    werr1 = 0;
    wderived0 = 0;
    wderived1 = 0;

    stateCTRL = 0;

    LinearSpeed = {0,0};
    LinearSpeed_past = {0,0};

}

ControllerResult Controller::calc(ControllerInput &ci)
{
    ControllerResult ctrlresult;
    double time = timer.elapsed();
    timer.restart();
    ctrlresult.rs = calcRobotSpeed_main(ci);
    //ctrlresult.rs = calcRobotSpeed_adjt(ci);
    //ctrlresult.rs = calcRobotSpeed_test(ci);
    //qDebug() << "id" << ci.id << "timer" << time;

    ctrlresult.msR = calcReal(ctrlresult.rs);
    ctrlresult.msS = calcSimul(ctrlresult.rs);
    return ctrlresult;
}

RobotSpeed Controller::calcRobotSpeed_main(ControllerInput &ci)
{
    //double time = timer.elapsed()/1000;
    //timer.restart();
    /******************************Linear Speed Controller************************************/
    err1 = (ci.mid_pos.loc - ci.cur_pos.loc)*.001;

//    if(ci.maxSpeed == 3)
//        ci.maxSpeed == 4;


    ///!
//    if((ci.mid_pos.loc - last_setpoint ).length() > 30)
//    {
////            qDebug() <<"L"<<err1.length()<<(err1 + (ci.mid_pos.loc - last_setpoint ) *0.009).length();
////             qDebug() <<"D"<<err1.dir().radian()<<(err1 + (ci.mid_pos.loc - last_setpoint ) *0.009).dir().radian();
////        err1 = err1 + (ci.mid_pos.loc - last_setpoint ) *0.009;
//        err1.setLength((err1 + (ci.mid_pos.loc - last_setpoint ) *0.009).length());
//    }
//    last_setpoint = ci.mid_pos.loc;
    //!!

    double kp;
    double ki;
    double kd;

    float err = err1.length();




    if(err > 1.5)
    {
        kp = fabs(err1.length());
        ki = 0.01;
        kd = 3;
        integral = integral + err1*AI_TIMER/1000.0 ;
    }
    else /*if(err > .04)*/
    {
        integral.scale(0);
        if(ci.fin_pos.loc == ci.mid_pos.loc)
        {
            kp = 5-2*fabs(ci.cur_vel.loc.length());fabs(err1.length())+2.9;////day2//f
                if((ci.mid_pos.loc - last_setpoint ).length() > 30)
                {
                    ki=.3;
                }
                last_setpoint = ci.mid_pos.loc;
        }
        else
        {
            kp = 4;
        }
        kd = 3;

    }




    derived1 = (ci.cur_vel.loc - derived0)*0.1;
    derived0 = ci.cur_vel.loc;



    LinearSpeed = err1*kp + integral*ki*err - derived1*kd;


//    ////////////////////////////////////////day2
    double diff_angel = ci.cur_vel.dir - LinearSpeed.dir().radian();
    if (diff_angel > M_PI) diff_angel -= 2 * M_PI;
    if (diff_angel < -M_PI) diff_angel += 2 * M_PI;
    if(fabs(diff_angel) > M_PI*0.7)
    {
        LinearSpeed = LinearSpeed_past + (LinearSpeed - LinearSpeed_past)*0.01;//*50/(ci.mid_pos.loc - last_setpoint).length();

//        if(ci.id==8)
//        qDebug()<<"filter"<<(ci.mid_pos.loc - last_setpoint).length();
    }
    LinearSpeed_past = LinearSpeed ;
//    ////////////////////////////////////////day2

    if(LinearSpeed.length()>ci.maxSpeed)
    {
        LinearSpeed.setLength(ci.maxSpeed);
    }

    if(ci.id == 8)
    qDebug()<<LinearSpeed.length();
    Vector2D RotLinearSpeed=LinearSpeed;
    LinearSpeed_past = LinearSpeed ;
    /*************************************Rotation ctrl**********************/
    double wkp,wki,wkd,wu1;
    double MAXROTATIONSPEED = 2.5,RotationSpeed;
    werr1 = ci.fin_pos.dir - ci.cur_pos.dir ;

    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;

    double werr = fabs(werr1);


    //wki=0.003*fabs(werr0);
    //wintegral = wintegral + werr1*AI_TIMER;

    wkp=1*fabs(werr0);//0.3;
    wkd=1;
    if(werr<.3 + 10*fabs(ci.cur_vel.dir))
    {
         wintegral=0;
        //wintegral = wintegral - 3*werr1*AI_TIMER;

        wkp=0.4;
        wkd=0.003;
    }



    //if(ci.id == 3)
    //qDebug()<<werr1<<wintegral;
    werr0=werr0+(werr1-werr0)*0.1;

    wderived1 = (ci.cur_vel.dir)*10;
    wderived0 = ci.cur_vel.dir;
    wu1 = werr1*wkp + wintegral*wki - wderived1*wkd;


    if (fabs(wu1)>MAXROTATIONSPEED)
    {
        wintegral = wintegral - werr1*AI_TIMER;
        wu1=MAXROTATIONSPEED*sign(wu1);
    }

    RotationSpeed = wu1;


    double alpha = ci.cur_pos.dir ;+atan(RotationSpeed*0.187);
    //alpha is the corrected angel whitch handle the problem
    //of nonlinear relation of rotational movement and linear movement




    RotLinearSpeed.x = LinearSpeed.x * cos(alpha) + LinearSpeed.y * sin(alpha);
    RotLinearSpeed.y = -LinearSpeed.x * sin(alpha) + LinearSpeed.y * cos(alpha);

    RobotSpeed ans;

    ans.VX = RotLinearSpeed.x;
    ans.VY = RotLinearSpeed.y;
    ans.VW = RotationSpeed ;

    if(werr <0.07  /*&& err1.length()<.015*/) ans.VW=0;//maximum priscision in angel for robot becuse of it/s phisic's limits is 0.07 rad


    if(err1.length()<.02)
    {
        ans.VX=0;
        ans.VY=0;
    }

    return ans;
}

RobotSpeed Controller::calcRobotSpeed_adjt(ControllerInput &ci)
{
    float RotationSpeed;

    RotationSpeed = wu1;
    Vector2D RotLinearSpeed;
    switch(stateCTRL)
    {
    case 0://jelo
        RotationSpeed = 0;
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        RotLinearSpeed = Vector2D(1.5,0);//sorate robot jelo
        if((Vector2D(1500,0) - ci.cur_pos.loc).length()>500 && fabs(werr1)>M_PI/2.0)
        {
            stateCTRL = 3;
        }
        break;
    case 1://aghab
        RotationSpeed = 0;
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        RotLinearSpeed = Vector2D(-.3,0);//sorate robot aghab
        if((Vector2D(1500,0) - ci.cur_pos.loc).length()>500&& fabs(werr1)<M_PI/2.0)
        {
            stateCTRL = 2;
        }
        break;
    case 2://charkhesh
        RotLinearSpeed = Vector2D(0,0);
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;

        if(werr1<-(M_PI/18.0))
        {
            RotationSpeed = -1;
        }
        else if(werr1>(M_PI/18.0))
        {
            RotationSpeed = 1;
        }
        else
        {
            RotationSpeed = 0;
            stateCTRL = 0;
        }

        break;
    case 3://charkhesh
        RotLinearSpeed = Vector2D(0,0);
        werr1 = ((Vector2D(1200,0)-ci.cur_pos.loc).dir().radian() + M_PI) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;

        if(werr1<-(M_PI/18.0))
        {
            RotationSpeed = -1;
        }
        else if(werr1>(M_PI/18.0))
        {
            RotationSpeed = 1;
        }
        else
        {
            RotationSpeed = 0;
            stateCTRL = 1;
        }
        break;
    }


    RobotSpeed ans;

    ans.VX = RotLinearSpeed.x;
    ans.VY = RotLinearSpeed.y;
    ans.VW = RotationSpeed;

    return ans;
}

RobotSpeed Controller::calcRobotSpeed_test(ControllerInput &ci)
{

    float RotationSpeed;

    double ap=1;
    double am=1;
    double am2=1;
    double t0;
    //double t1,t2;
    double s0;
    double s3;
    double s1;
    double v,dt,s,sp,vb;
    double tp;
    double t2p;
    double t3;
    /******************************Linear Speed Controller************************************/
    Vector2D RotLinearSpeed;
    err0 = err1;
    err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;

    t0 = -ci.cur_vel.loc.length()/ap;
    s0 = -ci.cur_vel.loc.length()*t0/2;
    s3 = pow(ci.fin_vel.loc.length(),2)/(2*am);
    v = 0;//sqrt(s1*2*ap);
    tp = (v/ap)+t0;
    t3 = (v/am) + tp;
    t2p = t3 - (ci.fin_vel.loc.length()/am);

    if(v>ci.maxSpeed)
    {
        s = err1.length() + s0 + s3;
        sp = s * pow((v-ci.maxSpeed)/v,2);
        dt = sp/ci.maxSpeed;
        t3 = t3 + dt;
        t2p = t2p + dt;
    }

    double dist;
    vb=ci.maxSpeed/2.0;
    if (ci.cur_vel.loc.length()<vb)
    {
        dist = (pow(ci.fin_vel.loc.length(),2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am2);
    }
    else
    {

        dist =(pow(vb,2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am);
        dist+=(pow(ci.fin_vel.loc.length(),2)-pow(vb,2))/(-2.0*am2);


    }

    u1 = err1;

    if(err1.length()<=dist)
    {
        if(ci.cur_vel.loc.length()<vb)
            u1.setLength(sqrt(2.0*am2*(err1.length())+pow(ci.fin_vel.loc.length(),2)));
        else

            u1.setLength(sqrt(2.0*am*(err1.length())+pow(ci.fin_vel.loc.length(),2)));
    }
    else if(err1.length()>dist)
    {
        t0 = -ci.cur_vel.loc.length()/ap;

        s0 = -ci.cur_vel.loc.length()*t0/2;
        s3 = pow(ci.fin_vel.loc.length(),2)/(2*am);
        s1 = (err1.length()+s0+s3)/(1+ap/am);
        v = sqrt(s1*2*ap);
        tp = (v/ap)+t0;
        t3 = (v/am) + tp;
        t2p = t3 - (ci.fin_vel.loc.length()/am);
        //t2 = t2p;
        double Sm = (pow(v,2)-pow(ci.fin_vel.loc.length(),2))/(2.0*am);
        u1.setLength(sqrt(2.0*ap*((err1.length()-Sm))+pow(v,2)));

    }

    if(u1.length()>ci.maxSpeed)
    {
        u1.setLength(ci.maxSpeed);
    }
    Vector2D LinearSpeed;

    LinearSpeed = u1;

    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);
    RotationSpeed=0;
    RobotSpeed result;

    result.VX = RotLinearSpeed.x;
    result.VY = RotLinearSpeed.y;
    result.VW = RotationSpeed;

    out << err1.y <<" "<< LinearSpeed.x <<" "<< LinearSpeed.y << endl;
    return result;
}

MotorSpeed Controller::calcReal(RobotSpeed rs)
{
    double motor[4][1],rotate[4][3],speed[3][1];

    speed[0][0] = -rs.VX;
    speed[1][0] = -rs.VY;
    speed[2][0] = -rs.VW;

    rotate[0][0] =  cos( 0.18716 * M_PI);//cos(M_PI /4.0);//-sin(rangle + M_PI);//7/4
    rotate[1][0] =  sin( M_PI / 4.0 );//-cos(0.22 * M_PI);//-sin(rangle - M_PI / 3);//0.218
    rotate[2][0] =  -cos( M_PI / 4.0 );//-sin(0.22 * M_PI);//-sin(rangle + M_PI / 3);//0.78
    rotate[3][0] =  -cos( 0.18716 * M_PI);//cos(M_PI /4.0);//-sin(rangle + M_PI);//5/4
    rotate[0][1] =  -sin(0.18716 * M_PI );//cos(M_PI /4.0);//cos(rangle + M_PI);//7/4
    rotate[1][1] = cos(M_PI / 4.0 );//- sin(0.22 * M_PI);// cos(rangle - M_PI / 3);//0.218
    rotate[2][1] = sin(M_PI / 4.0);//cos(0.22 * M_PI);//cos(rangle + M_PI / 3);//0.187
    rotate[3][1] = -sin(0.18716 * M_PI);//-cos(M_PI /4.0);//cos(rangle + M_PI);//5/4

    rotate[0][2] = -ROBOTRADIUS;
    rotate[1][2] = -ROBOTRADIUS;
    rotate[2][2] = -ROBOTRADIUS;
    rotate[3][2] = -ROBOTRADIUS;

    motor[0][0] = (rotate[0][0] * speed[0][0] + rotate[0][1] * speed[1][0])*SpeedToRPM + rotate[0][2] * speed[2][0]*SpeedToRPMR;
    motor[1][0] = (rotate[1][0] * speed[0][0] + rotate[1][1] * speed[1][0])*SpeedToRPM + rotate[1][2] * speed[2][0]*SpeedToRPMR;
    motor[2][0] = (rotate[2][0] * speed[0][0] + rotate[2][1] * speed[1][0])*SpeedToRPM + rotate[2][2] * speed[2][0]*SpeedToRPMR;
    motor[3][0] = (rotate[3][0] * speed[0][0] + rotate[3][1] * speed[1][0])*SpeedToRPM + rotate[3][2] * speed[2][0]*SpeedToRPMR;

    MotorSpeed result;

    result.M0 = (motor[0][0]);
    result.M1 = (motor[1][0]);
    result.M2 = (motor[2][0]);
    result.M3 = (motor[3][0]);

    return result;
}

MotorSpeed Controller::calcSimul(RobotSpeed rs)
{
    double motor[4][1],rotate[4][3],speed[3][1];

    speed[0][0] = rs.VX;
    speed[1][0] = rs.VY;
    speed[2][0] = rs.VW;

    rotate[0][0] = sin(M_PI / 3);//-sin(rangle - M_PI / 3);
    rotate[1][0] = sin(3 * M_PI / 4);//-sin(rangle + M_PI / 3);
    rotate[2][0] = sin(5 * M_PI / 4);//-sin(rangle + M_PI);
    rotate[3][0] = sin(5 * M_PI / 3);//-sin(rangle + M_PI);
    rotate[0][1] = -cos(M_PI / 3);//cos(rangle - M_PI / 3);
    rotate[1][1] = -cos(3 * M_PI / 4);//cos(rangle + M_PI / 3);
    rotate[2][1] = -cos(5 * M_PI / 4);//cos(rangle + M_PI);
    rotate[3][1] = -cos(5 * M_PI / 3);//cos(rangle + M_PI);
    rotate[0][2] = -ROBOTRADIUS;
    rotate[1][2] = -ROBOTRADIUS;
    rotate[2][2] = -ROBOTRADIUS;
    rotate[3][2] = -ROBOTRADIUS;

    motor[0][0] = (rotate[0][0] * speed[0][0] + rotate[0][1] * speed[1][0] + rotate[0][2] * speed[2][0])*100;
    motor[1][0] = (rotate[1][0] * speed[0][0] + rotate[1][1] * speed[1][0] + rotate[1][2] * speed[2][0])*100;
    motor[2][0] = (rotate[2][0] * speed[0][0] + rotate[2][1] * speed[1][0] + rotate[2][2] * speed[2][0])*100;
    motor[3][0] = (rotate[3][0] * speed[0][0] + rotate[3][1] * speed[1][0] + rotate[3][2] * speed[2][0])*100;

    //qDebug()<<"M0:"<<motor[0][0]<<"M1:"<<motor[1][0]<<"M2:"<<motor[2][0]<<"M3:"<<motor[3][0];
    //qDebug()<<err1.y;
    //out << err1.y <<" "<< endl;
    MotorSpeed result;

    result.M0 = -motor[0][0]/100;//*SpeedToRPM;//
    result.M1 = -motor[1][0]/100;//*SpeedToRPM;//
    result.M2 = -motor[2][0]/100;//*SpeedToRPM;//
    result.M3 = -motor[3][0]/100;//*SpeedToRPM;//

    return result;
}