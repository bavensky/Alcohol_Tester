#include <LiquidCrystal.h>

LiquidCrystal lcd(15, 14, 4, 5, 6, 7);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte beer[8] = {
  0b11111,
  0b10001,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};

int analogPin = A0;
int led_status  = 2;

int val = 0;
int warm = 0;
unsigned long pre = 0;
float percentage = 0.0;
float bac = 0.0;

void setup()   {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.createChar(1, heart);
  lcd.createChar(2, beer);
  lcd.clear();

  pinMode(led_status, OUTPUT);
  
  lcd.home();
  lcd.print(" Ready to drink ");
  delay(3000);
  printWarming();
  lcd.clear();
}


void loop() {
  delay(100);
  bac = readAlcohol();
  printTitle();
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.write(2);
  lcd.print(" = ");
  lcd.print(bac);
  lcd.print(" mg%   ");

  if(bac >= 50.00)  {
    digitalWrite(led_status, 1);
  }
  else  {
    digitalWrite(led_status, 0);
  }
}

void printTitle()
{
  lcd.home();
  lcd.print("Alcohol Tester !");
}

void printWarming()
{
  while (warm <= 15) {
    lcd.setCursor(0, 0);
    lcd.print("   Warming up!  ");
    lcd.setCursor(warm, 1);
    lcd.write(1);

    unsigned long nowtime = millis();
    if (nowtime - pre >= 1000)  {
      warm += 1;
      pre = nowtime;
    }
  }
}

int readAlcohol()
{
//Alcohol Sensor.
// The MQ3 sensor produces an analog output signal between 0 and 1023.
// BAC is expressed as the weight of ethanol, measured in grams per 210 liters of breath.
// The graphs in the PDFs give breath alcohol in terms of mg/L.
// To convert mg/L to BAC, multiply by 210 and divide by 1000, or in other words, multiply by 0.21.
//
// For example, if the analog output from our MQ3 is reading 400, we divide 400 by 1023 (the highest analog value) in order to get the ratio or percentage of alcohol on the breath.
// Then we determine that 0.4 or 40% alcohol on the breath will yield 0.4 * 0.21 = 0.084, which is slightly more than the legal alimit in in most states of 0.08.
//
// @author: Aidan Melen
// @date: 06/08/2015

  int val1;
  val1 = analogRead(analogPin); 
  percentage = val1 / 1023.0;
  bac = percentage * 0.21;
  bac = bac*1000;
  return bac;
}


