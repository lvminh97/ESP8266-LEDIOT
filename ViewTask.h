#pragma once

#include "ManagerControlAgle.h"
#include "MemoryItemAgle.h"

class ViewTask
{
public:
	
	ViewTask();
	ViewTask(MemoryItemAgle *memory);
	~ViewTask();
	unsigned int getValueTouch(void);
	unsigned char checkButton(void);
	void checkStatus(long time);
	void setupControlPin();
	
	
};

