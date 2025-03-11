#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Biblioteca DHT
#include <DHT.h>

//Definindo entradas do DHT11
#define DHTPIN A3 
#define DHTTYPE DHT11 


//Configuração do LCD 
#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende, col, lin);

//Sensores
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

boolean fahrenheit = false;

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
  lcd.setCursor(1,0);
  lcd.print("Luminosidade Mínima");
  digitalWrite(red, HIGH);
  delay(500);
  min_luz = analogRead(ldr);
  digitalWrite(red, LOW);
  lcd.clear();

  delay(100);
  lcd.setCursor(1,0);
  lcd.print("Luminosidade Máxima");
  digitalWrite(red, HIGH);
  delay(500);
  max_luz = analogRead(ldr);
  delay(100);
  digitalWrite(red, LOW);
  lcd.clear();

}


void loop() {
  

}

void menu(){
    
    //Criando as variáveis de 
    float h = dht11.readHumidity();
    float t = dht11.readTemperature();

    if (isnan(t) || isnan(h)) 
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Failed to read from DHT");
  } 
  else 
  {
   //Condiçao de temperatura
    if(t < 15 || t > 25){
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Temperatura crítica");
    delay(400);
    lcd.clear();
  }

    //Condição de Umidade
    if(h < 30 || h > 50){
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Temperatura crítica");
    delay(400);
    lcd.clear();
  }
  
  //Condição de luminosidade
  int lumin = analogRead(ldr);
  if(lumin < luz_min || lumin > (luz_max*0.3)){
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Temperatura crítica");
    delay(400);
    lcd.clear();
  }


  }
 }


int TransformaFahrenheit (int t){
  int temperatura = (t*1.8) + 32;
  return temperatura;
  }

void alerta(){
  }

