#pragma once

#include "ManagerControlAgle.h"
#include "MemoryItemAgle.h"

class ManagerTask
{
public:
	
	ManagerTask();
	ManagerTask(MemoryItemAgle *memory);
	~ManagerTask();
	unsigned int getValueTouch(void);
	unsigned char checkButton(void);
	void checkStatus(long time);
	void setupControlPin();
	
	
};

