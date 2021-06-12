#pragma once

#include "MemoryItemAgle.h"
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <WebSocketsServer.h>

#include <WiFi.h>
#include <FS.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <NTPtimeESP.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>



class CommunicateControlTask {
public:
bool flagAP;
MemoryItemAgle *memoryControl;
AsyncWebServer *server;
WiFiClient espClient;
DynamicJsonBuffer  jsonBufferWifi;
String mesage;
	//NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
public:
	virtual bool getTimeFromServer(MemoryItemAgle &datal)= 0 ;
	virtual void returnAllData(bool input)= 0;
	virtual bool checkPassWord(String password)= 0;
	virtual bool getTocken(AsyncWebServerRequest *server)= 0;
	virtual bool setupInfoDevice(AsyncWebServerRequest *server)= 0;
	virtual bool setTocken(AsyncWebServerRequest *server)= 0;
	virtual bool setup()= 0;

};


