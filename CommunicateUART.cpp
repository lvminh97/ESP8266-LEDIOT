#include "CommunicateUART.h"
#include "LogDebugAgle.h"


//static uart_isr_handle_t *handle_console;
static intr_handle_t handle_console;
static int lengData=0;
static unsigned char rxbuf[512];
static void IRAM_ATTR uart_intr_handle(void *arg)
{
  uint16_t rx_fifo_len, status;
  uint16_t i;
  
  status = UART2.int_st.val; // read UART interrupt Status

  rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer
  
  while(rx_fifo_len){
	  rxbuf[lengData] = UART2.fifo.rw_byte; // read all bytes
	  rx_fifo_len--;
	  lengData++;
  }
  
  // after reading bytes from buffer clear UART interrupt status
  uart_clear_intr_status(UART_NUM_2, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

}


CommunicateUART::CommunicateUART()
{
	//memset(data, 0, BUF_SIZE);
}


CommunicateUART::~CommunicateUART()
{
}




void CommunicateUART::setupCommunicate() {
	uart_num=UART_NUM_2;
	//uart_param_config(uart_num, &uart_config);
	ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
	//Set UART1 pins(TX: IO4, RX: I05, RTS: IO18, CTS: IO19)
	uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	//Install UART driver (we don't need an event queue here)
	//In this example we don't even use a buffer for sending data.
	uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
	// ESP_ERROR_CHECK(uart_set_pin(uart_num, &uart_config, UART_PIN_NO_CHANGE, 18, 19));
	
	// release the pre registered UART handler/subroutine
   ESP_ERROR_CHECK(uart_isr_free(uart_num));       

// register new UART ISR subroutine
  ESP_ERROR_CHECK(uart_isr_register(uart_num,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));
  //ESP_LOGI(TAG, "Return from UART ISR %X", ret);
  ESP_ERROR_CHECK(uart_enable_rx_intr(uart_num));

}
void CommunicateUART::setupCommunicate(MemoryItemAgle *mem){
	memoryControl=mem;
}
void CommunicateUART::anaslzysData() {
	int t=0;
	int k,l,lastdata;
	bool dataCheck=false;
	unsigned char crc=0;
	unsigned char *dataIndex;
	unsigned char *dataCoppy;
	//logDebugLV1("START:bool anaslzysData");
	//int len = uart_read_bytes(uart_num, &memoryControl->dataUart.DataRecive[0], BUF_SIZE, 20 / portTICK_RATE_MS);
	if (lengData>0) {
		logDebugLV1("START:bool EciveData"); 
		logDebugLV1(String(lengData));
		logDebugLV1("START:bool Data"); 
		l=0; lastdata=0;
		while(l<lengData){
			for(;l<lengData;l++){
				if(rxbuf[l]==0xac){ lastdata=l; break;} 
			}
			logDebugLV1("START:bool location"); 
		logDebugLV1(String(l));
			k=l+rxbuf[l+1];
			if(k<=lengData){
				crc=0xac;
				k--;
				for(t=l;t<k;t++){
					logDebugLV1(String(rxbuf[t])); 
					crc+=rxbuf[t];
				}
				logDebugLV1("START:finish crc"); 
		logDebugLV1(String(crc)); 
				k=l+rxbuf[l+1];
				if(rxbuf[t]==crc)
				{ 
			
			
					memoryControl->m_stateMachine.device=rxbuf[l+4];
					memoryControl->m_stateMachine.light=(rxbuf[l+5]<<24)|(rxbuf[l+6]<<16)|(rxbuf[l+7]<<8)|(rxbuf[l+8]);
					memoryControl->m_stateMachine.RGB=(rxbuf[l+9]<<24)|(rxbuf[l+10]<<16)|(rxbuf[l+11]<<8)|(rxbuf[l+12]);
					t=l+13;
					memoryControl->m_stateMachine.temperature=rxbuf[t];t++;
					memoryControl->m_stateMachine.no3=rxbuf[t];t++;
					memoryControl->m_stateMachine.verhicle=rxbuf[t];t++;
					memoryControl->m_stateMachine.pressure=rxbuf[t];t++;
					memoryControl->m_stateMachine.humidity=rxbuf[t];t++;
					memoryControl->m_stateMachine.pump=rxbuf[t];t++;
					memoryControl->m_stateMachine.other_device=rxbuf[t];t++;
					memoryControl->m_stateMachine.error=rxbuf[t];t++;
					memoryControl->m_stateMachine.LEDStatus=rxbuf[t];t++;
					memoryControl->m_stateMachine.waterCompaire=rxbuf[t];t++;
					memoryControl->m_stateMachine.flagConnect=rxbuf[t];t++;
					memoryControl->m_stateMachine.flagError=rxbuf[t];t++;

					
logDebugLV1(String(memoryControl->m_stateMachine.device));					
logDebugLV1(String(memoryControl->m_stateMachine.light));
logDebugLV1(String(memoryControl->m_stateMachine.RGB));
logDebugLV1(String(memoryControl->m_stateMachine.temperature));
logDebugLV1(String(memoryControl->m_stateMachine.no3));
logDebugLV1(String(memoryControl->m_stateMachine.verhicle));
logDebugLV1(String(memoryControl->m_stateMachine.pressure));
logDebugLV1(String(memoryControl->m_stateMachine.humidity));
logDebugLV1(String(memoryControl->m_stateMachine.pump));
logDebugLV1(String(memoryControl->m_stateMachine.other_device));
logDebugLV1(String(memoryControl->m_stateMachine.error));
logDebugLV1(String(memoryControl->m_stateMachine.LEDStatus));
logDebugLV1(String(memoryControl->m_stateMachine.waterCompaire));
logDebugLV1(String(memoryControl->m_stateMachine.flagConnect));
logDebugLV1(String(memoryControl->m_stateMachine.flagError));

		logDebugLV1(String(t)); 
				}
			}
			l+=rxbuf[l+1];
			
		}
		for(t=0;t<lengData;t++){
			rxbuf[t]=0;
		}
		lengData=0;
		logDebugLV1("finish:bool Data"); 
		
	}
	//uart_write_bytes(uart_num, "helo",4);
	if(memoryControl->dataUart.lengTx>0){
		uart_write_bytes(uart_num, (const char *)&memoryControl->dataUart.DataSend,
			memoryControl->dataUart.lengTx);
	}
}

void CommunicateUART::sendData(char *data,int leng) {

	uart_write_bytes(uart_num, (const char *)data, leng);
}

