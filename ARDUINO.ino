#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int boton_manual = 6;
const int sensor_humedad = A0;
const int bomba_sumergible = 8;
const int sensor_nivel = 9;
unsigned int contador_riegos = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(sensor_humedad, INPUT);
  pinMode(bomba_sumergible, OUTPUT);
  pinMode(sensor_nivel, INPUT);
  pinMode(boton_manual, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  int humedad = analogRead(sensor_humedad);
  int nivel = digitalRead(sensor_nivel);
  int boton = digitalRead(boton_manual);

  Serial.println(humedad);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad Suelo:");
  lcd.setCursor(0, 1);
  lcd.print(humedad);
  lcd.print("%");
  delay(1000);

  if (nivel == 0 && boton == 1) 
  {
    Serial.println("Nivel BAJO de agua. Rellenar tanque.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nivel BAJO agua!");
    lcd.setCursor(0, 1);
    lcd.print("Rellenar TANQUE!");
    delay(2000);
  }
  else if (nivel == 0 && boton == 0) 
  {
    Serial.println("Imposible REGAR, nivel bajo de AGUA. Debe RELLENAR el TANQUE!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IMPOSIBLE regar!");
    lcd.setCursor(0, 1);
    lcd.print("NIVEL bajo!");
    delay(2000);
  }
  else if (nivel == 1) 
  {
    Serial.println("Nivel CORRECTO de agua.");

    if (humedad >= 700) 
    {
      contador_riegos++;
      String mensajeRiego = "Riego #" + String(contador_riegos) + " a los " + String(millis() / 1000) + " segundos";
      Serial.println(mensajeRiego);

      Serial.println("La tierra está SECA, comenzando riego AUTOMÁTICO.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Riego automatico");
      lcd.setCursor(0, 1);
      lcd.print("HS: ");
      lcd.print(humedad);
      lcd.print("%");

      digitalWrite(bomba_sumergible, HIGH);
      delay(2000);
      digitalWrite(bomba_sumergible, LOW);
      delay(1000);

      if (boton == 0) 
      {
        Serial.println("No se puede activar RIEGO MANUAL. RIEGO AUTOMÁTICO ACTIVO.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Regando automatico");
        lcd.setCursor(0, 1);
        lcd.print("No MANUAL");
        delay(1000);
      }
    }

    if (humedad < 700)
    {
      if (boton == 0) 
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Riego MANUAL");
        lcd.setCursor(0, 1);
        lcd.print("HS: ");
        lcd.print(humedad);
        lcd.print("%");
        
        digitalWrite(bomba_sumergible, HIGH);
      } 
      else if (boton == 1) 
      {
        digitalWrite(bomba_sumergible, LOW);
      }
    }
  }
}
