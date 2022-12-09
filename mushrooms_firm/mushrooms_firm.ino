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

/*byte temprcvByte[5];
int tmpSensor = 2;
int analogPin = A0;
int analogVal = 0;
*/

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
   /*pinMode(tmpSensor, OUTPUT);
   digitalWrite(tmpSensor, HIGH);*/
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
  /*
   dht(tmpSensor);
   Serial.print("Humdity 1 = ");
  Serial.print(temprcvByte[0], DEC);
  Serial.print(".");
  Serial.print(temprcvByte[1], DEC);
  Serial.println("%");
  Serial.print("Temperature 1 = ");
  Serial.print(temprcvByte[2]-18, DEC);
  Serial.print(".");
  Serial.print(temprcvByte[3], DEC);
  Serial.println(" degC");
  delay(100);

  Serial.println(temprcvByte[0]+temprcvByte[1]+temprcvByte[2]+temprcvByte[3]);
  Serial.println(temprcvByte[4]);
  */

  if(t==0){
    t = millis();
  }

  // Read sensors
  retourTmp = TH06.ReadTemperature();
  retourHygro = TH06.ReadHumidity();

  Serial.println(retourTmp);

  lcd.clear();
  /*
  float rgb[3] = {0,0,0};
  hsv2rgb(ct, 1.0, 255, rgb);
  lcd.setRGB((int) rgb[0], (int) rgb[1], (int) rgb[2]);
  ct += 0.01;
  */
  lcd.setCursor(0, 0);
  lcd.print(retourTmp);             // ou lcd.print(nom de la variable à afficher);

  /*
  ShowSerial.print("Temperature: ");
  ShowSerial.print(retourTmp);
  ShowSerial.println("C");
  //Serial.print("%\t");

  ShowSerial.print("Humidity: ");
  ShowSerial.print(retourHygro);
  ShowSerial.println("%");
  */
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

  /*
  ShowSerial.print("CommandTemperature: ");
  ShowSerial.println(commandTmp);
  ShowSerial.print("CommandHygro: ");
  ShowSerial.println(commandHygro);
  */

  delay(1000);
}

float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float b, float rgb[3]) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}

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
  // C'mon, do smthing
}

/*
byte getTmpByteValue(int pin) {
  byte i = 0;
  byte value = 0;
  for (i = 0; i < 8; i++) {
    while (digitalRead(pin) == LOW); // wait for pin state to be HIGH
    delayMicroseconds(35);          // wait 30 microSeconds
    if (digitalRead(pin) == HIGH)
      value |= (1 << (8 - i));      // save bit value if pin 10 is HIGH
    while (digitalRead(pin) == HIGH); // wait for pin state to be LOW
  }
  return value;
}

void dht(int pin) {
  digitalWrite(pin, LOW);  // set pin LOW to start communication with module
  delay(30);              // wait 30 milliSeconds
  digitalWrite(pin, HIGH); // set pin HIGH
  delayMicroseconds(20);  // wait 20 microSeconds
  pinMode(pin, INPUT);     // change pin 10 mode to input
  while (digitalRead(pin) == HIGH);    // wait for pin to be LOW
  delayMicroseconds(50);  // wait for 80 microSeconds
  while (digitalRead(pin) == LOW);    // wait for pin to be HIGH
  delayMicroseconds(50); // wait for 80 microSeconds
  while (digitalRead(pin) == HIGH);    // wait for pin to be LOW
  for (int i = 0; i < 5; i++) // receive temperature and humidity values
    temprcvByte[i] = getTmpByteValue(pin);
  pinMode(pin, OUTPUT);    // change pin 10 mode to ouptut
  digitalWrite(pin, HIGH); // set pin 10 to HIGH
}
*/
