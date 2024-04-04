#include <OneShot.h>

                                              //OneShotオブジェクトを生成(引数は省略でき、省略した場合はミリ秒単位になります。)
OneShot oneShot(OneShot::Resolution::MILLIS); //Generate a OneShot object (you can omit the argument if the resolution is in milliseconds).

const uint8_t LED = 13; //LED pin LEDピン
const uint8_t SW1 = 2;  //Pin to connect the switch //スイッチ

                        //LEDの状態を表す変数
bool ledState = false;  //State of the LED (on/off)

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);

  //間隔を500ミリ秒に設定する
  //Set the interval to 500ms
  oneShot.setInterval(500);
}

void loop() {
  if (digitalRead(SW1) == LOW) {
    //タイマーを開始する(時間が経過してイベントが発生したあとは自動で再利用できるようになります。)
    //Start the timer (it will be automatically reset after time elapses)
    oneShot.start();
  }

  //時間が経過したと判定されてから次にupdate()メソッドが呼び出されるまでtrueになります。
  //From the time it is determined that time has elapsed until the next execution of the update() method, it returns true
  if (oneShot.hasOccurred()) {
    digitalWrite(LED, ledState);
    ledState = !ledState; //Toggle the LED state LEDの状態を切り替える
  }

  //タイマーの状態をアップデートする(できるだけ早い周期で呼び出すことをおすすめします。)
  //Update the timer (call it at the fastest possible rate, it doesn't have to be in one place)
  oneShot.update();
}
