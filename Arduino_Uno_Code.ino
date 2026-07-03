#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int chargePin = 8;
const int dischargePin = 9; 
const int analogPin = A0;

const int regPowerPin = 7;
const int regAnalogPin = A1;

float V_IN = 5.0;
float R_REF = 1000.0; 
unsigned long startTime;
unsigned long elapsedTime;


int emptyCount = 0; 

void setup() {
  Serial.begin(9600); 
  pinMode(chargePin, OUTPUT);
  pinMode(dischargePin, OUTPUT);
  pinMode(regPowerPin, OUTPUT);
  digitalWrite(regPowerPin, LOW);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("VoltQ: CHECK NOW"); 
}

void loop() {
  digitalWrite(regPowerPin, HIGH);
  delay(50);
  int regValue = analogRead(regAnalogPin);
  float regVoltage = (regValue * V_IN) / 1023.0;
  
  if (regVoltage > 4.0) {
    lcd.setCursor(0, 1);
    lcd.print("REGULATOR PASS  ");
    lcd.setCursor(0, 1);
    lcd.print("Vout: "); lcd.print(regVoltage, 2); lcd.print("V 5V Reg");
    Serial.print("REG:"); Serial.print(regVoltage, 2); Serial.println(" V");
    delay(7000); 
    digitalWrite(regPowerPin, LOW);
    return;
  }
  digitalWrite(regPowerPin, LOW);

  digitalWrite(chargePin, LOW);
  pinMode(dischargePin, OUTPUT);
  digitalWrite(dischargePin, LOW);
  lcd.setCursor(0, 1);
  lcd.print("Clearing Slot.. ");
  delay(600); 
  
  pinMode(dischargePin, INPUT); 
  digitalWrite(chargePin, HIGH);
  delay(5);
  int test1 = analogRead(analogPin);
  delay(20);
  int test2 = analogRead(analogPin);
  
  if (test1 >= 1015 && test1 < 1023) {
    emptyCount = 0; 
    lcd.setCursor(0, 1);
    lcd.print("Resistor > 100 kOhm    "); 
    Serial.println("above 100k ohm");
    delay(7000); // Increased display time
    return;
  }
  
  if (test1 >= 1023) {
    emptyCount++; 
    
    lcd.setCursor(0, 1);
    if (emptyCount > 1) {
      
      lcd.print("ALERT: BAD COND "); 
      Serial.println("ALERT: BAD CONDITION (REPEATED EMPTY)");
    } else {
      // First time empty
      lcd.print("Slot Empty      ");
      Serial.println("EMPTY"); 
    }
    delay(8000); 
    return;
  }
  
  if (test1 <= 5) {
    emptyCount = 0; 
    lcd.setCursor(0, 1);
    lcd.print("ALERT: POOR CONDITION   ");
    Serial.println("SHORT"); 
    delay(7000); 
    return;
  }

  emptyCount = 0; 

  if (test2 > test1 + 8) {
    lcd.setCursor(0, 1);
    lcd.print("Measuring Capacitor...");
    
    digitalWrite(chargePin, LOW);
    pinMode(dischargePin, OUTPUT);
    digitalWrite(dischargePin, LOW);
    delay(600);
    pinMode(dischargePin, INPUT);
    
    digitalWrite(chargePin, HIGH);
    startTime = micros();
    while(analogRead(analogPin) < 647) {
      if ((micros() - startTime) > 3000000) break;
    }
    elapsedTime = micros() - startTime;
    
    float capacitance = ((float)elapsedTime / R_REF); 
    lcd.setCursor(0, 1);
    
    if (capacitance > 1.0) {
      lcd.print("C: "); lcd.print(capacitance, 1); lcd.print(" uF GOOD CONDITION      ");
      Serial.print("CAPACITOR:"); Serial.print(capacitance, 1); Serial.println(" uF"); 
    } else {
      lcd.print("C: "); lcd.print(capacitance * 1000.0, 0); lcd.print(" nF GOOD CONDITION      ");
      Serial.print("CAPACITOR:"); Serial.print(capacitance * 1000.0, 0); Serial.println(" nF"); 
    }
    delay(7000); 
  } 
  else {
    float vHigh = (test2 * V_IN) / 1023.0; 
    
    pinMode(chargePin, INPUT_PULLUP); 
    delay(15);
    int lowCurrentReading = analogRead(analogPin);
    float vLow = (lowCurrentReading * V_IN) / 1023.0; 
    
    pinMode(chargePin, OUTPUT);
    digitalWrite(chargePin, HIGH); 

    if ((vHigh - vLow) < 0.4 && vHigh > 0.5) {
      lcd.setCursor(0, 1);
      lcd.print("DIODE/LED PASS GOOD CONDITION ");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print("Vf: "); lcd.print(vHigh, 2); lcd.print("V Anode->R20");
      Serial.print("LED:"); Serial.print(vHigh, 2); Serial.println(" V"); 
      delay(7000); 
    } 
    else {
      float rUnknown = R_REF * (vHigh / (V_IN - vHigh));
      lcd.setCursor(0, 1);
      lcd.print("R: ");
      if (rUnknown >= 1000.0) {
        lcd.print(rUnknown / 1000.0, 1); lcd.print(" kOhm     ");
        Serial.print("RESISTOR:"); Serial.print(rUnknown / 1000.0, 1); Serial.println(" kOhm"); 
      } else {
        lcd.print((int)rUnknown); lcd.print(" Ohm GOOD CONDITION     ");
        Serial.print("RESISTOR:"); Serial.print((int)rUnknown); Serial.println(" Ohm"); 
      }
      delay(7000); 
    }
  }
}
