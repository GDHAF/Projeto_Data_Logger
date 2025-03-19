#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <EEPROM.h>
#include <RTClib.h>

// Objeto RTC
RTC_DS1307 rtc;

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

// LED RGB
const int blue = 10;
const int green = 11;
const int red = 12;

// Botões
const int btn1 = 2;
const int btn2 = 3;

// Buzzer
const int buzzer = A0;

// Variáveis de uso
int min_luz = 0;
int max_luz = 500;

boolean fahrenheit = false;

// Endereço na EEPROM
int eepromAddr = 0;

// Variáveis para controle de tempo
unsigned long tempoAnterior = 0;
const unsigned long intervaloMenu = 2000; // Intervalo de 2 segundos para o menu

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  rtc.begin(); // inicializa o rtc
  dht.begin(); // Inicializa o sensor DHT22

  pinMode(ldr, INPUT);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

  pinMode(buzzer, OUTPUT);
  introduction();
  delay(2000);
  lcd.clear();
}

void menu() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int lumin = map(analogRead(ldr), 0, 969, 0, 100); 

  if (isnan(t) || isnan(h)) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Failed to read DHT");
    return;
  } 

  //Alarme temperatura
  if (t < 15 || t > 25) {
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp. crítica");
    tone(buzzer, 150, 100); 
    delay(120);             
    tone(buzzer, 150, 200); 
    delay(400);
    lcd.clear();
  }
  //Alarme humidade
  if (h < 30 || h > 50) {
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Umidade crítica");
    tone(buzzer, 150, 100); 
    delay(120);             
    tone(buzzer, 150, 200); 
    delay(400);
    lcd.clear();
  }
  //Alarme Luminosidade
  if (lumin < min_luz || lumin > (max_luz * 0.3)) {
    lcd.clear();
    alerta();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Luminosidade Crítica");
    tone(buzzer, 150, 100); 
    delay(120);             
    tone(buzzer, 150, 200); 
    delay(400);
    lcd.clear();
  }

  //Mostrando Umidade
  iconUmi();
  lcd.setCursor(0, 0);
  lcd.print("Umid: " + String(h));
  delay(2000);
  lcd.clear();

  
  //Mostrando Temperatura
  iconTemp();
  lcd.setCursor(0, 0);
  
  if(fahrenheit == false){
  lcd.print("Temp: " + String(t) + "C");
  }
  if(fahrenheit == true){
    lcd.print("Temp: " + String(TransformaFahrenheit(t)) + "F");
  }

  //Mostrando Luminosidade
  delay(2000);
  lcd.clear();
  iconLum();
  lcd.setCursor(0, 0);
  lcd.print("Lum: " + String(lumin));
  delay(2000);
  lcd.clear();

// Armazenamento dos dados na EEPROM
EEPROM.put(eepromAddr, t); // Armazena a temperatura
eepromAddr += sizeof(float);
Serial.print("Temperatura armazenada: ");
Serial.print(t);
Serial.println("C");

EEPROM.put(eepromAddr, h); // Armazena a umidade
eepromAddr += sizeof(float);
Serial.print("Umidade armazenada: ");
Serial.print(h);
Serial.println("%");

EEPROM.put(eepromAddr, lumin); // Armazena a luminosidade
eepromAddr += sizeof(int);
Serial.print("Luminosidade armazenada: ");
Serial.println(lumin);


// Obtém a hora atual do RTC
DateTime now = rtc.now();
int hora = now.hour();
int minuto = now.minute();

// Armazena a hora e os minutos na EEPROM
EEPROM.put(eepromAddr, hora);
eepromAddr += sizeof(int);
Serial.print("Hora armazenada: ");
Serial.println(hora);

EEPROM.put(eepromAddr, minuto);
eepromAddr += sizeof(int);
Serial.print("Minuto armazenado: ");
Serial.println(minuto);


if (eepromAddr >= 1024) eepromAddr = 0; // Reinicia o endereço para evitar estouro da EEPROM
}

int TransformaFahrenheit(int t) {
  int temperatura = (t * 1.8) + 32;
  return temperatura;
}

void BateCoracao() {
  analogWrite(red, 255);
  tone(buzzer, 150, 100); 
  delay(120);             
  tone(buzzer, 150, 200); 
  delay(800);
  analogWrite(red, 0);
}

void alerta() {
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}

void ajustarLuminosidade() {
  lcd.setCursor(1, 0);
  lcd.print("Lumin Minima");

  delay(1000);
  analogWrite(blue, 255);

  while (digitalRead(btn1) == LOW) {}

  analogWrite(blue, 0);
  BateCoracao();

  min_luz = map(analogRead(ldr), 0, 969, 0, 100);
  analogWrite(red, 0);
  lcd.clear();
  lcd.print(min_luz);
  delay(2000);

  lcd.clear();

  delay(2000);
  lcd.setCursor(1, 0);
  lcd.print("Lumin Maxima");

  analogWrite(blue, 255);
  while (digitalRead(btn1) == LOW) {}
  analogWrite(blue, 0);

  BateCoracao();

  max_luz = map(analogRead(ldr), 0, 969, 0, 100);
  delay(500);
  lcd.clear();
  lcd.print(0.3 * max_luz);
  delay(2000);
  lcd.clear();
}

void menu2() {
  lcd.clear();
  lcd.print("Ajustar Lum?");

  analogWrite(blue, 255);
  while (digitalRead(btn1) == LOW && digitalRead(btn2) == LOW) {}
  analogWrite(blue, 0);

  lcd.clear();
  if (digitalRead(btn1) == HIGH) {
    ajustarLuminosidade();
    lcd.clear();

  }

  delay(2500);

  
  lcd.print("Convert Temp");
  analogWrite(blue, 255);
  while (digitalRead(btn1) == LOW && digitalRead(btn2) == LOW) {}
  analogWrite(blue, 0);
  lcd.clear();

  if (digitalRead(btn1) == HIGH) {
    fahrenheit = !fahrenheit; 
    
  } 
}

void loop() {
  // Verifica se o botão 2 foi pressionado
  if (digitalRead(btn2) == HIGH && digitalRead(btn1) == LOW) {
    lcd.clear();
    lcd.print("Aguarde...");
    delay(4000);
    lcd.clear();
    menu2();
  }

  // Executa o menu principal em intervalos regulares
  if (millis() - tempoAnterior >= intervaloMenu) {
    tempoAnterior = millis();
    menu();
  }
}

//ICONES

void iconLum(){
byte name1x14[] = { B11000, B11000, B11000, B11000, B11111, B10001, B11011, B01110 };
byte name0x13[] = { B00000, B01000, B00100, B00001, B00011, B00111, B00111, B10111 };
byte name0x14[] = { B00100, B00000, B11111, B11000, B11000, B11000, B11000, B11000 };
byte name0x15[] = { B00000, B00010, B00100, B10000, B01000, B00100, B00100, B00101 };
byte name1x13[] = { B00111, B00111, B00011, B00001, B00100, B01000, B00000, B00000 };
byte name1x15[] = { B00100, B00100, B01000, B10000, B00100, B00010, B00000, B00000 };

lcd.createChar(0, name1x14);
  lcd.setCursor(14, 1);
  lcd.write(0);
  
  lcd.createChar(1, name0x13);
  lcd.setCursor(13, 0);
  lcd.write(1);
  
  lcd.createChar(2, name0x14);
  lcd.setCursor(14, 0);
  lcd.write(2);
  
  lcd.createChar(3, name0x15);
  lcd.setCursor(15, 0);
  lcd.write(3);
  
  lcd.createChar(4, name1x13);
  lcd.setCursor(13, 1);
  lcd.write(4);
  
  lcd.createChar(5, name1x15);
  lcd.setCursor(15, 1);
  lcd.write(5);

}

void iconUmi() {
	byte name0x1[] = { B00000, B00001, B00010, B00110, B01010, B10010, B00001, B00000 };
	byte name0x0[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00010 };
	byte name0x2[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000 };
	byte name1x0[] = { B00100, B00100, B00100, B00100, B00010, B00001, B00000, B00000 };
	byte name1x1[] = { B00000, B00000, B00001, B00010, B00100, B00001, B11110, B00000 };
	byte name1x2[] = { B11000, B01000, B01000, B01000, B10000, B00000, B00000, B00000 };  

  lcd.createChar(0, name0x1);
  lcd.setCursor(14, 0);
  lcd.write(0);
  
  lcd.createChar(1, name0x0);
  lcd.setCursor(13, 0);
  lcd.write(1);
  
  lcd.createChar(2, name0x2);
  lcd.setCursor(15, 0);
  lcd.write(2);
  
  lcd.createChar(3, name1x0);
  lcd.setCursor(13, 1);
  lcd.write(3);
  
  lcd.createChar(4, name1x1);
  lcd.setCursor(14, 1);
  lcd.write(4);
  
  lcd.createChar(5, name1x2);
  lcd.setCursor(15, 1);
  lcd.write(5);
}

void iconTemp() {
	byte name1x13[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000 };
	byte name0x14[] = { B01110, B10001, B10011, B10001, B10011, B10001, B10011, B10001 };
	byte name1x14[] = { B10011, B10001, B10011, B10001, B11111, B11111, B11111, B01110 };

  lcd.createChar(0, name1x13);
  lcd.setCursor(13, 1);
  lcd.write(0);
  
  lcd.createChar(1, name0x14);
  lcd.setCursor(14, 0);
  lcd.write(1);
  
  lcd.createChar(2, name1x14);
  lcd.setCursor(14, 1);
  lcd.write(2);
}



void introduction(){ //<- <- <- <- <- <- <- Função EXEC
    loadingPart(); 
    lcd.clear();
    animarTexto();
}
void Heart1part2(){
  byte name0x7[] = { B00000, B00000, B01100, B10010, B00001, B00000, B00000, B00000 };
  byte name0x6[] = { B00000, B00000, B00000, B00000, B00001, B00010, B00010, B00010 };
  byte name0x8[] = { B00000, B00000, B00110, B01001, B10000, B00000, B00000, B00000 };
  byte name0x9[] = { B00000, B00000, B00000, B00000, B10000, B01000, B01000, B01000 };
  byte name1x6[] = { B00010, B00001, B00000, B00000, B00000, B00000, B00000, B00000 };
  byte name1x7[] = { B00000, B00000, B10000, B01000, B00110, B00001, B00000, B00000 };
  byte name1x8[] = { B00000, B00000, B00001, B00010, B01100, B10000, B00000, B00000 };
  byte name1x9[] = { B01000, B10000, B00000, B00000, B00000, B00000, B00000, B00000 };
  
  lcd.createChar(0, name0x7);
  lcd.setCursor(7, 0);
  lcd.write(0);
  
  lcd.createChar(1, name0x6);
  lcd.setCursor(6, 0);
  lcd.write(1);
  
  lcd.createChar(2, name0x8);
  lcd.setCursor(8, 0);
  lcd.write(2);
  
  lcd.createChar(3, name0x9);
  lcd.setCursor(9, 0);
  lcd.write(3);
  
  lcd.createChar(4, name1x6);
  lcd.setCursor(6, 1);
  lcd.write(4);
  
  lcd.createChar(5, name1x7);
  lcd.setCursor(7, 1);
  lcd.write(5);
  
  lcd.createChar(6, name1x8);
  lcd.setCursor(8, 1);
  lcd.write(6);
  
  lcd.createChar(7, name1x9);
  lcd.setCursor(9, 1);
  lcd.write(7);
  delay(100);
}

void Heart2part2(){
byte name0x9[] = { B00000, B00000, B10000, B11000, B11100, B01100, B01100, B01100 };
byte name0x6[] = { B00000, B00000, B00001, B00011, B00111, B00110, B00110, B00110 };
byte name0x7[] = { B00000, B11100, B11110, B10011, B00001, B00000, B00000, B00000 };
byte name0x8[] = { B00000, B00111, B01111, B11001, B10000, B00000, B00000, B00000 };
byte name1x6[] = { B00110, B00011, B00001, B00000, B00000, B00000, B00000, B00000 };
byte name1x7[] = { B00000, B00000, B10000, B11000, B01110, B00111, B00001, B00000 };
byte name1x8[] = { B00000, B00000, B00001, B00011, B01110, B11100, B10000, B00000 };
byte name1x9[] = { B01100, B11000, B10000, B00000, B00000, B00000, B00000, B00000 };

lcd.createChar(0, name0x9);
  lcd.setCursor(9, 0);
  lcd.write(0);
  
  lcd.createChar(1, name0x6);
  lcd.setCursor(6, 0);
  lcd.write(1);
  
  lcd.createChar(2, name0x7);
  lcd.setCursor(7, 0);
  lcd.write(2);
  
  lcd.createChar(3, name0x8);
  lcd.setCursor(8, 0);
  lcd.write(3);
  
  lcd.createChar(4, name1x6);
  lcd.setCursor(6, 1);
  lcd.write(4);
  
  lcd.createChar(5, name1x7);
  lcd.setCursor(7, 1);
  lcd.write(5);
  
  lcd.createChar(6, name1x8);
  lcd.setCursor(8, 1);
  lcd.write(6);
  
  lcd.createChar(7, name1x9);
  lcd.setCursor(9, 1);
  lcd.write(7);
  delay(100);

}
void Heart3part2(){
byte name0x8[] = { B00111, B01111, B11111, B11001, B10000, B00000, B00000, B00000 };
byte name0x6[] = { B00000, B00001, B00011, B00111, B01111, B01110, B01110, B01110 };
byte name0x7[] = { B11100, B11110, B11111, B10011, B00001, B00000, B00000, B00000 };
byte name0x9[] = { B00000, B10000, B11000, B11100, B11110, B01110, B01110, B01110 };
byte name1x6[] = { B00110, B00111, B00011, B00001, B00000, B00000, B00000, B00000 };
byte name1x7[] = { B00000, B00000, B10000, B11000, B11110, B01111, B00111, B00001 };
byte name1x8[] = { B00000, B00000, B00001, B00011, B01111, B11110, B11100, B10000 };
byte name1x9[] = { B01110, B11100, B11000, B10000, B00000, B00000, B00000, B00000 };

  lcd.createChar(0, name0x8);
  lcd.setCursor(8, 0);
  lcd.write(0);
  
  lcd.createChar(1, name0x6);
  lcd.setCursor(6, 0);
  lcd.write(1);
  
  lcd.createChar(2, name0x7);
  lcd.setCursor(7, 0);
  lcd.write(2);
  
  lcd.createChar(3, name0x9);
  lcd.setCursor(9, 0);
  lcd.write(3);
  
  lcd.createChar(4, name1x6);
  lcd.setCursor(6, 1);
  lcd.write(4);
  
  lcd.createChar(5, name1x7);
  lcd.setCursor(7, 1);
  lcd.write(5);
  
  lcd.createChar(6, name1x8);
  lcd.setCursor(8, 1);
  lcd.write(6);
  
  lcd.createChar(7, name1x9);
  lcd.setCursor(9, 1);
  lcd.write(7);
  delay(100);
}
void pulso(){

  for(int a = 0; a< 4; a++){
    if (a == 0){
    Heart1part2();
    Heart2part2();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Heart");
    lcd.setCursor(12,1);
    lcd.print("Beat");
    Heart3part2();
    }
    else{
    lcd.setCursor(0,1);
    lcd.print("/\/\/");
    lcd.setCursor(13,0);
    lcd.print("\/\/\/");
    Heart1part2();
    Heart2part2();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Heart");
    lcd.setCursor(12,1);
    lcd.print("Beat");
    Heart3part2();
    lcd.setCursor(0,1);
    lcd.print("\/\/\/");
    lcd.setCursor(13,0);
    lcd.print("/\/\/");
    }
  }
}
void animarTexto() {
    const char palavra1[] = "Heart";
    const char palavra2[] = "Beat";
    int len1 = strlen(palavra1);
    int len2 = strlen(palavra2);
    
    for (int i = 0; i < len1 || i < len2; i++) {
        if (i < len1) {
            lcd.setCursor(i, 0); 
            lcd.print(palavra1[i]);
        }
        
        if (i < len2) {
            lcd.setCursor(15 - i, 1); 
            lcd.print(palavra2[len2 - 1 - i]);
        }
        
        delay(300); 
    }
    delay(200);
    pulso();
}

void dotLoad(){
  for (int a = 7; a< 10;a++ ){
    lcd.setCursor(a,1);
    lcd.print(".");
    delay(50);
  }

}

void loadingPart(){
  byte  zero[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
  byte B[] = { B00001, B01110, B01110, B00001, B01110, B01110, B00000, B11111 };
  byte E[] = { B11111, B11111, B10001, B01110, B00000, B01111, B10001, B11111 };
  byte A[] = { B11111, B11111, B10001, B11110, B10000, B01110, B10000, B11111 };
  byte T[] = { B10111, B10111, B00011, B10111, B10111, B10110, B11001, B11111 };
  byte cora[] = { B11111, B11111, B10101, B01010, B01110, B10101, B11011, B11111 };



  lcd.setCursor(10,0);
  lcd.print("Heart");
  lcd.setCursor(0,1);
  lcd.print("Loading");

  for(int g = 0; g < 6; g++){
    delay(250);
    dotLoad();
    if(g==0){
      lcd.createChar(g, zero);
      lcd.setCursor(10, 1);
      lcd.write(g);

  }
   if(g==1){
      lcd.createChar(g, B);
      lcd.setCursor(11, 1);
      lcd.write(g);
  }
   if(g==2){
      lcd.createChar(g, E);
      lcd.setCursor(12, 1);
      lcd.write(g);
  }
   if(g==3){
      lcd.createChar(g, A);
      lcd.setCursor(13, 1);
      lcd.write(g);
  } 
  if(g==4){
      lcd.createChar(g, T);
      lcd.setCursor(14, 1);
      lcd.write(g);
  }
   if(g==5){
      lcd.createChar(g, cora);
      lcd.setCursor(15, 1);
      lcd.write(g);
      delay(250);
  }
  }
}