#include"GameHeaders.h"
using namespace std::chrono;

// Constructor
// Default initialization is to tic based mode with tic size = 20ms
Timeline::Timeline() {
    this->start_time_CPU = high_resolution_clock::now(); 
    this->resetMainTimers();
    this->resetPauseTimers();
    this->Paused = false;
    this->tic_size = 20;
}

// Destructor
Timeline::~Timeline() {

}

// updates time.
void Timeline::updateTime() {
    this->current_time_CPU = high_resolution_clock::now();
    auto elapsed = this->current_time_CPU - this->start_time_CPU;
    this->current_time = duration_cast<milliseconds>(elapsed).count();
    this->tic_iterations = int64_t(this->current_time / this->tic_size);
    if (this->Paused) {
        this->paused_time_current = this->current_time - this->paused_time_start;
    }
    return;
}

// returns the current tic size 
int64_t Timeline::getTicSize() {
    return (this->tic_size);
}

// returns the number of ms left in current tic
int64_t Timeline::getTimeRemainingTic() {
    return (this->current_time - (this->tic_size * this->tic_iterations));
}

// returns the time since instantiation of timeline object in absolute terms.
int64_t Timeline::getTime_Absolute() {
    return this->current_time;
}

// returns the time since instantiation of timeline object in terms of the current tic size.
int64_t Timeline::getTime_Tics() {
    return this->tic_iterations;
}

// returns the time spent paused in absolute terms.
int64_t Timeline::getTimePaused_Absolute() {
    return this->paused_time_current;
}

// returns the time spent paused in terms of the current tic size.
int64_t Timeline::getTimePaused_Tics() {
    return int64_t(this->paused_time_current / this->tic_size);
}

// sets the tic size 
void Timeline::setTicSize(int64_t new_tic_size) {
    this->tic_size = new_tic_size;
    return;
}

// pause the timeline.
void Timeline::pause() {
    this->Paused = true;
    this->paused_time_start = this->current_time;
    return;
}

// unpause the timeline
void Timeline::unpause() {
    this->Paused = false;
    this->resetPauseTimers();
}

// resets the Pause timers.
void Timeline::resetPauseTimers() {
    this->paused_tic_iterations = 0;
    this->paused_time_start = 0;
    this->paused_time_current = 0;
    return;
}

// resets the Main timers.
void Timeline::resetMainTimers() {
    this->tic_iterations = 0;
    this->current_time = 0;
    return;
}