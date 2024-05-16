/*
    MIT License

    Copyright (c) 2024 MOMIZICH

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <Arduino.h>
#include "OneShot.h"

OneShot::OneShot(const TimeFunc timeFunc)
    : func_(nullptr), interval_(0), remainingTime_(0), endTime_(0), startTime_(0), state_(State::STOPPED), hasOccurred_(false), now_(timeFunc) {  }

void OneShot::registerCallback(const CallbackFunc func) {
    if (state_ != State::STOPPED) { return; }

    func_ = func;
}

void OneShot::registerCallback(const CallbackFunc func, const uint32_t interval) {
    registerCallback(func);
    setInterval(interval);
}

void OneShot::removeCallback() { registerCallback(nullptr); }

OneShot::TimeFunc OneShot::getTimeFunc() const { return now_; }
uint32_t OneShot::now() const { return now_(); }

OneShot::State OneShot::getState() const { return state_; }

bool OneShot::hasOccurred() const { return hasOccurred_; }

uint32_t OneShot::getInterval() const { return interval_; }

void OneShot::setInterval(const uint32_t interval) {
    if (state_ != State::STOPPED) { return; }

    interval_ = interval;
}

uint32_t OneShot::getStartTime() const { return startTime_; }

uint32_t OneShot::getEndTime() const { return endTime_; }

uint32_t OneShot::getRemainingTime() const {
    if (state_ == State::STOPPED) { return 0; }
    if (state_ == State::PAUSED) { return remainingTime_; }
    return interval_ - (now_() - startTime_);
}

uint32_t OneShot::getElapsedTime() const {
    if (state_ == State::STOPPED) { return 0; }
    return interval_ - getRemainingTime();
}

void OneShot::start() {
    if ((state_ != State::STOPPED) || (interval_ == 0)) { return; }
    state_ = State::RUNNING;

    startTime_ = now_();
    endTime_ = startTime_ + interval_;
    hasOccurred_ = false;
}

void OneShot::start(const uint32_t interval) {
    setInterval(interval);
    start();
}

void OneShot::pause() {
    if (state_ != State::RUNNING) { return; }
    remainingTime_ = getRemainingTime();
    state_ = State::PAUSED;
}

void OneShot::resume() {
    if (state_ != State::PAUSED) { return; }
    state_ = State::RUNNING;

    endTime_ = now_() + remainingTime_; //終了時間を残り時間を元に再計算する
}

void OneShot::cancel() {
    hasOccurred_ = false;

    startTime_ = 0;
    endTime_ = 0;
    remainingTime_ = 0;

    state_ = State::STOPPED;
}

bool OneShot::update() {
    hasOccurred_ = false;
    if (state_ != State::RUNNING) { return false; }

    if (now_() >= endTime_) {
        if (func_ != nullptr) { func_(); }
        state_ = State::STOPPED;
        hasOccurred_ = true;
    }

    return hasOccurred_;
}
