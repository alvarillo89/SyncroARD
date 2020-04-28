byte flag;
byte startUpPulse;
const byte interruptPin = 2;

void setup() {
  flag = 0;
  startUpPulse = 0;
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Task, RISING);
}

void loop() {
  if(flag == 1){
    Serial.println("TASK");
    flag = 0;
  }
}

void Task(){
  if (startUpPulse) {
    flag = 1;
  } else {
    startUpPulse = 1;
  }
}
