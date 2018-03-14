
/*******************************************************************
Przykład użycia biblioteki AirlyApp

This library provide access to specific air parameters, especially air pollution PM (PM - Particulate Matters) from arily.eu
Blog autora http://theveel.com/?p=370

API documentation / dokumentacja API https://developer.airly.eu/docs

W wersji darmowej dostępu do API użytkownik zobowiązuje się do wykorzystywania API wyłącznie w celach niekomercyjnych.
Limit zapytań wynosi 1000 zapytań na dzień oraz 50 zapytań na minutę.

On a free plan, API consumer is required to use API only in non-commercial uses.
API is limited to 1000 requests per day and 50 requests per minute.
 *******************************************************************/

#include <AirlyApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "";       // your network SSID (name)
char password[] = "";  // your network key
#define API_KEY ""  // your Airly.eu API Token https://developer.airly.eu/

WiFiClientSecure client;
AirlyApi api(API_KEY, client);

//Inputs
//user location latitude, longitude
String lat = "50.0572748";
String lon = "19.907631";
String maxDistance = "2000";

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  checkNearestSonsor();
}

void checkNearestSonsor() {
  Serial.println("\n1.Getting sensor lat " + lat + " lon " + lon);
           api.getNearestSonsor(lat,lon,maxDistance);    
    int id=api.getLocationId().toInt();

if(id>0)
    //get temperature, humidity and preassure
    {
    checkSonsor(id);
    Serial.println("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    Serial.println("Lokalizacja "+api.getRoute()+" "+api.getStreetNumber()+". "+api.getDistance()+"m. stąd.");
    Serial.println("Indeks jakości powietrza: "+ api.getAirQualityIndex());
    Serial.print("PM2,5: "+api.getPM25()+"µg "+toPercent(api.getPM25().toInt(),25));
    Serial.println("  PM10: "+api.getPM10()+"µg "+toPercent(api.getPM10().toInt(),50));
    Serial.println("Temperatura: "+api.getTemperature()+"°C, ciśnienie: "+api.getPressure()+"hPa, wilgotność: "+api.getHumidity()+"%");
    
    }else{
    Serial.println("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    Serial.println("$$$$$$$$$$  A P I   E R R O R  $$$$$$$$$$");}    
    Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

}

void checkSonsor(int id) {
    api.getSensorValues(id); //(id)
}

String toPercent(int val, float base) {
  int i = (val / base) * 100;
  return (String)i + "%";
}

void loop() {

}
