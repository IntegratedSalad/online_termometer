#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include "DHT.h"
//#include "FS.h"
#include <LittleFS.h>

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
const char directory_path[] = "/data";
const char folder[] = "/test";

bool save = true;

void setup() {
  // put your setup code here, to run once:

  
  Serial.begin(57600);
  lcd.init();
  lcd.backlight();
  dht.begin();

  /*
  cfg.setAutoFormat(false);
  SPIFFS.setConfig(cfg);
  SPIFFS.begin();
  */
  LittleFS.format();
  LittleFS.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.clear();

  float *filled_data = gather_data(data);
  float temperature = filled_data[0];
  float humidity = filled_data[1];

  lcd.setCursor(2, 0);
  lcd.print(F("Temperature:"));
  lcd.setCursor(5, 1);
  lcd.print(temperature);

  delay(2000);
  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.print(F("Humidity: "));
  lcd.setCursor(5, 1);
  lcd.print(humidity);
  delay(2000);

  if (save)
  {
    save_data(temperature, humidity, "/test.txt");
//    save = false;
  }
  
  delay(2000);

  read_data();

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

void create_txt(String file_name)
{
  File f = LittleFS.open(file_name, "a");
  f.close();
}

void save_data(float temp, float humid, const char folder_name[])
{
    // concatenate strings

    //String path = String(directory_path) + String(folder_name) + String("/test.txt");
    String path = String("/test.txt");
    File f = LittleFS.open(path, "a");
    lcd.clear();
    if (!f) 
    {
      lcd.setCursor(0, 0);
      lcd.print("Error:");
      lcd.setCursor(0, 1);
      lcd.print("FILE O. FAILED");
      delay(10000);
      
    } else
    {
      String temp_str = String(temp);
      String humid_str = String(humid);
      String to_print = String(temp_str + ";" + humid_str + "\n");
      const char* final_print = to_print.c_str();
      lcd.setCursor(0, 0);
      f.write(final_print);
      lcd.print("Written to file!");
      f.close();
    }
}

void read_file(File f)
{
  String line = ".";
  while (line != "")
  {
    line = f.readString();
    Serial.println(line);
  }
}

void read_data()
{
  String path = String("/test.txt");
  File f = LittleFS.open(path, "r");
  read_file(f);
  f.close();

}

bool check_if_file(String path)
{

  return LittleFS.exists(path);
  
}

String get_folder_name_from_data()
{
  String empty = String();
  return empty;
}

// Measure temp and save temperature at 0.00 6.00 12.00 18.00.
// At the end of a month, send file to a webpage. 
