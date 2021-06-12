#pragma once
#include "MemoryItemAgle.h"


class CommunicateUART:public CommunicateUART
{

public:
	CommunicateUART();
	~CommunicateUART();
	void setupCommunicate();
	void setupCommunicate(MemoryItemAgle *mem);
	void anaslzysData();
	void sendData(char *data,int leng);
};

