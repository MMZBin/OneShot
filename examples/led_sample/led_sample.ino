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
