//
// Created by gras on 11/30/23.
//

#include "Timer.h"

#include <SDL_stdinc.h>
#include <SDL_timer.h>


Timer::Timer() {
    paused = false;
    started = false;

    start_ticks = 0;
    paused_ticks = 0;
}

void Timer::stop() {
    paused = false;
    started = false;

    start_ticks = 0;
    paused_ticks = 0;
}

bool Timer::is_paused() {
    return paused && started;
}

bool Timer::is_started() {
    return started;
}

Uint32 Timer::get_ticks() {
    if(!started) {
        return 0;
    }
    if(!paused )
        return SDL_GetTicks() - start_ticks ;

    return paused_ticks;
}

void Timer::unpause() {
    if( started || !paused)
        return;

    paused = false;

    start_ticks = SDL_GetTicks() - (paused_ticks);

    paused_ticks = 0;
}


void Timer::start() {
    started = true;
    paused = false;

    paused_ticks = 0;

    start_ticks = SDL_GetTicks();
}

void Timer::pause() {
    if( paused || !started )
        return;

    paused = true;

    paused_ticks = SDL_GetTicks() - start_ticks;
    start_ticks = 0;
}



