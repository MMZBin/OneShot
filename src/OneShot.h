#ifndef MMZ_ONESHOT_EVENT
#define MMZ_ONESHOT_EVENT

class OneShot {
public:
    using CallbackFunc = void (*)();

    enum class State : uint8_t {
        STOPPED,
        RUNNING,
        PAUSED
    };

    enum class Resolution {
        MILLIS,
        MICROS
    };

    OneShot(Resolution res=Resolution::MILLIS);

    void registerCallback(CallbackFunc func);

    void registerCallback(CallbackFunc func, uint32_t interval);

    void removeCallback();

    OneShot::State getState() const;

    bool isOccurred() const;

    uint32_t getInterval() const;

    void setInterval(uint32_t interval);

    void start();

    void pause();

    void resume();

    void cancel();

    void update(); //loop関数内でこれを呼び出してください

private:
    using TimeFunc = uint32_t (*)();

    CallbackFunc func_;      //コールバック関数
    uint32_t interval_;      //タイマーが作動するまでの時間
    uint32_t remainingTime_; //タイマーの残り時間(一時停止したとき)
    uint32_t endTime_;       //タイマーが作動する時間
    uint32_t startTime_;     //タイマーが開始した時間

    State state_;            //タイマーの状態
    bool isOccurred_;        //タイマーが作動してから次にupdate()メソッドが呼ばれるかキャンセルされるまでtrueになる

    const TimeFunc now_;     //時間の取得用の関数ポインタ
};

#endif
