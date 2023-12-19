//
// Created by gras on 11/30/23.
//
#pragma once
#ifndef TIMER_H
#define TIMER_H
#include <SDL_stdinc.h>


class Timer {
public:
    //initializes
    Timer();

    //return the current ticks
    Uint32 get_ticks();

    //pauses the current timer
    void pause();

    //unpause the current timer
    void unpause();

    //start the current timer
    void start();

    //returns true if the timer is paused
    bool is_paused();

    //returns true if the timer is started
    bool is_started();

    //stops the timer, initializes it
    void stop();

private:
    Uint32 start_ticks;
    Uint32 paused_ticks;

    bool paused;
    bool started;
};



#endif //TIMER_H
