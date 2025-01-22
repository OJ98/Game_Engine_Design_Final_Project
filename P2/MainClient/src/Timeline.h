#pragma once

class Timeline {
    private:
        std::chrono::_V2::system_clock::time_point start_time_CPU;
        std::chrono::_V2::system_clock::time_point current_time_CPU;
        
        // All in milliseconds
        int64_t current_time;
        int64_t tic_size;
        int64_t paused_time_start;
        int64_t paused_time_current;
        
        int64_t tic_iterations;
        int64_t paused_tic_iterations;
    
    public:
        Timeline();
        virtual ~Timeline();

        bool Paused;

        //Accessors
        int64_t getTime_Absolute();
        int64_t getTime_Tics();
        int64_t getTime_Loops();
        
        int64_t getTimePaused_Absolute();
        int64_t getTimePaused_Tics();
        int64_t getTimePaused_Loops();

        int64_t getTicSize();
        int64_t getTimeRemainingTic();

        //Modifiers
        void setTicSize(int64_t);
        void pause();
        void unpause();

        //Update Time
        void updateTime();
        
        //Resets
        void resetPauseTimers();
        void resetMainTimers();
};