#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Configuração do LCD 
#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende, col, lin);

//Sensores
const int dht11 = A3;
const int ldr = A2;
const int rtc = A1;

//LED RGB
const int red = 10;
const int green = 11;
const int blue = 12;

//Extra
const int btn1 = 2;
const int btn2 = 3;
const int buzzer = A0;

//Variáveis de uso
int min_luz = 0;
int max_luz = 500;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(dht11, INPUT);
  pinMode(ldr, INPUT);
  pinMode(rtc, INPUT);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

  pinMode(buzzer, OUTPUT);

}



void ajustarLuminosidade(){
  Serial.println("Luminosidade Mínima");
  min_luz = analogRead(ldr);
  digitalWrite(red, HIGH);
  delay(100);
  digitalWrite(red, LOW);

  Serial.println("Luminosidade Máxima");
  max_luz = analogRead(ldr);
  digitalWrite(red, HIGH);
  delay(100);
  digitalWrite(red, LOW);

}


void loop() {
  

}
