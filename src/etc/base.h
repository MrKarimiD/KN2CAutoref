#ifndef BASE_H
#define BASE_H

enum TeamSideType
{
    SIDE_RIGHT,
    SIDE_LEFT
};

enum TeamColorType
{
    COLOR_BLUE = 0,
    COLOR_YELLOW = 1
};

enum CameraConfigType
{
    CAMERA_NONE = 0,
    CAMERA_0 = 1,
    CAMERA_1 = 2,
    CAMERA_2 = 3,
    CAMERA_3 = 4,
    CAMERA_BOTH_L = 5,
    CAMERA_BOTH_R = 6,
    CAMERA_ALL = 7
};

enum GameModeType
{
    MODE_REAL,
    MODE_SIMULATION
};

enum GameStateType
{
    STATE_Null = 0,

    STATE_Halt = 1, // OK!
    STATE_Stop = 2, // OK!

    STATE_Start = 3,
    STATE_ForceStart= 4, // OK!

    STATE_Kick_off_Blue = 5, // OK!
    STATE_Kick_off_Yellow = 6, // OK!

    STATE_Penalty_Blue = 7, // OK!
    STATE_Penalty_Yellow = 8, // OK!

    STATE_Free_kick_Blue = 9,
    STATE_Free_kick_Yellow = 10,

    STATE_Indirect_Free_kick_Blue = 11,
    STATE_Indirect_Free_kick_Yellow = 12,


};

#endif // BASE_H
