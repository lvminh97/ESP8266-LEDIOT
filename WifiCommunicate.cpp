#include "LogDebugAgle.h" 
#include "CommunicateControlTask.h"
//#include <NTPtimeESP.h>








CommunicateControlTask::CommunicateControlTask(MemoryItemAgle *memory)
{
  logDebugLV1("START:CommunicateControlTask::CommunicateControlTask"); 
	
  logDebugLV1("End:CommunicateControlTask::CommunicateControlTask"); 
}


CommunicateControlTask::~CommunicateControlTask()
{
  logDebugLV1("START:CommunicateControlTask::~CommunicateControlTask"); 
  logDebugLV1("End:CommunicateControlTask::~CommunicateControlTask"); 
}


String CommunicateControlTask::IpAddress2String(const IPAddress& ipAddress)
{
  logDebugLV1("START:String CommunicateControlTask::IpAddress2String"); 
  logDebugLV1("End:String CommunicateControlTask::IpAddress2String"); 
	return String(ipAddress[0]) + String(".") + \
		String(ipAddress[1]) + String(".") + \
		String(ipAddress[2]) + String(".") + \
		String(ipAddress[3]);
}
void CommunicateControlTask::loopEsp32() {
  logDebugLV1("START:void CommunicateControlTask::loopEsp32"); 
	
  logDebugLV1("End:void CommunicateControlTask::loopEsp32"); 
}


/*****************************************************************************
function:  getTimeFromServer
description :get time from internet
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::getTimeFromServer(MemoryItemAgle &datal) {
  logDebugLV1("START:bool CommunicateControlTask::getTimeFromServer"); 
	int i = 0;
	////Serial.println("Start getTimeFromServer");
	configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
	while (!time(nullptr)) {

		delay(1000);
		i++;

		if (i > 60) return false;
	}
	time_t now = time(nullptr);
	struct tm * timeinfo;
	ctime(&now);
	timeinfo = localtime(&now);
	datal.m_RTC.day = timeinfo->tm_wday;
	datal.m_RTC.hour = timeinfo->tm_hour;
	datal.m_RTC.minuth = timeinfo->tm_min;
	datal.m_RTC.second = timeinfo->tm_sec + i / 2;
	datal.m_RTC.set_start=millis();
  logDebugLV1("End:	while "); 
	return true;
}

/*****************************************************************************
function:  responseSetup
description : change data user in to EEFPROM if password is true
Create date : 4/6/2018
user cread : Lu Van Cuong
******************************************************************************/
/*****************************************************************************
function:  responseSetup
description : change data user in to EEFPROM if password is true
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseSetup(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseSetup"); 
	memoryControl->usernameHostPost = server->arg(VALUE_NAME_HOSTPOST);
	memoryControl->passwordHosPost = server->arg(VALUE_PASS_HOSTPOST);
	memoryControl->usernameWifi = server->arg(VALUE_WIFI);
	memoryControl->passwordWifi = server->arg(VALUE_PASS_WIFI);
	memoryControl->IP_Client = server->arg(VALUE_IP);
	String stroldPassHospost = server->arg(VALUE_OLD_PASS_HOSTPOST);
	int i = 0;
	bool state = false;

	if (memoryControl->checkDataInput(stroldPassHospost)) {

		memoryControl->getHandleRoot();
		state = true;

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
	if (state) {

		memoryControl->saveAllSetup();
		//wifiConnect();
		//reconnectWifi(); 

	}
  logDebugLV1("End:	if "); 
	return state;
}




bool CommunicateControlTask::getTockenSetup(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::getTockenSetup"); 
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	int i = 0;
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		memoryControl->getHandleRoot();
		state = true;

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}
bool CommunicateControlTask::updateDataInfoAlarm(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::updateDataInfoAlarm"); 
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		memoryControl->getCompressAllInfoAlarm();
		state = true;

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);

	return state;
  logDebugLV1("End:	else"); 
	return true;
}


/*****************************************************************************
function:  responseSetupTimeControl
description : setuptime Control for auutomatic pump
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/

bool CommunicateControlTask::responseSetupTimeControl(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseSetupTimeControl"); 
	unsigned long curent = millis();
	int hour = server->arg(VALUE_HOUR).toInt();
	int minuth = server->arg(VALUE_MINUTH).toInt();
	unsigned char synTime[4];
	String nameAlarm = server->arg(VALUE_NAME);
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	int i;
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->m_RTC.day = server->arg(VALUE_DAY_UPDATE).toInt();
		memoryControl->m_RTC.hour = server->arg(VALUE_HOUR_UPDATE).toInt();
		memoryControl->m_RTC.minuth = server->arg(VALUE_MINUTH_UPDATE).toInt();
		memoryControl->m_RTC.second = server->arg(VALUE_SECOND_UPDATE).toInt();

		memoryControl->timerSetupRaw.hour = hour;
		memoryControl->timerSetupRaw.minuth = minuth;
		memoryControl->timerSetupRaw.ID = server->arg(VALUE_TIME_ID).toInt();
		//Serial.print("Start responseSetupTimeControl is");Serial.println(String(timerSetup.ID));
		if (memoryControl->timerSetupRaw.ID<MAX_NUMBER_ALARM) {

			//Serial.println(nameAlarm);
			for (i = 0; i<nameAlarm.length(); i++) {

				if (i >= LENG_NAME_DATA_ALARM) break;
				memoryControl->timerSetupRaw.name[i] = nameAlarm.charAt(i);

			}
			i = memoryControl->timerSetupRaw.ID*(sizeof(Alarm_Wifi_Setup) - 1);

			if (server->arg(VALUE_SUN) == VALUE_ON) memoryControl->timerSetupRaw.Sun = ON; else memoryControl->timerSetupRaw.Sun = OFF;
			if (server->arg(VALUE_MON) == VALUE_ON) memoryControl->timerSetupRaw.Mon = ON; else memoryControl->timerSetupRaw.Mon = OFF;
			if (server->arg(VALUE_TUS) == VALUE_ON) memoryControl->timerSetupRaw.Tus = ON; else memoryControl->timerSetupRaw.Tus = OFF;
			if (server->arg(VALUE_WEN) == VALUE_ON) memoryControl->timerSetupRaw.Wen = ON; else memoryControl->timerSetupRaw.Wen = OFF;
			if (server->arg(VALUE_THUS) == VALUE_ON) memoryControl->timerSetupRaw.Thu = ON; else memoryControl->timerSetupRaw.Thu = OFF;
			if (server->arg(VALUE_FRI) == VALUE_ON) memoryControl->timerSetupRaw.Fri = ON; else memoryControl->timerSetupRaw.Fri = OFF;
			if (server->arg(VALUE_SAT) == VALUE_ON) memoryControl->timerSetupRaw.Sat = ON; else memoryControl->timerSetupRaw.Sat = OFF;
			memoryControl->timerSetupRaw.state = server->arg(VALUE_AUTOMATIC).toInt();
			memoryControl->timerSetupRaw.time_for_pump = server->arg(VALUE_TIME_FOR_PUMP).toInt();
			/*
			Serial.print("Start i is");Serial.println(String(i));
			Serial.print("Start hour is");Serial.println(String(timerSetup.hour));
			Serial.print("Start minuth is");Serial.println(String(timerSetup.minuth));
			Serial.print("Start 1 is");Serial.println(String(timerSetup.Mon));
			Serial.print("Start 2 is");Serial.println(String(timerSetup.Tus));
			Serial.print("Start 3 is");Serial.println(String(timerSetup.Wen));
			Serial.print("Start 4 is");Serial.println(String(timerSetup.Thu));
			Serial.print("Start 5 is");Serial.println(String(timerSetup.Fri));
			Serial.print("Start 6 is");Serial.println(String(timerSetup.Sat));
			Serial.print("Start timeis");Serial.println(String(timerSetup.state));
			Serial.print("Start timeis time_for_pump");Serial.println(String(timerSetup.time_for_pump));*/
			memoryControl->saveDataTimerToEEprom();
			if (memoryControl->m_stateControl.humanControl) memoryControl->updateDataState(NOT_CONTROL);
			else memoryControl->updateDataState(DATA_OK);

		}
		else
		{

			memoryControl->updateDataState(DATA_FALSE);
			state = false;

		}

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}

/*****************************************************************************
function:  responseTurnOnPump
description : turn on pump command
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseTurnOnPump(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseTurnOnPump"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;

		if (memoryControl->m_stateControl.humanControl)
			memoryControl->updateDataState(NOT_CONTROL);
		else
		{

			memoryControl->m_stateMachine.pump = TURN_ON;
			memoryControl->updateDataState(DATA_OK);

		}

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}
/*****************************************************************************
function:  responseTurnOffPump
description : turn off pump command
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseTurnOffPump(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseTurnOffPump"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		if (memoryControl->m_stateControl.humanControl)
			memoryControl->updateDataState(NOT_CONTROL);
		else
		{

			memoryControl->m_stateMachine.pump = TURN_OFF;
			memoryControl->updateDataState(DATA_OK);

		}

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}


/*****************************************************************************
function:  responseTurnOnPump
description : turn on pump command
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
unsigned int CommunicateControlTask::responseTurnOnOffPump(AsyncWebServerRequest *server) {
  logDebugLV1("START:unsigned int CommunicateControlTask::responseTurnOnOffPump"); 
	int i;
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	unsigned int  infoControl = server->arg(VALUE_CONTROL).toInt();
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		if (memoryControl->m_stateControl.humanControl)
			memoryControl->updateDataState(NOT_CONTROL);
		else
		{

			memoryControl->m_stateMachine.pump = infoControl;
			memoryControl->updateDataState(DATA_OK);

		}

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return memoryControl->m_stateMachine.pump;
}

/*****************************************************************************
function:  responseCalibrateDeviceLevel1
description : calibrateSensor
Create date : 6/12/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseCalibrateDeviceLevel1(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseCalibrateDeviceLevel1"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->updateDataState(DATA_OK);

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}

/*****************************************************************************
function:  responseCalibrateDeviceHightLevel
description : calibrateSensor
Create date : 28/12/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseCalibrateDeviceHightLevel(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseCalibrateDeviceHightLevel"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	unsigned char dataUpdate = server->arg(VALUE_DATA).toInt();

	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->updateDataState(DATA_OK);
		memoryControl->calibrateWaterSensor(ADRESS_SETUP_WATER_CALIBRATE_WATER, dataUpdate);

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}
/*****************************************************************************
function:  responseCalibrateDeviceLowLevel
description : calibrateSensor
Create date : 28/12/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::responseCalibrateDeviceLowLevel(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::responseCalibrateDeviceLowLevel"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	unsigned char dataUpdate = server->arg(VALUE_DATA).toInt();

	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->updateDataState(DATA_OK);
		memoryControl->calibrateWaterSensor(ADRESS_SETUP_WATER_CALIBRATE_NO_WATER, dataUpdate);

	}
	else
	{

		memoryControl->updateDataState(DATA_FALSE);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}

/*****************************************************************************
function:  updateDataUser
description : update info Mobile phone
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::updateDataUser(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::updateDataUser"); 

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->updateDataState(DATA_OK);

	}
	else
		memoryControl->updateDataState(NOT_ACCESS);
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	if "); 
	return state;
}
/*****************************************************************************
function:  updateTimeToUser
description : update time from Mobile phone
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::updateTimeToUser(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::updateTimeToUser"); 
	int i;

	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		memoryControl->m_RTC.day = server->arg(VALUE_DAY_UPDATE).toInt();
		memoryControl->m_RTC.hour = server->arg(VALUE_HOUR_UPDATE).toInt();
		memoryControl->m_RTC.minuth = server->arg(VALUE_MINUTH_UPDATE).toInt();
		memoryControl->m_RTC.second = server->arg(VALUE_SECOND_UPDATE).toInt();
		memoryControl->updateDataState(DATA_OK);

	}
	else
	{

		memoryControl->updateDataState(DATA_FALSE);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}

/*****************************************************************************
function:  updateDataTocken
description : update info Device by json data
Create date : 7/12/2017
user cread : Lu Van Cuong
******************************************************************************/
bool CommunicateControlTask::updateDataTocken(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::updateDataTocken"); 
	String strUserNameHostPost = server->arg(VALUE_DATA_TOCKEN);
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	int i = 0;
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		memoryControl->getHandleRoot();
		state = true;

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
	if (state) {

		
		memoryControl->saveDataStringToEEprom(strUserNameHostPost, ADRESS_USER_SETUP_TOCKEN_DATA);
	

	}
  logDebugLV1("End:	if "); 
	return state;
}


bool CommunicateControlTask::otherControlManager(AsyncWebServerRequest *server) {
  logDebugLV1("START:bool CommunicateControlTask::otherControlManager"); 
	String stroldPassHospost = server->arg(VALUE_PASS_HOSTPOST);
	String Data = server->arg(VALUE_HTTP_MQTT);
	bool state = false;
	if (memoryControl->checkDataInput(stroldPassHospost)) {

		state = true;
		JsonObject& root = jsonBufferWifi.parseObject(Data);
		if (root.containsKey("co"))
		{

			int Control = root["co"];
			// Start CuongLV 23-1-2018 add event update state device
			if (Control == 4) {

				//responseMQTT_StateDevice(root, control, event);

			}
			if (Control == 6) {

				//readMQTT_AlarmDevice(root, control, event);

			}
			if (Control == 7) {

				//responseSetupMaxTimeControl(root, control, event);

			}

			// End CuongLV 23-1-2018 add event update state device 

		}

	}
	else
	{

		memoryControl->updateDataState(NOT_ACCESS);

	}
	server->send(HTTP_SUCCESS, "text/html", memoryControl->messageInfo);
  logDebugLV1("End:	else"); 
	return state;
}

