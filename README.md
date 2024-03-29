This library uses English that has been machine-translated from Japanese.

# OneShot

一回限りのワンショットイベントを生成するライブラリです。
イベントは再トリガー(`start()`)することで何度でも利用可能です。
コールバック関数は必ずしも登録する必要はありません。

This is a library for generating one-shot events, which occur only once.
The event can be reused multiple times by triggering it again (`start()`).
It's not always necessary to register a callback function.

## 機能 Functions

#### enum class State
- タイマーの状態を表します。 Represents the state of the timer.
  - STOPPED
  - RUNNING
  - PAUSED

#### enum class Resolution
- タイマーの分解能を設定します。 Sets the resolution of the timer.
  - MILLIS
  - MICROS

#### OneShot(OneShot::Resolution res)
  - OneShotオブジェクトを生成します。 Generates a OneShot object.
    - res
      - タイマーの分解能を設定します。 Sets the resolution of the timer.
      - デフォルトではMILLISです。 Default is MILLIS.

#### void registerCallback(CallbackFunc func)
  - タイマーイベントが発生した際に呼び出されるコールバック関数を登録します。 Registers a callback function to be called when the timer event occurs.
    - func
      - 登録するコールバック関数 Callback function to register.

#### void registerCallback(CallbackFunc func, uint32_t interval)
  - コールバック関数とインターバルを一度に登録します。 Registers a callback function and interval at once.
    - func
      - 登録するコールバック関数 Callback function to register.
    -interval
      - イベントが発生するまでの間隔(ミリ秒もしくはマイクロ秒) Time until the event occurs (in milliseconds or microseconds).
     
#### void removeCallback()
  - コールバック関数を削除します。 Delete the callback function.

#### OneShot::State getState()
  - 現在の状態をState列挙型で返します。 Returns the current state as State enumeration.

#### bool hasOccurred()
  - イベントが発生したと判定されてから次にupdate()メソッドが実行されるまでtrueになります。 Returns true if an event has occurred until the next update() method is executed.

#### uint32_t getInterval()
  - 現在のイベントが発生するまでの間隔を返します。 Returns the current interval until the event occurs.

#### void setInterval(uint32_t interval)
  - イベントが発生するまでの時間を設定します。 Sets the time until the event occurs.
  - タイマーが動作している間は無効になります。 It becomes invalid while the timer is running.
    - interval
      - イベントが発生するまでの時間(ミリ秒もしくはマイクロ秒) Time until the event occurs (in milliseconds or microseconds).
     
#### uint32_t getStartTime()
  - タイマーが開始された時間を返します。 Returns the time when the timer was started.
  - 停止中(State::STOPPED)の間は0になります。 During the stopped state (State::STOPPED), it will return 0.

#### uint32_t getEndTime()
  - イベントが発生する時間を返します。 Returns the time when the event occurs.
  - 停止中(State::STOPPED)の間は0になります。 During the stopped state (State::STOPPED), it will return 0.

#### uint32_t getRemainingTime()
  - タイマーが動作するまでの残り時間を返します。 Returns the remaining time until the timer operates.
  - 停止中(State::STOPPED)の間は0になります。 During the stopped state (State::STOPPED), it will return 0.

#### getElapsedTime()
  - タイマーが開始されてからの経過時間を返します。
  - 停止中(State::STOPPED)の間は0になります。 During the stopped state (State::STOPPED), it will return 0.


#### void start()
  - タイマーを開始します。 Starts the timer.
  - すでに動作している間に実行された場合、なにもしません。 Does nothing if already running.

#### void pause()
  - タイマーを一時停止します。 Pauses the timer.

#### void resume()
  - タイマーを再開します。 Resumes the timer.

#### void cancel()
  - タイマーをキャンセルします。 Cancels the timer.

#### void update()
  - イベントの状態を更新します。 Updates the state of the event.
  - タイマーの精度を高めるためには、このメソッドをできるだけ早い周期で呼び出す必要があります。 To improve the accuracy of the timer, this method needs to be called at the earliest possible cycle.
  - このメソッドが呼び出される箇所が一箇所である必要はありません。 It is not necessary for this method to be called at a single location.


## 使用例 Usage Example
```
#include <OneShot.h>

OneShot oneShot(OneShot::Resolution::MILLIS); //Generate a OneShot object (you can omit the argument if the resolution is in milliseconds).

const uint8_t LED = 13; //LED pin
const uint8_t SW1 = 2;  //Pin to connect the switch

bool ledState = false;  //State of the LED (on/off)

//Callback function executed when time elapses
void ledToggle() {
  digitalWrite(LED, ledState);
  ledState = !ledState; //Toggle the LED state
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);

  //Register the callback function to be executed after 500ms
  oneShot.registerCallback(ledToggle, 500);
}

void loop() {
  if (digitalRead(SW1) == LOW) {
    //Start the timer (it will be automatically reset after time elapses)
    oneShot.start();
  }

  //Update the timer (call it at the fastest possible rate, it doesn't have to be in one place)
  oneShot.update();
}

```
