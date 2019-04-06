#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int buzzerPin = 10;
int wire1 = 0;
int wire2 = 1;
int wire3 = 2;
int wire4 = 3;
const byte ledPin = 13;    //using the built in LED

/*  Application variables:  */
int maxSec = 60*40; // Time to defuse the bomb
bool EXPLODED = false;
bool DEFUSED = false;
unsigned long period = 10000;  //Amount of milliseconds between the buzzers

/*  TIMERS  */
unsigned long startMillis;  //global start time of the program
unsigned long startMillisForBuzzer;  //Start timer for buzzer
bool showResult= true;
int totalSec = 0; // Ellapsed time since start
unsigned long currentMillis;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  lcd.init();
  lcd.backlight();
  startMillisForBuzzer = millis();
  startMillis = millis();
  pinMode(wire1,INPUT_PULLUP);
  pinMode(wire2,INPUT_PULLUP);
  pinMode(wire3,INPUT_PULLUP);
  pinMode(wire4,INPUT_PULLUP);
}

void loop() {
  if(EXPLODED || DEFUSED){
    if(EXPLODED){
      checkBuzzer(1000);
    }
    return;
  }
  timer(); //Continue timer
  checkWires();
}

void checkBuzzer(int duration){
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillisForBuzzer >= period)  //check whether the period has elapsed
  {
    //tone(buzzerPin, 800, duration);  //if so, buzzer to the player
    startMillisForBuzzer = currentMillis;  //save the current time to the starttime for the buzzer
  }
}


void timer(){
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  int ellapsedSeconds = (currentMillis - startMillis) / 1000; //get total seconds since start
  
  if(ellapsedSeconds > maxSec){
    explode();
  }
  else{
    if(ellapsedSeconds > totalSec){
      totalSec = ellapsedSeconds;
      writeLine(0,4,getCountdown(totalSec));
    }
  }
}

String getCountdown(int ellapsedSeconds){
  int secToGo = maxSec - ellapsedSeconds;
  int timerSec = secToGo % 60;
  int timerMin = secToGo / 60;
  int timerHour = secToGo / (60*60);

  String sTimerMin = String(timerMin);
  if(timerMin < 10){
    sTimerMin = String("0" + sTimerMin);
  }
  String sTimerSec = String(timerSec);
  if(timerSec < 10){
    sTimerSec = String("0" + sTimerSec);
  }
  String sTimerHour = String(timerHour);
  if(timerHour < 10){
    sTimerHour = String("0" + sTimerHour);
  }

  return String(sTimerHour + ":" + sTimerMin + ":" + sTimerSec);
}

void explode(){
  EXPLODED = true;
  writeCurrentTime();
  writeLine(1,1,"BOMB EXPLODED");
  period = 2000;
}
void defuse(){
  DEFUSED = true;
  writeCurrentTime();
  writeLine(1,2, "BOMB DEFUSED");
}

void writeCurrentTime(){
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  int total = (currentMillis - startMillis) / 1000;
  writeLine(0,4,getCountdown(totalSec));
}

void writeLine(int line, int column, String text){
  clearLine(line);
  lcd.setCursor(column, line);
  lcd.print(text);
}

void clearLine(int line){
  lcd.setCursor(0,line);
  lcd.print("                ");
}

void checkWires(){
  int pin1 = digitalRead(wire1);
  int pin2 = digitalRead(wire2);
  int pin3 = digitalRead(wire3);
  int pin4 = digitalRead(wire4);

  if(pin1 == HIGH || pin2 == HIGH || pin3 == HIGH){
    explode();
  }
  else{
    if(pin4 == HIGH){
      defuse();
    }
  }
}
