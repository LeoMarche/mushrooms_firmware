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
   expectedTmp = 30;
   expectedHygro = 30.0;
   coeffPropTmp = 1.0;
   coeffIntTmp = 0.0003;
   coeffPropHygro = 1.0;
   lcd.begin(16, 2);
   lcd.setRGB(0,255,0);
}

void loop() {

  if(t==0){
    t = millis();
  }

  // Read sensors
  retourTmp = TH06.ReadTemperature();
  retourHygro = TH06.ReadHumidity();

  Serial.println(retourTmp);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(retourTmp);             // ou lcd.print(nom de la variable à afficher);

  float currt = millis();

  // Get dt
  dt = currt-t;
  t = currt;

  // Execute ORDER 66
  commandTmp = pid(coeffPropTmp, coeffIntTmp, expectedTmp - retourTmp, &integraleTmpError, dt/1000);
  commandHygro = pid(coeffPropHygro, coeffIntHygro, expectedHygro - retourHygro, &integraleHygroError, dt/1000);

  commandTmp = limit(commandTmp);
  commandHygro = limit(commandHygro);

  setTmpCommand();
  setHygroCommand();
}

/*
float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float b, float rgb[3]) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}
*/

float pid(float coeffProp, float coeffInt, float error, float *integraleError, float dt) {
  (*integraleError) += error * dt;
  Serial.print("Integral error :");
  Serial.println(*integraleError);
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
  Serial.println((int) (commandTmp / 5 * 255));
  analogWrite(PWM_tmp, (int) (commandTmp / 5 * 255));
}

void setHygroCommand() {
  int timeUp = commandHygro * 255 / 80;
  digitalWrite(PWM_hygro, HIGH);
  delay(timeUp*100);
  digitalWrite(PWM_hygro, LOW);
  delay((16-timeUp)*100);
}
