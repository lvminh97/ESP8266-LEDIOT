#include"LogDebugAgle.h"
#include "DeviceTask.h"




DeviceTask::DeviceTask()
{
  logDebugLV2("START:DeviceTask::DeviceTask"); 
  logDebugLV2("End:DeviceTask::DeviceTask"); 
}
DeviceTask::DeviceTask(MemoryItemAgle *memory)
{
  logDebugLV2("START:DeviceTask::DeviceTask"); 
	memoryControl = memory;
  logDebugLV2("End:DeviceTask::DeviceTask"); 
}

DeviceTask::~DeviceTask()
{
  logDebugLV2("START:DeviceTask::~DeviceTask"); 
  logDebugLV2("End:DeviceTask::~DeviceTask"); 
}
unsigned int DeviceTask::getValueTouch() {
  logDebugLV2("START:unsigned int DeviceTask::getValueTouch"); 
  logDebugLV2("End:unsigned int DeviceTask::getValueTouch"); 
	return touchRead(TOUCH_BUTTON);
}
/*****************************************************************************
function:  checkButton
description : check event button
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
unsigned char DeviceTask::checkButton(void) {
  logDebugLV2("START:unsigned char DeviceTask::checkButton"); 
	static unsigned char buttonValue=0;
	buttonAction = getValueTouch();
	if (buttonAction < buttonMin) {

		buttonMin = buttonAction;
		buttonCenter = (buttonMax + buttonMin) / 2;

		return 0;
	}
	if (buttonAction > buttonMax) {

		buttonMax = buttonAction;
		buttonCenter = (buttonMax + buttonMin) / 2;

		return 0;
	}

	if (buttonAction > buttonCenter) {
		buttonValue++;
		if(buttonValue>40) buttonValue=40;

	}
	else
	{
		buttonValue=0;
	}
	
  logDebugLV2("End:					{"); 
	return buttonValue;
}
/*****************************************************************************
function:  checkStatus
description : check status for action Sleep
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
void DeviceTask::checkStatus(long time) {
  logDebugLV2("START:void DeviceTask::checkStatus"); 

  logDebugLV2("End:	DeviceTask::checkStatus"); 
}


/*****************************************************************************
function:  setupControlPin
description : setup Port
Create date : 7/11/2017
user cread : Lu Van Cuong
******************************************************************************/
void DeviceTask::setupControlPin()
{
  logDebugLV2("START:void DeviceTask::setupControlPin"); 
	////Serial.println("Start setupControlPin ");
	/*pinMode(MASTER_DEVICE_PUMP1, OUTPUT);
	pinMode(MASTER_DEVICE_PUMP2, OUTPUT);
	pinMode(MASTER_DEVICE_PUMP3, OUTPUT);
	pinMode(MASTER_DEVICE_PUMP4, OUTPUT);
	pinMode(PIN_LED_WIFI, OUTPUT);
	digitalWrite(MASTER_DEVICE_PUMP1, HIGH);
	digitalWrite(MASTER_DEVICE_PUMP2, HIGH);
	digitalWrite(MASTER_DEVICE_PUMP3, HIGH);
	digitalWrite(MASTER_DEVICE_PUMP4, HIGH);*/
  logDebugLV2("End:void DeviceTask::setupControlPin"); 
}


