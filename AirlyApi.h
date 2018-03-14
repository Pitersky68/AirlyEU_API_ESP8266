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

#include <JsonListener.h>
#include <JsonStreamingParser.h>

#include <Arduino.h>
 #include <ArduinoJson.h>
#include <Client.h>

#define HOST "airapi.airly.eu" 	
#define SSL_PORT 443



class AirlyApi: public JsonListener {
  public:
    AirlyApi (String apiKey, Client &client);

	void getSensorValues(int sensorId);
	void getNearestSensor (String lat, String lon, String maxDistance);
	
	String getLocationId();
	String getAirQualityIndex();
	String getPM25();
	String getPM10();
	String getDistance();
	String getName();
	String getVendor();
	String getPollutionLevel();
	String getMeasurementTime();
	String getLatitude();
	String getLongitude();
	String getStreetNumber();
	String getRoute();
	String getLocality();
	String getCountry();
	String getPressure();
	String getHumidity();
	String getTemperature();

  private:
  void doUpdate(String url);
    //JsonObject * parseUpdates(String response);
    String _apiKey;
    Client *client;
    const int maxMessageLength = 15000;
    bool checkForOkResponse(String response);
	String currentKey;
    String currentParent = "";
	String locationId;
	String airQualityIndex;
	String pm25;
	String pm10;
	String distance;
	String name;
	String vendor;
	String pollutionLevel;
	String measurmentIime;
	String latitude;
	String longitude;
	String streetNumber;
	String route;
	String locality;
	String country;
	String pressure;
	String humidity;
	String temperature;

	
	    virtual void whitespace(char c);

    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
};



