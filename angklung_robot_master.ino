int motorPins[] = {20,21,22,23,16,17,18,19,26,32,14,15,5,4,3,2};
int pwmPins[] = {6,7,8,9,10,11,12,13};

int motorSpeed = 255;  // Kecepatan motor (0-255)
unsigned long previousMillis = 0;
const long interval = 100;  // Waktu gerak motor (70 ms)

bool motorActive = false;
char currentGesture = '\0';
int stepMotor = 0;  // Step 0 = maju, Step 1 = mundur, Step 2 = berhenti

void setup() {
  Serial.begin(2000000);
  for (int i = 0; i < 16; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
  for (int i = 0; i < 8; i++) {
    pinMode(pwmPins[i], OUTPUT);
  }
}

void loop() {
    if (Serial.available() > 0) {
        char gesture = Serial.read();
        if (!motorActive) {
            controlMotors(gesture);
            currentGesture = gesture;
            motorActive = true;
            stepMotor = 0;
            previousMillis = millis();
        }
    }

    if (motorActive && millis() - previousMillis >= interval) {
        previousMillis = millis();
        if (stepMotor == 0) {
            reverseMotors(currentGesture);
            stepMotor = 1;
        } else if (stepMotor == 1) {
            stopMotors();
            motorActive = false;
            stepMotor = 2;
        }
    }
}

void controlMotors(char gesture) {
    switch (gesture) {
        case 'A': // "do"
            Serial.println("DO");
            digitalWrite(motorPins[0], HIGH);
            digitalWrite(motorPins[1], LOW);
            analogWrite(pwmPins[0], motorSpeed);
            break;
        case 'B': // "re"
            Serial.println("RE");
            digitalWrite(motorPins[2], HIGH);
            digitalWrite(motorPins[3], LOW);
            analogWrite(pwmPins[1], motorSpeed);
            break;
        case 'C': // "mi"
            Serial.println("MI");
            digitalWrite(motorPins[4], HIGH);
            digitalWrite(motorPins[5], LOW);
            analogWrite(pwmPins[2], motorSpeed);
            break;
        case 'D': // "fa"
            Serial.println("FA");
            digitalWrite(motorPins[6], HIGH);
            digitalWrite(motorPins[7], LOW);
            analogWrite(pwmPins[3], motorSpeed);
            break;
        case 'E': // "so"
            Serial.println("SO");
            digitalWrite(motorPins[8], HIGH);
            digitalWrite(motorPins[9], LOW);
            analogWrite(pwmPins[4], motorSpeed);
            break;
        case 'F': // "la"
            Serial.println("LA");
            digitalWrite(motorPins[10], HIGH);
            digitalWrite(motorPins[11], LOW);
            analogWrite(pwmPins[5], motorSpeed);
            break;
        case 'G': // "si"
            Serial.println("SI");
            digitalWrite(motorPins[12], HIGH);
            digitalWrite(motorPins[13], LOW);
            analogWrite(pwmPins[6], motorSpeed);
            break;
        case 'H': // "do tinggi"
            Serial.println("DOO");
            digitalWrite(motorPins[14], HIGH);
            digitalWrite(motorPins[15], LOW);
            analogWrite(pwmPins[7], motorSpeed);
            break;
    }
}

void reverseMotors(char gesture) {
    switch (gesture) {
        case 'A': // "do"
            digitalWrite(motorPins[0], LOW);
            digitalWrite(motorPins[1], HIGH);
            break;
        case 'B': // "re"
            digitalWrite(motorPins[2], LOW);
            digitalWrite(motorPins[3], HIGH);
            break;
        case 'C': // "mi"
            digitalWrite(motorPins[4], LOW);
            digitalWrite(motorPins[5], HIGH);
            break;
        case 'D': // "fa"
            digitalWrite(motorPins[6], LOW);
            digitalWrite(motorPins[7], HIGH);
            break;
        case 'E': // "so"
            digitalWrite(motorPins[8], LOW);
            digitalWrite(motorPins[9], HIGH);
            break;
        case 'F': // "la"
            digitalWrite(motorPins[10], LOW);
            digitalWrite(motorPins[11], HIGH);
            break;
        case 'G': // "si"
            digitalWrite(motorPins[12], LOW);
            digitalWrite(motorPins[13], HIGH);
            break;
        case 'H': // "do tinggi"
            digitalWrite(motorPins[14], LOW);
            digitalWrite(motorPins[15], HIGH);
            break;
    }
}

void stopMotors() {
    for (int i = 0; i < 16; i++) {
        digitalWrite(motorPins[i], LOW);
    }
    for (int i = 0; i < 8; i++) {
        analogWrite(pwmPins[i], 0);
    }
    Serial.println("Motor Stopped");
}
