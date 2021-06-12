#include "MemoryItem.h"
#include <EEPROM.h>
#include "LogDebugAgle.h" 

MemoryItem::MemoryItem()
{
}


MemoryItem::~MemoryItem()
{
}

bool MemoryItem::checkClientAction(){
	
	if (!EEPROM.begin(1024))
	{
		Serial.println("failed to initialise EEPROM");
	}	
	
	if(EEPROM.read(1000)==1){
		return true;
	}
	return false;
}


void MemoryItem::getAllSetup() {
	getAllDataUserFromEEPROM();
	
}


void MemoryItem::checkTimeRTC() {
	unsigned long current;
	unsigned long rtc=millis();
	//unsigned long currentTimeStame;
	unsigned int raw;
	if (m_RTC.set_start > rtc) {
		current = 0xffffffff - m_RTC.set_start + rtc;
		m_RTC.set_start = rtc;
		current = current / 1000;
		raw = current / 60;
		//m_RTC.timestamp+=current;
		m_RTC.second = (m_RTC.second + current)%60;
		// fixbug Timer
		raw = (m_RTC.second + current)/60+m_RTC.minuth;
		m_RTC.minuth = raw%60;
		raw =raw/60+ m_RTC.hour;
		m_RTC.hour =raw % 24;
		m_RTC.day = (m_RTC.day + (raw / 24)) % 7;
		current=0;
	}
	else
	{
		current=rtc-m_RTC.set_start;
		//currentTimeStame=(rtc-m_RTC.set_start)/1000;
	}
	current = current / 1000;
	//g_RTC.timestamp=m_RTC.timestamp+currentTimeStame;
	raw = current / 60;
	g_RTC.second = (m_RTC.second + current)%60;
	raw = (m_RTC.second + current)/60+m_RTC.minuth;
	g_RTC.minuth = raw % 60;
	raw = raw / 60+ m_RTC.hour;
	g_RTC.hour = raw % 24;
	g_RTC.day = (m_RTC.day + (raw / 24)) % 7;
	
	g_RTC.timestamp=m_RTC.timestamp+g_RTC.second;
	
	
	if(g_RTC.rawcheckMinuthTime!=g_RTC.minuth){
		g_RTC.rawcheckMinuthTime=g_RTC.minuth;
		if(g_RTC.rawcheckMinuthTime>g_RTC.rawMinuthTime)
			raw=g_RTC.rawcheckMinuthTime-g_RTC.rawMinuthTime;
		else
			raw=60+g_RTC.rawcheckMinuthTime-g_RTC.rawMinuthTime;	
		g_RTC.rawMinuthTime=g_RTC.minuth;
		m_RTC.timestamp=m_RTC.timestamp+60*raw;
		g_RTC.timestamp=m_RTC.timestamp+g_RTC.second;
		
	}
}

void MemoryItem::getAllDataUserFromEEPROM() {
	char x;
	
	String PORT;

	
	int i = ADRESS_USER_NAME_WIFI;
	mPassword = "";
	userWifi = "";
	
	
	passwordWifi = "";
	PORT="";
	
	logDebugLV2("getAllDataUserFromEEPROM");
	
	if (!EEPROM.begin(1024))
	{
		Serial.println("failed to initialise EEPROM");
	}	
	
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE))  break;
		userWifi += x;
	} while (x);
	
	i = ADRESS_USER_PASS_WIFI;
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE)) break;
		passwordWifi += x;
	} while (x);
	
	i = ADRESS_USER_PASS;
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE))  break;
		mPassword += x;
	} while (x);


	idtoppic= "";
	toppic_pub= "";
	toppic_sub= "";
	username_toppic= "";
	password_topic= "";
	link_toppic= "";
	i = ADRESS_USER_TOCKEN;
	int j = 0;
	port = 18548;
	do {
		x = char(EEPROM.read(i));
		i++;
		if (i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE)) break;
		if (x == '$') j++;
		else
		{
			if (j == 0) link_toppic += x;
			if (j == 1) username_toppic += x;
			if (j == 2) password_topic += x;
			if (j == 3) toppic_pub += x;
			if (j == 4) toppic_sub += x;
			if (j == 5) PORT += x;
			if (j == 6) idtoppic += x;

		}
		if (i>1000) break;
	} while (x);
	if (PORT != "")
		port = PORT.toInt();

	userhost = "Agle";
	usernameHostPost="Agle";
	passwordHosPost="password";
	pasworhost = "password";
	
	
	
	 i = ADRESS_USER_NAME_WIFI;
	mPassword = "";
	userWifi = "";
	pasworWifi = "";
	passwordWifi = "";
	
	
	
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE))  break;
		userWifi += x;
	} while (x);
	
	i = ADRESS_USER_PASS_WIFI;
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE))  break;
		pasworWifi += x;
	} while (x);
	
	i = ADRESS_USER_PASS;
	do {
		x = char(EEPROM.read(i));
		i++;
		if(i>1022) break;
		if ((x == NULL_VALUE) || (x == FULL_VALUE))  break;
		mPassword += x;
	} while (x);
	if(mPassword=="") mPassword="password";
	EEPROM.end();
	logDebugLV2(mPassword );
	logDebugLV2(userWifi );
	logDebugLV2(pasworWifi );
	logDebugLV2(idtoppic);
	logDebugLV2(toppic_pub);
	logDebugLV2(toppic_sub);
	logDebugLV2(username_toppic);
	logDebugLV2(password_topic);
	logDebugLV2(link_toppic);
	logDebugLV2(passwordWifi );
}
void MemoryItem::setLedStatus(unsigned char status) {
	m_stateMachine.LEDStatus = status;
}
void MemoryItem::setFlagConnectionWifi(bool flag) {
	m_stateMachine.flagConnect = flag;
}
bool MemoryItem::getFlagConnectionWifi(void) {
	return m_stateMachine.flagConnect;
}

void MemoryItem::setFlagStateError(bool flag) {
	m_stateMachine.flagError = flag;
}

void MemoryItem::getAllControl(void) {
	int readAdress=ADRESS_ROM_CONTROL_SAVE;
	unsigned int j,dataSave,i;
	EEPROM.begin(1024);
	j=0;
	dataSave=0;
	for (i = 0; i<8; i++) {
		j=EEPROM.read(i+ADRESS_ROM_CONTROL_SAVE);
		dataSave=dataSave<<1;
		if(j==1){
			dataSave=dataSave|1;
		}
		logDebugLV1(String(EEPROM.read(i+ADRESS_ROM_CONTROL_SAVE)));
		
	}
}

long MemoryItem::compaireWithCurrentTimeStamp(unsigned char second, unsigned minuth, unsigned hour, unsigned day) {
	long time_check = 0;
	/*//Serial.print("g_RTC.second "); ////Serial.println(String(g_RTC.second));
	Serial.print("g_RTC.minuth "); ////Serial.println(String(g_RTC.minuth));
	Serial.print("g_RTC.day "); ////Serial.println(String(g_RTC.day));*/
	if ((g_RTC.day == 0)&(day == 6)) {
		time_check = (g_RTC.second + g_RTC.minuth * 60 + (g_RTC.hour + 24) * 3600) - (second + minuth * 60 + hour * 3600);
	}
	else
	{
		time_check = (g_RTC.second + g_RTC.minuth * 60 + (g_RTC.hour + g_RTC.day * 24) * 3600) - (second + minuth * 60 + (hour + day * 24) * 3600);
	}

	return time_check;
}

