#include <OneShot.h>

OneShot oneShot(OneShot::Resolution::MILLIS); //Generate a OneShot object (you can omit the argument if the resolution is in milliseconds).

const uint8_t LED = 13; //LED pin
const uint8_t SW1 = 2;  //Pin to connect the switch

bool ledState = false;  //State of the LED (on/off)

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);

  //Set the interval to 500ms
  oneShot.setInterval(500);
  
}

void loop() {
  if (digitalRead(SW1) == LOW) {
    //Start the timer (it will be automatically reset after time elapses)
    oneShot.start();
  }
  
  //From the time it is determined that time has elapsed until the next execution of the update() method, it returns true
  if (oneShot.hasOccurred()) {
    digitalWrite(LED, ledState);
    ledState = !ledState; //Toggle the LED state
  }

  //Update the timer (call it at the fastest possible rate, it doesn't have to be in one place)
  oneShot.update();
}
