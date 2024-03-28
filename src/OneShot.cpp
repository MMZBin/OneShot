#include <Arduino.h>
#include "OneShot.h"

OneShot::OneShot(Resolution res)
    : func_(nullptr), interval_(0), remainingTime_(0), endTime_(0), startTime_(0), status_(Status::STOPPED), isOccurred_(false), now_((res == Resolution::MILLIS) ? millis : micros) {  }

void OneShot::registerCallback(CallbackFunc func) {
    if (status_ != Status::STOPPED) { return; }

    func_ = func;
}

void OneShot::registerCallback(CallbackFunc func, uint32_t interval) {
    registerCallback(func);
    setInterval(interval);
}

OneShot::Status OneShot::getStatus() const { return status_; }

bool OneShot::isOccurred() const { return isOccurred_; }

uint32_t OneShot::getInterval() const { return interval_; }

void OneShot::setInterval(uint32_t interval) {
    if (status_ != Status::STOPPED) { return; }

    interval_ = interval;
}

void OneShot::start() {
    if ((status_ != Status::STOPPED) || (interval_ == 0)) { return; }
    status_ = Status::RUNNING;

    startTime_ = now_();
    endTime_ = startTime_ + interval_;
    isOccurred_ = false;
}

void OneShot::pause() {
    if (status_ != Status::RUNNING) { return; }
    status_ = Status::PAUSED;

    remainingTime_ = interval_ - (now_() - startTime_);
}

void OneShot::resume() {
    if (status_ != Status::PAUSED) { return; }
    status_ = Status::RUNNING;

    endTime_ = now_() + remainingTime_; //終了時間を残り時間を元に再計算する
}

void OneShot::cancel() {
    isOccurred_ = false;
    status_ = Status::STOPPED;
}

void OneShot::update() {
    isOccurred_ = false;
    if (status_ != Status::RUNNING) { return; }

    if (now_() >= endTime_) {
        if (func_ != nullptr) { func_(); }
        status_ = Status::STOPPED;
        isOccurred_ = true;
    }
}
