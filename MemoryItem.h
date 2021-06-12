#pragma once

#include"DefineModelAgleStruct.h"
#include <String.h>
#include <WString.h>

class MemoryItem
{
public:
	// FLAG OF sTATE OF SYSTEM
	WM_RTC g_RTC = { 0,0,0,0 };
	WM_RTC m_RTC = { 0,0,0,0 };
	ControlAllAcessDataUart dataUart;
	StateMachine m_stateMachine;
	unsigned int mqtt_port = 18548; //Port của CloudMQTT
	unsigned char m_dataRecive[16] = { 0 };
	int gateway[4] = { 192, 168, 0, 1 }; // set gateway to match your network
	int subnet[4] = { 255, 255, 255, 0 }; // set subnet mask to match your
	ControlAllAcess dataControl;
	String Tocken;
	String mPassword;
	String userWifi,passwordWifi,pasworWifi;
	String userhost,pasworhost,usernameHostPost,passwordHosPost;
	String idtoppic,toppic_pub,toppic_sub,username_toppic,password_topic,link_toppic;
	int port;
	String mesage;

	
public:
	MemoryItem();
	~MemoryItem();
	bool checkClientAction();
	void getAllSetup();
	void checkTimeRTC();
	void getAllDataUserFromEEPROM();
	void setLedStatus(unsigned char status);
	void setFlagConnectionWifi(bool flag);
	bool getFlagConnectionWifi(void);
	void setFlagStateError(bool flag);
	void getAllControl(void);
	long compaireWithCurrentTimeStamp(unsigned char second,
	unsigned minuth, unsigned hour, unsigned day);
};

