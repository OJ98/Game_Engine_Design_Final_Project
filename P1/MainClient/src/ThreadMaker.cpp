#include"GameHeaders.h"

//Constructor
ThreadMaker::ThreadMaker(int i, std::function<void()> fn, ThreadMaker *other, std::mutex *_mutex, std::condition_variable *_condition_variable) {
    this->i = i; // set the id of this thread
    if(i==0) { busy = true; }
    else { this->other = other; }
    this->fn = fn;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
}

//Destructor
ThreadMaker::~ThreadMaker() {

}

bool ThreadMaker::isBusy() {
    // this locks the mutex until the variable goes out of scope (i.e., when the function returns in this case)
    std::lock_guard<std::mutex> lock(*_mutex);  
    return busy;
}

void ThreadMaker::run() {
    // Executes the function provided and performs exception catching.
    // Since the code is designed to make extensive use of getters and setters, no common data elements exist and thus mutex is implemented 
    // only on the conditional variable as a demo.
    try {
        this->fn();
        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
        busy = !busy;
        _condition_variable->notify_all();
        // std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
        // std::cout << "Thread " << i << " is FINISHED!" << std::endl;
    }
    catch (...) {
        // std::cerr << "Thread " << i << " caught exception." << std::endl;
    }
}

void ThreadMaker::runWrapper() {
    this->run();
}