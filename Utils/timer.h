//
// Created by Collin Shuey on 7/16/24.
//
// Timing function
#ifndef HASHMAP_TIMER_H
#define HASHMAP_TIMER_H


#include <chrono>

template <typename F>
long execution_time(F func) {
    using namespace std::chrono;
    auto start = system_clock::now();
    func();
    auto dur = duration_cast<milliseconds>(system_clock::now() - start);
    return dur.count();
}

#endif //HASHMAP_TIMER_H
