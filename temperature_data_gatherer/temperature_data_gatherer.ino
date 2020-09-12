#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <iostream>
#include <fstream>
#include "DHT.h"
#include "FS.h"

#define DHTPIN 2
#define DHTTYPE DHT11

//LCD SETUP
int lcd_columns = 16;
int lcd_rows = 2;



LiquidCrystal_I2C lcd(0x27, lcd_columns, lcd_rows);
//

// DHT11 SETUP

DHT dht(DHTPIN, DHTTYPE);

//

// SPIFFS SETUP

SPIFFSConfig cfg;

//

float data[2];
const char directory_path[] = "/Users/buppo/Documents/Arduino/temperature_data_gatherer/data";
const char folder[] = "/test";


bool save = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();

  cfg.setAutoFormat(false);
  SPIFFS.setConfig(cfg);
  SPIFFS.begin();
  

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(8000);
  lcd.clear();

  float *filled_data = gather_data(data);
  float temperature = filled_data[0];
  float humidity = filled_data[1];

  lcd.setCursor(2, 0);
  lcd.print(F("Temperature:"));
  lcd.setCursor(2, 0);
  lcd.print(F("Temperature:"));
  lcd.setCursor(5, 1);
  lcd.print(temperature);

  delay(8000);
  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.print(F("Humidity: "));
  lcd.setCursor(5, 1);
  lcd.print(humidity);

  if (save)
  {
    save_data(temperature, humidity, "/test");
  }


}

float* gather_data(float arr[])
{

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h))
  {
    lcd.setCursor(0, 0);
    lcd.print("Error:");
    lcd.setCursor(0, 1);
    lcd.print("Failed to read.");

    while (1);
    return arr;
    
  } else
  {
    arr[0] = t;
    arr[1] = h;
    
    return arr;
  }
  
}

void save_data(float temp, float humid, const char folder_name[])
{
    // concatenate strings

    String path = String(directory_path) + String(folder_name);
    
    File f = SPIFFS.open(path, "a");
    lcd.clear();
    if (!f) 
    {
      lcd.setCursor(0, 0);
      lcd.print("Error:");
      lcd.setCursor(0, 1);
      lcd.print("FILE O. FAILED");
      while(1);
      
    } else
    {
      String temp_str = String(temp);
      String humid_str = String(humid);
      String to_print = String(temp_str + " " + humid_str);
      f.println(to_print);

      f.close();
    }
}

String get_folder_name_from_data()
{
  String empty = String();
  return empty;
}

// Measure temp and save temperature at 0.00 6.00 12.00 18.00.
// At the end of a month, send file to a webpage. 
