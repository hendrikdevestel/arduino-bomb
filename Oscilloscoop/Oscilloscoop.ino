int yellow1 = 4;
int yellow2 = 5;
int yellow3 = 6;
int yellow4 = 7;
int powerLed = 10;
int powerSwitch = 13;
bool delayed = false;
int delayTime = 500;
unsigned long startMillis;  //start of switched off
unsigned long currentMillis;

void setup() {
  Serial.begin(9600);
  pinMode(yellow1, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(yellow3, OUTPUT);
  pinMode(yellow4, OUTPUT);
  pinMode(powerLed, OUTPUT);
  pinMode(powerSwitch, INPUT);
  
}

void loop() {
    if(digitalRead(powerSwitch) == LOW){
      turnOff();
    }
    else{
      turnOn();
    }
}

void turnOn(){
  digitalWrite(powerLed, HIGH);
  if(!delayed){
    delay(delayTime);
    delayed = true;
  }
  digitalWrite(yellow1, HIGH);
  digitalWrite(yellow2, HIGH);
  digitalWrite(yellow3, HIGH);
  digitalWrite(yellow4, HIGH);
}
void turnOff(){
  if(startMillis == NULL){
    startMillis = millis();
  }
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= 500)  //check whether the power is off for at least 0.5 seconds
  {
    turnLightsOff();
  }
}
void turnLightsOff(){
  digitalWrite(powerLed, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(yellow2, LOW);
  digitalWrite(yellow3, LOW);
  digitalWrite(yellow4, LOW);
  startMillis = NULL;
}
