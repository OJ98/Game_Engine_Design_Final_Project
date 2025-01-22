#pragma once

class ThreadMaker {
    private:
        bool busy; // a member variable used to indicate thread "status"
        int i; // an identifier
        ThreadMaker *other; // a reference to the "other" thread in this example
        std::function<void()> fn; //function to be executed as a part of the thread execution.
        std::mutex *_mutex; // the object for mutual exclusion of execution
        std::condition_variable *_condition_variable; // for messaging between threads
    public:
        ThreadMaker(int, std::function<void()>, ThreadMaker*, std::mutex*,  std::condition_variable*);
        virtual ~ThreadMaker();
        bool isBusy();
        void run();
        void runWrapper();
};