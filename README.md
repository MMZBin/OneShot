This library uses English that has been machine-translated from Japanese.

# OneShot

一回限りのワンショットイベントを生成するライブラリです。

This is a library for generating one-shot events, which occur only once.

## 機能 Functions

#### enum class State
- タイマーの状態を表します。
  - STOPPED
  - RUNNING
  - PAUSED

#### enum class Resolution
- タイマーの分解能を設定します。
  - MILLIS
  - MICROS

#### OneShot(OneShot::Resolution res)
  - OneShotオブジェクトを生成します。
    - res
      - タイマーの分解能を設定します。
      - デフォルトではMILLISです。

#### void registerCallback(CallbackFunc func)
  - タイマーイベントが発生した際に呼び出されるコールバック関数を登録します。
    - func
      - 登録するコールバック関数

#### void registerCallback(CallbackFunc func, uint32_t interval)
  - コールバック関数とインターバルを一度に登録します。
    - func
      - 登録するコールバック関数
    -interval
      - イベントが発生するまでの間隔

#### OneShot::State getState()
  - 現在の状態をState列挙型で返します。

#### bool isOccurred()
  - イベントが発生したと判定されてから次にupdate()メソッドが実行されるまでtrueになります。

#### uint32_t getInterval()
  - 現在のイベントが発生するまでの間隔を返します。

#### void setInterval(uint32_t interval)
  - イベントが発生するまでの時間を設定します。
  - タイマーが動作している間は無効になります。
    - interval
      - イベントが発生するまでの時間(ミリ秒もしくはマイクロ秒)

#### void start()
  - タイマーを開始します。
  - すでに動作している間に実行された場合、なにもしません。

#### void pause()
  - タイマーを一時停止します。

#### void resume()
  - タイマーを再開します。

#### void cancel()
  - タイマーをキャンセルします。

#### void update()
  - イベントの状態を更新します。
  - タイマーの精度を高めるためには、このメソッドをできるだけ早い周期で呼び出す必要があります。
  - このメソッドが呼び出される箇所が一箇所である必要はありません。


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
