// game_state.h
//
// This class implements the transition system that defines the rules
// of the game.  The state changes based on the input from the referee
// and whether the ball has been kicked since the last referee
// command.  Calling the method transition() with these inputs will
// have the class maintain the state of the game.
//
// In addition, their are query methods for determining the current
// game state and what behavior is allowed in these game states.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
/* LICENSE: */

#ifndef __game_state_h__
#define __game_state_h__

// We want this to be usable by other teams, so below are the
// constants that this module makes use of.  If this is not being used
// within the CMDragons system then the #if 1 should be changed to a
// #if 0.
//
// These constants should match with those found in constants.h.
//

#if 0
#include <constants.h>
#else
#define TEAM_BLUE 0
#define TEAM_YELLOW 1
#endif

#include <commands.h>

class GameState {
  static const int GAME_ON;
  static const int GAME_OFF;
  static const int HALTED;

  static const int KICKOFF;
  static const int PENALTY;
  static const int DIRECT;
  static const int INDIRECT;
  static const int RESTART;

  static const int BLUE;
  static const int YELLOW;

  static const int READY;
  static const int NOTREADY;

  static const int TIMEOUT;

  int state;

  // The set of possible states are:
  //
  // { GAME_ON, GAME_OFF, HALTED, NEUTRAL,
  //   { { KICKOFF, PENALTY, DIRECT, INDIRECT } |
  //     { BLUE, YELLOW } | { READY, NOTREADY } } }
  //

  int color;

public:
  GameState() { color = BLUE; state = GAME_OFF; }

  void init(int _color) { color = (_color == TEAM_BLUE) ? BLUE : YELLOW; }

  int get() { return state; }
  void set(int _state) { state = _state; }

  // This is the state machine transition function.  It takes the last
  // ref_command as input
  void transition(char ref_command, bool ball_kicked) {
    if (ref_command == COMM_HALT) {
      state = HALTED; return; }

    if (ref_command == COMM_STOP) {
      state = GAME_OFF; return; }

    if (ref_command == COMM_START) {
      state = GAME_ON; return; }

    if (ref_command == COMM_READY && state & NOTREADY) {
      state &= ~NOTREADY; state |= READY; return; }

    if (state & READY && ball_kicked) {
      state = GAME_ON; return; }

    if (state == GAME_OFF) {
      switch (ref_command) {
      case COMM_KICKOFF_BLUE:
        state = KICKOFF | BLUE | NOTREADY; return;
      case COMM_KICKOFF_YELLOW:
        state = KICKOFF | YELLOW | NOTREADY; return;

      case COMM_PENALTY_BLUE:
        state = PENALTY | BLUE | NOTREADY; return;
      case COMM_PENALTY_YELLOW:
        state = PENALTY | YELLOW | NOTREADY; return;

      case COMM_DIRECT_BLUE:
        state = DIRECT | BLUE | READY; return;
      case COMM_DIRECT_YELLOW:
        state = DIRECT | YELLOW | READY; return;

      case COMM_INDIRECT_BLUE:
        state = INDIRECT | BLUE | READY; return;
      case COMM_INDIRECT_YELLOW:
        state = INDIRECT | YELLOW | READY; return;

      case COMM_TIMEOUT_BLUE:
          state = TIMEOUT | BLUE | NOTREADY; return;
      case COMM_TIMEOUT_YELLOW:
          state = TIMEOUT | YELLOW | NOTREADY; return;

      default: break;
      }
    }
  }

  bool gameOn() { return (state == GAME_ON); }

  bool restart() { return (state & RESTART); }
  bool ourRestart() { return restart() && (state & color); }
  bool theirRestart() { return restart() && ! (state & color); }

  bool kickoff() { return (state & KICKOFF); }
  bool blueKickoff() { return kickoff() && (state & color); }
  bool yellowKickoff() { return kickoff() && ! (state & color); }

  bool penaltyKick() { return (state & PENALTY); }
  bool bluePenaltyKick() { return penaltyKick() && (state & color); }
  bool yellowPenaltyKick() { return penaltyKick() && ! (state & color); }

  bool directKick() { return (state & DIRECT); }
  bool blueDirectKick() { return directKick() && (state & color); }
  bool yellowDirectKick() { return directKick() && ! (state & color); }

  bool indirectKick() { return (state & INDIRECT); }
  bool blueIndirectKick() { return indirectKick() && (state & color); }
  bool yellowIndirectKick() { return indirectKick() && ! (state & color); }

  bool freeKick() { return directKick() || indirectKick(); }
  bool blueFreeKick() { return blueDirectKick() || blueIndirectKick(); }
  bool yellowFreeKick() { return yellowDirectKick() || yellowIndirectKick(); }

  bool canMove() { return (state != HALTED); }

  bool allowedNearBall() {
    return gameOn() || (state & color); }

  bool canKickBall() {
    return gameOn() || (ourRestart() && (state & READY)); }

  bool timeout(){
      return (state & TIMEOUT);
  }

};

#endif


