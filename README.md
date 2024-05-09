This library uses English that has been machine-translated from Japanese.

** 警告：v0.4.0より破壊的変更が加えられました。時間計測関数の登録方法が変更され、Resolution列挙型が削除されました。 **
Warning: Destructive changes have been made since v0.4.0. The method of registering timing measurement functions has been altered, and the Resolution enumeration type has been removed.

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

#### OneShot(TimeFunc timeFunc)
  - OneShotオブジェクトを生成します。 Generates a OneShot object.
    - timeFunc
      - タイマーの分解能を設定します。 Sets the resolution of the timer.
      - ミリ秒単位の場合はmillis、マイクロ秒単位の場合はmicrosを使います。 For millisecond units, use "millis"; for microsecond units, use "micros".
      - あなた独自の計測用関数を使用することもできます。 You can also use your own custom measurement functions.
      - デフォルトではmillisです。 Default is "millis".
     
  - 例:`OneShot oneShot(millis);`

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

#### OneShot::TimeFunc getTimeFunc()
  - タイマーの時間測定に使われている関数ポインタを返します。 This function returns a function pointer used for timing measurements.
   
#### uint32_t now()
  - 現在の時間を返します。 This function returns the current time.
    - 時間の分解能はタイマーの分解能に依存します。 The resolution of this function depends on the timer's resolution.

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

#### void start(uint32_t interval)
  - インターバルを指定すると同時にタイマーを開始します。 Start the timer simultaneously with specifying the interval.

#### void pause()
  - タイマーを一時停止します。 Pauses the timer.

#### void resume()
  - タイマーを再開します。 Resumes the timer.

#### void cancel()
  - タイマーをキャンセルします。 Cancels the timer.

#### bool update()
  - イベントの状態を更新します。 Updates the state of the event.
  - タイマーの精度を高めるためには、このメソッドをできるだけ早い周期で呼び出す必要があります。 To improve the accuracy of the timer, this method needs to be called at the earliest possible cycle.
  - このメソッドが呼び出される箇所が一箇所である必要はありません。 It is not necessary for this method to be called at a single location.
  - イベントが発生したらtrue、しなければfalseを返します。(hasOccurred()メソッドと同じです。) When the event occurs, it returns true; otherwise, it returns false (same as the hasOccurred() method).


## 使用例 Usage Example
```
#include <OneShot.h>

OneShot oneShot(millis); //Generate a OneShot object (you can omit the argument if the resolution is in milliseconds).

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
