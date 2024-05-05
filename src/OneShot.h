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

#ifndef MMZ_ONESHOT_EVENT_H
#define MMZ_ONESHOT_EVENT_H

class OneShot {
public:
    using CallbackFunc = void (*)();
    using TimeFunc = uint32_t (*)();

    enum class State : uint8_t {
        STOPPED,
        RUNNING,
        PAUSED
    };

    enum class Resolution : uint8_t {
        MILLIS,
        MICROS
    };

    OneShot(const Resolution res=Resolution::MILLIS);

    void registerCallback(const CallbackFunc func);

    void registerCallback(const CallbackFunc func, const uint32_t interval);

    void removeCallback();

    OneShot::Resolution getResolution() const;

    OneShot::TimeFunc getTimeFunc() const;
    uint32_t now() const;

    OneShot::State getState() const;

    bool hasOccurred() const;

    uint32_t getInterval() const;
    void setInterval(const uint32_t interval);

    uint32_t getStartTime() const;
    uint32_t getEndTime() const;
    uint32_t getRemainingTime() const;
    uint32_t getElapsedTime() const;

    void start();
    void start(const uint32_t interval);
    void pause();
    void resume();
    void cancel();

    bool update(); //loop関数内でこれを呼び出してください

private:
    CallbackFunc func_;      //コールバック関数
    uint32_t interval_;      //タイマーが作動するまでの時間
    uint32_t remainingTime_; //タイマーの残り時間(一時停止したとき)
    uint32_t endTime_;       //タイマーが作動する時間
    uint32_t startTime_;     //タイマーが開始した時間

    State state_;            //タイマーの状態
    Resolution resolution_;  //時間の分解能

    bool hasOccurred_;       //タイマーが作動してから次にupdate()メソッドが呼ばれるかキャンセルされるまでtrueになる

    const TimeFunc now_;     //時間の取得用の関数ポインタ
};

#endif