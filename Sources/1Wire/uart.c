/////////////////////////////////////////////////////////////////////////
///	\file uart.c
///	\brief Portable serial layer. 
///
///	\section UartLayer Serial Layer
///
///	Author: Ronald Alexander Nobrega De Sousa (Opticalworm)
///	Website: www.HashDefineElectronics.com
///
///	Licences:
///
///		Copyright (c) 2014 Ronald Alexander Nobrega De Sousa
///
///		Permission is hereby granted, free of charge, to any person obtaining a copy
///		of this software and associated documentation files (the "Software"), to deal
///		in the Software without restriction, including without limitation the rights
///		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///		copies of the Software, and to permit persons to whom the Software is
///		furnished to do so, subject to the following conditions:
///
///		The above copyright notice and this permission notice shall be included in
///		all copies or substantial portions of the Software.
///	
///		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
///		THE SOFTWARE.
/////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "common.h"
#include "fsl_uart_driver.h"
#include "fsl_clock_manager.h"

#define ONE_WIRE_UART 		1

static uart_user_config_t uartConfig;
static uart_state_t uartState;

extern void UART_DRV_IRQHandler(uint32_t instance);
void UART1_IRQHandler(void)
{
	UART_DRV_IRQHandler(ONE_WIRE_UART);
}

/////////////////////////////////////////////////////////////////////////
///	\brief	setup the uart hardware
///
///	\param baud the desire baudrate
/////////////////////////////////////////////////////////////////////////
void Uart_Init(uint32_t baud)
{
	uartConfig.baudRate = baud;
	uartConfig.bitCountPerChar = kUart8BitsPerChar;
	uartConfig.parityMode = kUartParityDisabled;
	uartConfig.stopBitCount = kUartOneStopBit;

	UART_DRV_Init(ONE_WIRE_UART, &uartState, &uartConfig);
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Set Uart baudrate. can be called at any time.
///
///	\param baud the desire baudrate
///
///	\note setting baudrate will effect any data currently been sent.
///		make sure that you check that the write buffer is empty
/////////////////////////////////////////////////////////////////////////
void Uart_Setbaud(uint32_t baud)
{
	UART_DRV_Deinit(ONE_WIRE_UART);
	uartConfig.baudRate = baud;
	UART_DRV_Init(ONE_WIRE_UART, &uartState, &uartConfig);
}

/////////////////////////////////////////////////////////////////////////
///	\brief	you can use this function to check if the write buffer is
///	empty and ready for new data
///
///	\param destination pointer to return the read byte
///	\return TRUE = empty and ready. else false
/////////////////////////////////////////////////////////////////////////
uint_fast8_t Uart_WriteBusy(void)
{
	uint_fast8_t ReturnState = TRUE;
	
	ReturnState = UART_HAL_GetStatusFlag(g_uartBase[ONE_WIRE_UART], kUartTxDataRegEmpty);
	
	return ReturnState;
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Uart write single byte
///
///	\param source byte to write
/////////////////////////////////////////////////////////////////////////
void Uart_WriteByte(uint8_t source)
{
	UART_DRV_SendDataBlocking(ONE_WIRE_UART, &source, 1, 10);
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Uart read single byte
///
///	\param destination pointer to return the read byte
///	\return FALSE on success else TRUE
/////////////////////////////////////////////////////////////////////////
uint_fast8_t Uart_ReadByte(uint8_t * destination)
{
	uint_fast8_t ReturnState = FALSE;

	if(UART_DRV_ReceiveDataBlocking(ONE_WIRE_UART, destination, 1, 10) != kStatus_UART_Success)
		ReturnState = TRUE;

	return ReturnState;
}
