#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int buzzerPin = 10;
const byte ledPin = 13;    //using the built in LED
/* KEYPAD: */
const byte numRows= 4;
const byte numCols= 4;
char keymap[numRows][numCols]= { {'1', '2', '3', 'A'},{'4', '5', '6', 'B'},{'7', '8', '9', 'C'},{'*', '0', '#', 'D'} };
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

/*  Application variables:  */
String code = "2305"; //The code to defuse the bomb
int maxSec = 10; // Time to defuse the bomb
String inseredCode = "";
bool EXPLODED = false;
bool DEFUSED = false;
unsigned long period = 10000;  //Amount of milliseconds between the buzzers
bool canEnterCode = false; // Whether the code can be entered (after pressing *)
int keyIndex = 12; //Where the code will be insered on the display

/*  TIMERS  */
unsigned long startMillis;  //global start time of the program
unsigned long startMillisForBuzzer;  //Start timer for buzzer
unsigned long startMillisForBlinking;  //Start timer for buzzer
bool showResult= true;
int totalSec = 0; // Ellapsed time since start
unsigned long currentMillis;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
  lcd.init();
  lcd.backlight();
  startMillisForBuzzer = millis();
  startMillis = millis();
}

void loop() {
  if(EXPLODED || DEFUSED){
    if(EXPLODED){
      checkBuzzer(1000);
    }
    return;
  }
  timer(); //Continue timer
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {
    if(canEnterCode){ // If the code can be entered
      inseredCode = String(inseredCode + keypressed);
      if(inseredCode == code){
        defuse();
      }
      else{
        if(inseredCode.length() <= 3){
          lcd.setCursor(keyIndex,1);
          lcd.print(keypressed);
          keyIndex++;
        }
        else{
          explode();
        }
      }
    }
    else{
      if(keypressed == '*'){
        tone(buzzerPin, 500, 100);
        inseredCode = "";
        writeLine(1,0, "Enter code: ____");
        canEnterCode = true;
      }
    }
  }
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

void checkBlinking(){
  int blinkPeriod = 1000;
  currentMillis = millis();
  if(currentMillis - startMillisForBlinking >= blinkPeriod){
    if(showResult){
      writeLine(1,1, "BOMB EXPLODED");
      showResult = false;
    }
    else{
      clearLine(1);
      showResult = true;
    }
    startMillisForBlinking = currentMillis;
  }
}
