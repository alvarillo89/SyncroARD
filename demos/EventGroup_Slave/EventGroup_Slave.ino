#include <Wire.h>
#include <Arduino_FreeRTOS.h>

int shouldStart;
int tickStart;
int tickEnd;
int lastDeltaTick;

// FreeRTOS tasks:
void BlinkLed( void *pvParameters );


void setup() {
  shouldStart = 0;
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  xTaskCreate(BlinkLed, "BlinkLed", 128, NULL, 1, NULL);
}


void loop() { delay(10000); }


void receiveEvent(int howMany) {
  int x = Wire.read();
  shouldStart = x;
}


void requestEvent(){
  Wire.beginTransmission(5);
  Wire.write(1);
  Wire.endTransmission();           
}


void BlinkLed( void *pvParameters ) {
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);

  while(!shouldStart) { delay(1); }
  
  for(;;) {
    tickStart = xTaskGetTickCount();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    tickEnd = xTaskGetTickCount();
    lastDeltaTick = tickEnd - tickStart;
  }
}
