
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <UltrasonicSRF04.h>
//#include "UltrasonicSRF04.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BUZZER_PIN 24

const int BATTERY_PIN =  A0;
const float R1 = 4700.0;
const float R2 = 10000.0;
const int MODE_BUTTONS[] = {12, 13, 14, 15, 16};
const int MODE_NUMBERS = 5;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
UltrasonicSRF04 sensor(10, 9);


void setOled(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED LCD Not Found");
    while(true);}

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("HELLO, OLED LCD Found");
    display.display();
    
}

float batteryStatus(){
  int adc_val = analogRead(BATTERY_PIN);
  float vout = adc_val * 5.0 / 1023;
  float vout_bat = vout * (R1 + R2)/R2;
  return vout_bat;
}

void showBatteryStatus(){
 float battery = batteryStatus();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Battery: ");
    display.print(battery);
    display.println(" V");
    display.display();
 
}

void showDistance(float distance, String unit) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.print(distance);
  display.print(" ");
  display.println(unit);
  display.display();
}

void setBuzzer(){
 pinMode(BUZZER_PIN, OUTPUT);
 digitalWrite(BUZZER_PIN, LOW);
}

void alarm(bool cond){
  if (cond){
    digitalWrite(BUZZER_PIN, HIGH);}else{
      digitalWrite(BUZZER_PIN, LOW);}  
}

void setButtons(){
  for(int i = 0; i< MODE_NUMBERS; i++){
    pinMode(MODE_BUTTONS[i], INPUT_PULLUP);
    }
}

bool buttonStatus(int ind){
  return digitalRead(MODE_BUTTONS[ind]);
}
int mode = 0;
void selectMode(){
  for (int i=0; i<5; i++){
    if (!buttonStatus(i)){
      mode = i;
      break;
      }
    }
}
void showCurrentMode(){
   switch(mode){
    case 0:
      showBatteryStatus();
      break;
    case 1:
      showDistance(sensor.getDistance(), "cm");
      break;
    case 2:
      showDistance(sensor.getDistance("inch"), "inch");
      break;
    case 3:
      showDistance(sensor.getFilteredDistance(), "cm");
      break;
    case 4:
      showBatteryStatus();
      delay(1000);
      showDistance(sensor.getDistance(),"cm");
    }
}

void setup() {
  Serial.begin(9600);
  setOled();
  setBuzzer();
  setButtons();
}

void loop() {
  selectMode();
  showCurrentMode();
  bool isInBound = sensor.inBound(30.0);
  alarm(isInBound);
  delay(500);
}
