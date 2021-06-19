/**
 * File: Timer.cc
 * Author: Jing Yonglin
 * Description: Class implementation for a simple timer
 */

#include <chrono>
#include <iostream>

#include "utils/Timer.hpp"

using std::cout;
using std::endl;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

void Timer::Start(){
    start_pt_ = steady_clock::now();
}

double Timer::Stop(){
    steady_clock::time_point stop_pt = steady_clock::now();
    std::chrono::duration<double> time_used = duration_cast<std::chrono::duration<double>>(stop_pt - start_pt_);

    return time_used.count();
}