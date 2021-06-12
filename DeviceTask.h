#pragma once

#include "ManagerControlAgle.h"
#include "MemoryItemAgle.h"

class DeviceTask
{
public:
	
	DeviceTask();
	DeviceTask(MemoryItemAgle *memory);
	~DeviceTask();
	unsigned int getValueTouch(void);
	unsigned char checkButton(void);
	void checkStatus(long time);
	void setupControlPin();
	
	
};

