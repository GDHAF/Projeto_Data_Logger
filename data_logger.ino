int carRed = 12; // assign the car lights
int carYellow = 11;
int carGreen = 10;
int pedRed = 9; // assign the pedestrian lights
int pedGreen = 8;
int button = 2; // button pin
int crossTime = 1000; // time allowed to cross
unsigned long changeTime; // time since button pressed
int led_1 = 3;
int led_2 = 4;
int led_3 = 5;
int led_4 = 6;
int led_5 = 7;
int buzzer = 13;

void setup() {
    pinMode(carRed, OUTPUT);
    pinMode(carYellow, OUTPUT);
    pinMode(carGreen, OUTPUT);
    pinMode(pedRed, OUTPUT);
    pinMode(pedGreen, OUTPUT);
    pinMode(button, INPUT); // button on pin 2

    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
    pinMode(led_4, OUTPUT);
    pinMode(led_5, OUTPUT);

    pinMode(buzzer, OUTPUT);
    
    // turn on the green light
    digitalWrite(carGreen, HIGH);
    digitalWrite(pedRed, HIGH);
}

void loop() {
    int state = digitalRead(button);
    /* check if button is pressed and it is
    over 5 seconds since last button press */
    if (state == HIGH && (millis() - changeTime) > 5000) {
        // Call the function to change the lights
        changeLights();
    }
}

void changeLights() {
    digitalWrite(carGreen, LOW); // green off
    digitalWrite(carYellow, HIGH); // yellow on
    delay(2000); // wait 2 seconds

    digitalWrite(carYellow, LOW); // yellow off
    digitalWrite(carRed, HIGH); // red on
    delay(1000); // wait 1 second till its safe

    digitalWrite(pedRed, LOW); // ped red off
    digitalWrite(pedGreen, HIGH); // ped green on
    
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, HIGH);
    digitalWrite(led_4, HIGH);
    digitalWrite(led_5, HIGH);
   // delay(500); // wait for preset time period

    delay(crossTime);
    digitalWrite(led_1, LOW);
    
    delay(crossTime);
    digitalWrite(led_2, LOW);

    delay(crossTime);
    digitalWrite(led_3, LOW);

    delay(crossTime);
    digitalWrite(led_4, LOW);

    // flash the ped green
    for (int x = 0; x < 5; x++) {
        digitalWrite(pedGreen, HIGH);
        tone(buzzer, 262, 100);
        delay(100);

        digitalWrite(pedGreen, LOW);
        tone(buzzer, 392, 100);
        delay(100);
    }

    digitalWrite(led_5, LOW);
    delay(250);
    // turn ped red on
    digitalWrite(pedRed, HIGH);

    //digitalWrite(carYellow, HIGH); // yellow on
    digitalWrite(carRed, LOW); // red off
    delay(1000);
    digitalWrite(carGreen, HIGH);
    //digitalWrite(carYellow, LOW); // yellow off

    // record the time since last change of lights
    changeTime = millis();
    // then return to the main program loop
}