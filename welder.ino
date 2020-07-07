/*
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 12, 11, 10, 8); //CLK, DIR, DC, CE, RST
*/
int waitTime = 0; //raw
int feedSpeed = 0;

int convwaitTime = 0; //converted
int convfeedSpeed = 0;



void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); //trafo
  pinMode(3, OUTPUT); //plyn
  pinMode(4, OUTPUT); //stator
  pinMode(5, OUTPUT); //rotor
  pinMode(9, OUTPUT); //rotor pwm
  pinMode(6, INPUT_PULLUP); //trigger
  pinMode(7, INPUT_PULLUP); //trafo feedback
  pinMode(A0, INPUT); //rotor pwm
  pinMode(A1, INPUT); //fire delay
/*
  display.begin();
  display.setContrast(60);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
 */
}

void loop() {
  convwaitTime = map(waitTime, 0, 670, 500, 2000); //convert raw values to in system values
  convfeedSpeed = map(feedSpeed, 0, 670, 0, 255);
  
  waitTime = analogRead(A0);
  feedSpeed = analogRead(A1);
  /*
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("standby");
  display.setCursor(0,9);
  display.print("delay: " + String(convwaitTime) + "ms");
  display.setCursor(0,18);
  display.print("feedspeed: " + String(convfeedSpeed) + "%");
  display.display(); 
  */
  digitalWrite(2, LOW); //reset all to low in standby state
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(9, LOW);

  //wait for button press
  if (digitalRead(6) == 0) {
    /*
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Stage-1 INIT");
    display.setCursor(0,9);
    display.print("delay: " + String(convwaitTime) + "ms");
    display.setCursor(0,18);
    display.print("feedspeed: " + String(convfeedSpeed) + "%");
    display.display(); 
    */
    digitalWrite(3, HIGH); //start plyn
    delay(convwaitTime); //adjustable delay A1
    digitalWrite(2, HIGH); //start trafo
    delay(500); //tweak later
    //wait for feedback D7
    if (digitalRead(7) == 0) {
      //check if D6 still pressed, stay looping until released
      while(digitalRead(6)== 0) {
      /*
       display.clearDisplay();
       display.setCursor(0,0);
       display.print("Normal");
       display.setCursor(0,9);
       display.print("delay: " + String(convwaitTime) + "ms");
       display.setCursor(0,18);
       display.print("feedspeed: " + String(convfeedSpeed) + "%");
       display.display(); 
     */
       digitalWrite(4, HIGH); //start stator
       digitalWrite(5, HIGH); //start rotor
       analogWrite(9, convfeedSpeed); //PWM for rotor control
      }
    }
      //check if feedback fails
     else if (digitalRead(7) == 1) {
       /*
       display.clearDisplay();
       display.setCursor(0,0);
       display.print("ERROR");
       display.setCursor(0,9);
       display.print("delay: " + String(convwaitTime) + "ms");
       display.setCursor(0,18);
       display.print("feedspeed: " + String(convfeedSpeed) + "%");
       display.display(); 
        */
       //safestate
       digitalWrite(2, LOW); 
       digitalWrite(3, LOW);
       digitalWrite(4, LOW);
       digitalWrite(5, LOW);
       digitalWrite(9, LOW);
       delay(500);
     }
    
  }
}
