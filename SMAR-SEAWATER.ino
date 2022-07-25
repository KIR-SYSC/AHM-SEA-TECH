#include "GravityTDS.h"
#include "LCD_I2C.h"

#define TDS_PIN   A0
#define PH_PIN    A1
#define RELAY_PIN 3

LCD_I2C lcd(0x27, 16, 2);
GravityTDS gravityTds;

int samples           = 10;
float adc_resolution  = 1024.0,
      voltage_to_ph   = 3.3,
      voltage_to_tds  = 4.74,
      temp_val        = 25;
      
float getPH();
int  getTDS();
void logic(float ph_value, int tds_value);

void setup(){
    pinMode(RELAY_PIN, OUTPUT);
    gravityTds.setPin(TDS_PIN);
    gravityTds.setAref(voltage_to_tds); 
    gravityTds.setAdcRange(adc_resolution);  
    gravityTds.begin(); 
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("Welcome to");
    lcd.setCursor(4, 1);
    lcd.print("SEA-TACH");
    
    delay(2000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Smart Seawater");
    lcd.setCursor(2, 1);
    lcd.print("Desalination");
    delay(2000);
    lcd.clear();
    Serial.begin(9600);
}

void loop(){ 
    lcd.clear();
    float ph_value  = getPH();
      Serial.print  ("pH = :");
      Serial.println (ph_value);
      lcd.setCursor(0, 0);
      lcd.print("pH   : ");
      lcd.print(ph_value);
    int   tds_value = getTDS();
      Serial.print  ("TDS = :");
      Serial.println (tds_value); 
      lcd.setCursor(0, 1);
      lcd.print("TDS  : "); 
      lcd.print(tds_value);
      lcd.print(" ppm");
      delay(1000);
    logic(ph_value, tds_value);
}

void logic(float ph_value, int tds_value){
  if(ph_value >= 6.5 && ph_value <=8.5 && tds_value <=900){
      Serial.println ("Air Layak Minum");
      digitalWrite(RELAY_PIN, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Air Bersih");
      lcd.setCursor(3, 1);
      lcd.print("Lanjutkan");
      delay(1000);
      
  }
  else{
    Serial.println ("Air tidak layak minum");
    digitalWrite(RELAY_PIN, LOW);
     lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Air Kotor");
      lcd.setCursor(1, 1);
      lcd.print("Ulangi Proses");
      delay(1000);
  }
}
float getPH () {
    int measurings=0;
    for (int i = 0; i < samples; i++){
        measurings += analogRead(PH_PIN);
        delay(10);
    }
    float voltage = 5 / adc_resolution * measurings/samples;
    return 7 + ((voltage_to_ph - voltage) / 0.18);
}
int getTDS(){
  gravityTds.setTemperature(temp_val); 
  gravityTds.update();
  return gravityTds.getTdsValue();
}
