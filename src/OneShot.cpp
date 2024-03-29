#include <Arduino.h>
#include "OneShot.h"

OneShot::OneShot(Resolution res)
    : func_(nullptr), interval_(0), remainingTime_(0), endTime_(0), startTime_(0), state_(State::STOPPED), isOccurred_(false), now_((res == Resolution::MILLIS) ? millis : micros) {  }

void OneShot::registerCallback(CallbackFunc func) {
    if (state_ != State::STOPPED) { return; }

    func_ = func;
}

void OneShot::registerCallback(CallbackFunc func, uint32_t interval) {
    registerCallback(func);
    setInterval(interval);
}

void OneShot::removeCallback() { registerCallback(nullptr); }

OneShot::State OneShot::getState() const { return state_; }

bool OneShot::isOccurred() const { return isOccurred_; }

uint32_t OneShot::getInterval() const { return interval_; }

void OneShot::setInterval(uint32_t interval) {
    if (state_ != State::STOPPED) { return; }

    interval_ = interval;
}

uint32_t OneShot::getStartTime() const { return startTime_; }

uint32_t OneShot::getEndTime() const { return endTime_; }

uint32_t OneShot::getRemainingTime() const {
    if (state_ == State::PAUSED) { return remainingTime_; }
    return interval_ - (now_() - startTime_);
}

uint32_t OneShot::getElapsedTime() const { return (endTime_ - startTime_) - getRemainingTime(); }

void OneShot::start() {
    if ((state_ != State::STOPPED) || (interval_ == 0)) { return; }
    state_ = State::RUNNING;

    startTime_ = now_();
    endTime_ = startTime_ + interval_;
    isOccurred_ = false;
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
    isOccurred_ = false;

    startTime_ = 0;
    endTime_ = 0;
    remainingTime_ = 0;

    state_ = State::STOPPED;
}

void OneShot::update() {
    isOccurred_ = false;
    if (state_ != State::RUNNING) { return; }

    if (now_() >= endTime_) {
        if (func_ != nullptr) { func_(); }
        state_ = State::STOPPED;
        isOccurred_ = true;
    }
}
