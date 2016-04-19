//******************************************************************************
//*
//*     FULLNAME:  Single-Chip Microcontroller Real-Time Operating System
//*
//*     NICKNAME:  scmRTOS
//*
//*     PROCESSOR: ADSP-BF533 (Analog Devices)
//*
//*     TOOLKIT:   VDSP (Analog Devices)
//*
//*     PURPOSE:   Port Test File (low-level stuff for terminal support)
//*
//*     Version: v5.1.0
//*
//*
//*     Copyright (c) 2003-2016, scmRTOS Team
//*
//*     Permission is hereby granted, free of charge, to any person 
//*     obtaining  a copy of this software and associated documentation 
//*     files (the "Software"), to deal in the Software without restriction, 
//*     including without limitation the rights to use, copy, modify, merge, 
//*     publish, distribute, sublicense, and/or sell copies of the Software, 
//*     and to permit persons to whom the Software is furnished to do so, 
//*     subject to the following conditions:
//*
//*     The above copyright notice and this permission notice shall be included 
//*     in all copies or substantial portions of the Software.
//*
//*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//*     EXPRESS  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
//*     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//*     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
//*     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
//*     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
//*     THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*
//*     =================================================================
//*     Project sources: https://github.com/scmrtos/scmrtos
//*     Documentation:   https://github.com/scmrtos/scmrtos/wiki/Documentation
//*     Wiki:            https://github.com/scmrtos/scmrtos/wiki
//*     Sample projects: https://github.com/scmrtos/scmrtos-sample-projects
//*     =================================================================
//*
//******************************************************************************
//*     MSP430/IAR sample by Harry E. Zhurov, Copyright (c) 2012-2016


#include "uart_dev.h"

                                           
//---------------------------------------------------------------------------
char     RxBuf[RX_BUF_SIZE];
uint16_t RxIndex;

usr::ring_buffer<char, TX_BUF_SIZE, uint16_t> TxBuf;

uint16_t PRESCALER = 417; // 9600 bps @4MHz
uint8_t  MODULATOR = 0; 
//---------------------------------------------------------------------------
void UART::init()
{
    BCSCTL1 |= XTS;                   // turn on High-Speed part
    U0CTL    = CHAR;                  // 8 bit
    U0TCTL  |= SSEL0;                 // ACLK
    U0BR0    = PRESCALER;             
    U0BR1    = PRESCALER >> 8;        
    U0MCTL   = MODULATOR;             
                                      
    U0RCTL   = URXEIE;                
    ME1     |= UTXE0  + URXE0;        // Enable USART0 TXD/RXD
    IE1     |= URXIE0;                // Enable USART0 RX interrupt
    P3SEL   |= (1 << 4) | (1 << 5);   // P3.4,5 = USART0 TXD/RXD
    P3DIR   |= (1 << 4);              // P3.4 output direction

}
//---------------------------------------------------------------------------
void UART::send(const char c)
{
    U0TXBUF = c;
}
//---------------------------------------------------------------------------
void UART::send(const char* s)
{
    uint16_t i = 0;
    while(s[i])
    {
        TxBuf.push(s[i++]);
    }
    enable_tx_int();

}
//---------------------------------------------------------------------------
//void UART::send(const uint8_t *data, const uint16_t count)
//{
//    for(uint16_t i = 0; i < count; i++)
//    {
//        if( TxBuf.get_count() )
//        {
//            TxBuf.push(data[i]);
//        }
//    }
//
//    MMR16(UART_IER)  |= ETBEI;
//}
//---------------------------------------------------------------------------
#pragma vector = USART0RX_VECTOR
OS_INTERRUPT void character_incoming_isr()
{
    OS::TISRW ISRW;
    uint8_t data = U0RXBUF;                // read data and clear flag

    switch(data)
    {
    case 27:                               // Esc - kill line
        UART::send("\\\r\n");
        RxIndex = 0;
        break;
    case '\n':
    break;
    case '\b':                             // backspace
        if(RxIndex)
        {
            --RxIndex;
            UART::send("\b \b");           // erase char
        }
        else
        {
            UART::send('\a');              // beep instead of echo
        }
        break;
    default:
        RxBuf[RxIndex++] = data;           // place char to buffer
        UART::send(data);                  // echo
        if(RxIndex < sizeof(RxBuf) - 1)
        break;                             // else buffer overflow
    case '\r':                             // CR - execute
        RxBuf[RxIndex] = 0;
        RxIndex = 0;
        NewLineIncoming.signal_isr();
    }       
}
//---------------------------------------------------------------------------
#pragma vector = USART0TX_VECTOR
__interrupt void txbuf_empty_isr()
{
    U0TXBUF = TxBuf.pop();
    if(TxBuf.get_count() == 0) disable_tx_int();

//
//  if(TxBuf.get_count())
//  {
//      U1TXBUF = TxBuf.pop();
//  }
//  else
//  {
//      disable_tx_int();
//  }
}
//---------------------------------------------------------------------------

