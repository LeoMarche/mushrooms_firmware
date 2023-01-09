#include <TH06_dev.h>
#include "Arduino.h"
#include "Wire.h"
#include "THSensor_base.h"
#include "rgb_lcd.h"

#define ShowSerial Serial

#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define COMSerial Serial
    #define ShowSerial Serial
    TH06_dev TH06;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define COMSerial Serial1
#define ShowSerial SerialUSB
TH06_dev TH06;
#endif

#ifdef ARDUINO_ARCH_STM32F4
#define COMSerial Serial
#define ShowSerial SerialUSB
TH06_dev TH06;
#endif

int PWM_tmp = 10;
int PWM_hygro = 11;
float integraleTmpError, integraleHygroError;
float retourTmp, retourHygro;
float commandTmp, commandHygro;
float coeffPropTmp, coeffPropHygro, coeffIntTmp, coeffIntHygro;
float expectedTmp, expectedHygro;
float t, dt, currt;
rgb_lcd lcd;
float ct = 0;

void setup() {
   pinMode(PWM_tmp, OUTPUT);
   pinMode(PWM_hygro, OUTPUT);
   TH06.begin();
   Serial.begin(9600);
   t = 0;
   expectedTmp = 24;
   expectedHygro = 95.0;
   coeffPropTmp = 1.0;
   coeffIntTmp = 0.0003;
   coeffPropHygro = 0.1;
   coeffIntHygro = 0.003;
   lcd.begin(16, 2);
   lcd.setRGB(0,255,0);
}

void loop() {

  if(t==0){
    t = millis();
  }

  int nbAvailable = Serial.available();

  // You can specify indications to the arduino using op number
  // through serial communication
  while(nbAvailable > 0) {
    int op = Serial.parseInt();
    if(op==1){
      expectedTmp = Serial.parseFloat();
      Serial.println(expectedTmp);
    } else if(op==2) {
      expectedHygro = Serial.parseFloat();
      Serial.println(expectedHygro);
    } else if (op==3) {
      Serial.println(expectedTmp);
    } else if (op==4) {
      Serial.println(expectedHygro);
    } else if (op==5) {
      Serial.println(retourTmp);
    } else if (op==6) {
      Serial.println(retourHygro);
    }
    nbAvailable = Serial.available();
  }
  
  // Read sensors
  retourTmp = TH06.ReadTemperature();
  retourHygro = TH06.ReadHumidity();

  // Print command anc actual to LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(retourTmp);
  lcd.print("   ");
  lcd.print(retourHygro);
  float currt = millis();

  // Get dt
  dt = currt-t;
  t = currt;

  // Get command using PI corrector
  commandTmp = pid(coeffPropTmp, coeffIntTmp, expectedTmp - retourTmp, &integraleTmpError, dt/1000);
  commandHygro = pid(coeffPropHygro, coeffIntHygro, expectedHygro - retourHygro, &integraleHygroError, dt/1000);

  commandTmp = limit(commandTmp);
  commandHygro = limit(commandHygro);

  // Apply command
  setTmpCommand();
  setHygroCommand();
}

float pid(float coeffProp, float coeffInt, float error, float *integraleError, float dt) {
  (*integraleError) += error * dt;
  return coeffProp * error + coeffInt * (*integraleError);
}

float limit(float commandValue){
  if(commandValue < 0){
    commandValue = 0;
  } else if(commandValue > 5){
    commandValue = 5;
  }
  return commandValue;
}

void setTmpCommand() {
  lcd.setCursor(0, 1);
  lcd.print((int) (commandTmp / 5 * 255));
  analogWrite(PWM_tmp, (int) (commandTmp / 5 * 255));
}

void setHygroCommand() {
  int timeUp = commandHygro * 255 / 80;
  lcd.setCursor(10, 1);
  lcd.print(timeUp);
  digitalWrite(PWM_hygro, HIGH);
  delay(timeUp*300);
  digitalWrite(PWM_hygro, LOW);
  delay((16-timeUp)*300);
}
