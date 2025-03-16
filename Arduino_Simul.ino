#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Biblioteca DHT
#include <DHT.h>

// Definindo entradas do DHT22
#define DHTPIN A3 
#define DHTTYPE DHT22 

// Criando o objeto DHT
DHT dht(DHTPIN, DHTTYPE);

// Configuração do LCD 
#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende, col, lin);

// Sensores
const int ldr = A2;
const int rtc = A1;

// LED RGB
const int blue = 10;
const int green = 11;
const int red = 12;

// Extra
const int btn1 = 2;
const int btn2 = 3;
const int buzzer = A0;

// Variáveis de uso
int min_luz = 0;
int max_luz = 500;

boolean fahrenheit = false;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.clear();

  dht.begin(); // Inicializa o sensor DHT22

  pinMode(ldr, INPUT);
  pinMode(rtc, INPUT);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

  pinMode(buzzer, OUTPUT);

}

void menu(){
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int lumin = 0;

    if (isnan(t) || isnan(h)) {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Failed to read DHT");
      return;
    } 

    // Condição de temperatura
    if(t < 15 || t > 25){
      lcd.clear();
      alerta();
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Temp. crítica");
      delay(400);
      lcd.clear();
    }

    // Condição de Umidade
    if(h < 30 || h > 50){
      lcd.clear();
      alerta();
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Umidade crítica");
      delay(400);
      lcd.clear();
    }
  
    // Condição de luminosidade
    lumin = analogRead(ldr);
    if(lumin < min_luz || lumin > (max_luz*0.3)){
      lcd.clear();
      alerta();
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Luminosidade Critica");
      delay(400);
      lcd.clear();
    }

    delay(2000);

    lcd.print("Umi: " + String(h));
    delay(2000);

    lcd.clear();

    if(fahrenheit){
      lcd.print("Temp: " + String(TransformaFahrenheit(t)) + "F");
    } else {
      lcd.print("Temp: " + String(t) + "C");
    }
    
    delay(2000);

    lcd.clear();

    lcd.print("Lum: " + String(lumin));
    delay(2000);

    lcd.clear();
 }

int TransformaFahrenheit (int t){
  int temperatura = (t * 1.8) + 32;
  return temperatura;
}

void BateCoracao(){
  analogWrite(red, 255);
  tone(buzzer, 150, 100); 
  delay(120);             
  tone(buzzer, 150, 200); 
  delay(800);
  analogWrite(red, 0);
}

void alerta(){
  // Aqui pode colocar alerta com buzzer ou piscar LEDs
}

void ajustarLuminosidade(){
  lcd.setCursor(1,0);
  lcd.print("Lumin Minima");

  delay(100);
  analogWrite(blue, 255);

  //Para indicar que está aguardando o apertar do botão
  while(digitalRead(btn1) == LOW){
    
  }
  
  analogWrite(blue, 0);

  BateCoracao();

  min_luz = analogRead(ldr);
  analogWrite(red, 0);
  lcd.clear();
  lcd.print(min_luz);
  delay(2000);

  lcd.clear();

  delay(100);
  lcd.setCursor(1,0);
  lcd.print("Lumin Maxima");

  analogWrite(blue, 255);
  while(digitalRead(btn1) == LOW){
    
  }
  analogWrite(blue, 0);

  BateCoracao();
  
  max_luz = analogRead(ldr);
  delay(100);
  lcd.clear();
  lcd.print(0.3*max_luz);
  delay(2000);
  lcd.clear();

}

void loop() {
  menu();

  lcd.clear();
  lcd.print("Ajustar Lum?");
  
  analogWrite(blue, 255);
  while(digitalRead(btn1) == LOW && digitalRead(btn2) == LOW){
    
  }
  analogWrite(blue, 0);
  lcd.clear();
  if (digitalRead(btn1) == HIGH){
    ajustarLuminosidade();
  }

  delay(100);

  lcd.clear();  
  lcd.print("Convert Temp");
  analogWrite(blue, 255);
  while(digitalRead(btn1) == LOW && digitalRead(btn2) == LOW){
    
  }
  analogWrite(blue, 0);
  lcd.clear();
  if (digitalRead(btn1) == HIGH){
    fahrenheit = true;
  }
  else if (digitalRead(btn2) == HIGH){
    fahrenheit = false;
  }

}
