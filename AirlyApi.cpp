
/*
The MIT License (MIT)
Copyright (c) 2018 Kamil Kaleta. All right reserved.
http://theveel.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

This library provide access to specific air parameters, especially air pollution PM (PM - Particulate Matters).
API documentation https://developer.airly.eu/docs

Airly is 'Airly sp. z o.o.' trademark. 
More About Airly https://airly.eu/en/company/

*/
#include "AirlyApi.h"

AirlyApi::AirlyApi(String apiKey, Client &client)	{
  _apiKey = apiKey;
  this->client = &client;
}

void AirlyApi::getNearestSensor(String lat, String lon, String maxDistance){
	String command="/v1/nearestSensor/measurements?latitude=" + lat + "&longitude=" + lon + "&maxDistance=" + maxDistance;
	doUpdate(command);
}

void AirlyApi::getSensorValues(int id){
	String command="/v1/sensor/measurements?sensorId=" + (String)id+"&historyHours=24&historyResolutionHours=1";	
	doUpdate(command);
}

//wersja z parserem i listenerem
void AirlyApi::doUpdate(String url) {
	yield();
	JsonStreamingParser parser;
	parser.setListener(this);
  
	    Serial.print(">>command: ");
        Serial.println(url);
		
  String headers = "";
  String body = "";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  long now;
  bool avail;
  boolean isBody = false;
  // Connect with airly api over ssl
  if (client->connect(HOST, SSL_PORT)) {
    Serial.println(".... connected to server");
    String a = "";
    char c;
    int ch_count = 0;
	
	    client->print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266theveelCom\r\n" +
			   "apikey: "+_apiKey+"\r\n" +
               "Connection: close\r\n\r\n");
			   
    // client->println("GET " + url + "&apikey=" + _apiKey);
	///////
    now = millis();
    avail = false;
    while (millis() - now < 2500) {
	// while(client.connected()) {
		// Serial.print("jest klient? ");
		// Serial.println(client->available());
      while (client->available()) {
        c = client->read();
		

      if (c == '{' || c == '[') {
        isBody = true;
      }
		
			// Serial.write(c);
          if (ch_count < maxMessageLength) {
			// Serial.println("building body!");
            ///////////////////////////////////////////////////////////
			// body = body + c;
			      if (isBody) {
						parser.parse(c);
					}
			///////////////////////////////////////////////////////////
            ch_count++;
          }
        // }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }

        avail = true;
      // }
   
	}
	   if (avail) {
        // Serial.println("Body:");
        // Serial.println(body);
        // Serial.println("END");
        // break;
      }
	  
    }
  }


}



void AirlyApi::whitespace(char c) {
  // Serial.println("\nwhitespace");
}

void AirlyApi::startDocument() {
  // Serial.println("\nstart document");
}

void AirlyApi::key(String key) {
	
	Serial.println("*----------------------------------------*\ncurrentParent: " + currentParent);
  Serial.println("key: " + key);
  currentKey = String(key);
}
	
void AirlyApi::value(String value) {
  Serial.println("value: " + value);
if(currentParent==""){  
  if(currentKey=="airQualityIndex"){
	  Serial.println("indeks powietrza "+(String)value);
	  airQualityIndex=value.toInt();
  }else
   if(currentKey=="pm25"){
	  Serial.println("pm25 "+(String)value);
	  pm25=value.toInt();
  }else
	if(currentKey=="pm10"){
	  Serial.println("pm10 "+(String)value);
	  pm10=value.toInt();
  }else
	if(currentKey=="distance"){
	  Serial.println("distance "+(String)value);
	  distance=value.toInt();
  }else
	if(currentKey=="id"){
	  Serial.println("id "+(String)value);
	  locationId=value;
  }else
	if(currentKey=="name"){
	  Serial.println("name "+(String)value);
	  name=value;
  }else
	if(currentKey=="vendor"){
	  Serial.println("vendor "+(String)value);
	  vendor=value;
  }else
   if(currentKey=="pollutionLevel"){
	  Serial.println("pollutionLevel "+(String)value);
	  pollutionLevel=value;
  }else
   if(currentKey=="measurementTime"){
	  Serial.println("measurementTime "+(String)value);
	  measurmentIime=value;
  }
}  
  else	 if(currentParent=="location"){
				if(currentKey=="latitude"){
					Serial.println("latitude "+(String)value);
					latitude=value;
				}else
				if(currentKey=="longitude"){
					Serial.println("longitude "+(String)value);
					longitude=value;
				}	  
			}
			
  else	 if(currentParent=="address"){
				if(currentKey=="streetNumber"){
					Serial.println("streetNumber "+(String)value);
					streetNumber=value;
				}else
				if(currentKey=="route"){
					Serial.println("route "+(String)value);
					route=value;
				}else
				if(currentKey=="locality"){
					Serial.println("locality "+(String)value);
					locality=value;
				}
				if(currentKey=="country"){
					Serial.println("country "+(String)value);
					country=value;
				}

			}
	
//singleSensorValuesNyId
  else	 if(currentParent=="currentMeasurements"){
				if(currentKey=="temperature"){
					Serial.println("temperature "+(String)value);
					temperature=value.toInt();
				}else
				if(currentKey=="pressure"){
					Serial.println("pressure "+(String)value);
					pressure=value.toInt()/100;
				}else
				if(currentKey=="humidity"){
					Serial.println("humidity "+(String)value);
					humidity=value.toInt();
				}
				
				//windDirection
				//windSpeed
				
			
			}
//for future use
  // else	 if(currentParent=="history"){}
  // else	 if(currentParent=="forecast"){}  
			
			
}

void AirlyApi::endArray() {
  // Serial.println("\nend array. ");
}

void AirlyApi::endObject() {
  // Serial.println("\nend object. ");
  currentParent = "";
  currentKey="";
}

void AirlyApi::endDocument() {
  // Serial.println("\nend document. ");
}

void AirlyApi::startArray() {
   // Serial.println("\nstart array. ");
}

void AirlyApi::startObject() {
   // Serial.println("\nstart object. ");
    currentParent = currentKey;
}
/////////////////////////g e t t e r s

String AirlyApi::getLocationId(){
	return locationId;
}
String AirlyApi::getAirQualityIndex(){
	return airQualityIndex;
}
String AirlyApi::getPM25(){
	return pm25;
}
String AirlyApi::getPM10(){
	return pm10;
}
String AirlyApi::getDistance(){
	return distance;
}
String AirlyApi::getName(){
	return name;
}
//
String AirlyApi::getVendor(){
	return vendor;
}
String AirlyApi::getPollutionLevel(){
	return pollutionLevel;
}
String AirlyApi::getMeasurementTime(){
	return measurmentIime;
}
String AirlyApi::getLatitude(){
	return latitude;
}
String AirlyApi::getLongitude(){
	return longitude;
}
String AirlyApi::getStreetNumber(){
	return streetNumber;
}
String AirlyApi::getRoute(){
	return route;
}
String AirlyApi::getLocality(){
	return locality;
}
String AirlyApi::getCountry(){
	return country;
}
String AirlyApi::getPressure(){
	return pressure;
}
String AirlyApi::getHumidity(){
	return humidity;
}
String AirlyApi::getTemperature(){
	return temperature;
}
//airQualityIndex