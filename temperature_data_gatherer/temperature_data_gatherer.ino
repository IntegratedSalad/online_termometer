#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h> // write functions in the seperate file
#include <SHA256.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <LittleFS.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define SALT_LEN 23

//LCD SETUP
int lcd_columns = 16;
int lcd_rows = 2;

// LCD SETUP

LiquidCrystal_I2C lcd(0x27, lcd_columns, lcd_rows);
//

// DHT11 SETUP

DHT dht(DHTPIN, DHTTYPE);

//

// SHA256 SETUP

SHA256 sha256;

//

// SERVER SETUP

ESP8266WiFiMulti wifi_multi;

ESP8266WebServer server(80);

float data[2];
const char directory_path[] = "/data";
const char folder[] = "/test";
char* hash[32];
char* hash_to_check[32];

bool save = true;

void handle_root();
void handle_not_found();

void setup() {
  
  Serial.begin(57600);
  Serial.println('\n');
  lcd.init();
  lcd.backlight();
  
  dht.begin();
  
  LittleFS.begin();

  wifi_multi.addAP("NETIASPOT-2.4GHz-AEAAF1", ""); // In order to access the site, user must provide a password
  Serial.println("Connecting...");

  while (wifi_multi.run() != WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) // mDNS responder for esp8266.local
  {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", handle_root); // handle_root when a client requests URI "/"
  server.onNotFound(handle_not_found); // handle_not_found when client requests an unknown URI

  server.begin();
  Serial.println("HTTP server started");

//  String salt_string = return_contents_file("/data.txt");
//  const char* _salt = salt_string.c_str();

  
//  char* hash = "4822f19a10832003c1c1e33c8b0a5b7120093a0277a1e087ecb3c9568181cf00";
//  char* hash_bin = "\xf1\x9a\x10\x83 \x03\xc1\xc1\xe3<\x8b\n[q \t:\x02w\xa1\xe0\x87\xec\xb3\xc9V\x81\x81\xcf\x00";

//  encrypt(another_hash, msg, salt);
//  File hash_file = LittleFS.open("/hash.txt", "a");
//  Serial.println(hash);
//  Serial.print((char*)another_hash);
////  hash_file.write(String((char*)another_hash)));
//  Serial.println(String(hash).length()); 
//  Serial.println(String((char*)another_hash).length());
//

}

void loop() {

  server.handleClient();

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
    save = false;
  }
  
  delay(2000);

//  read_data();


  /* Input via html form - print hash from msg and hash from input. */
  /* Which type will have the array with hash? */


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
      lcd.print("Written to the file!");
      f.close();
    }
}

void print_file(File f)
{
  String line = ".";
  while (line != "")
  {
    line = f.readString();
    Serial.println(line);
  }
}

String return_contents_file(String path)
{
  /* Return array of contents of file. 
     File has to have one line.
  */
  File f = LittleFS.open(path, "r");
  if (!f)
  {
    Serial.println("Couldn't find the file");
  }
  
  return f.readString();
}

void read_data()
{
  String path = String("/test.txt");
  File f = LittleFS.open(path, "r");
  print_file(f);
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

void encrypt(void* buffer, const void* message, const void* salt)
{
  // to buffer we will save our hash
  sha256.update(message, (size_t)sizeof(message));
  sha256.update(salt, (size_t)sizeof(salt));
  sha256.finalize(buffer, sha256.hashSize());
  sha256.clear();
  sha256.reset();
}

const char* get_hash()
{
  String str = return_contents_file("/hash.txt");
  const char* hash = str.c_str();
  return hash;
}

const char* get_salt()
{
  String salt_string = return_contents_file("/data.txt");
  const char* salt = salt_string.c_str();
  return salt;
}


// Measure temp and save temperature at 0.00 6.00 12.00 18.00.
// At the end of a month, send file to a webpage. 

// Create folder with the month and year format: {year}{month}
// Write to it starting 0.00, with 6 hour interval.
// After one day, upload it to the server.


/*  Todo:
 *  1. Hash the password.
 *  2. Create Json file and append data to it.
 *  3. Move file functions into seperate file
 */
