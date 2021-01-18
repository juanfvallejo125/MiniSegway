/*
 * ISRs.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Vallejo
 */

#include "MiniSegway.h"

// Interrupt handlers for encoders
extern "C" void PORT4_IRQHandler(){
    // Right Encoder Interrupt triggered
    if(GPIO_getInputPinValue(rightMotor.enc_portB, rightMotor.enc_pinB) == 1){
        rightMotor.enc_count++;
    //        rightMotor.enc_velocity = 1000.0/(ms-rightMotor.prev_tick);
    }
    else{
        rightMotor.enc_count--;
    //        rightMotor.enc_velocity = -1000.0/(ms-rightMotor.prev_tick);
    }
    //    rightMotor.prev_tick = ms;

    GPIO_clearInterruptFlag(rightMotor.enc_portA, rightMotor.enc_pinA);

}

extern "C" void PORT5_IRQHandler(){
    // Left Encoder Interrupt triggered
    if(GPIO_getInputPinValue(leftMotor.enc_portB, leftMotor.enc_pinB) == 0){
        leftMotor.enc_count++;
//        leftMotor.enc_velocity = 1000.0/(ms-leftMotor.prev_tick);
    }
    else{
        leftMotor.enc_count--;
//        leftMotor.enc_velocity = -1000.0/(ms-leftMotor.prev_tick);
    }
//    leftMotor.prev_tick = ms;
    GPIO_clearInterruptFlag(leftMotor.enc_portA, leftMotor.enc_pinA);
}

extern "C" void SysTick_Handler()
{
    ms++;
    if(ms%100 == 0){
        ten_hz = true;
    }
}

extern "C" void EUSCIA2_IRQHandler(void){
    // Transmit the data until the buffer is dry
    if(UART_getEnabledInterruptStatus(UARTHandler.UARTModule) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG){
        UART_transmitData(UARTHandler.UARTModule, UARTHandler.ringBufferTX[UARTHandler.bufferTXTail]);
        UARTHandler.bufferTXTail++;
        if(UARTHandler.bufferTXTail == TX_BUFFER_SIZE){
            UARTHandler.bufferTXTail = 0;
        }
        if(UARTHandler.bufferTXTail == UARTHandler.bufferTXHead){
            UART_disableInterrupt(UARTHandler.UARTModule, EUSCI_A_UART_TRANSMIT_INTERRUPT); // Disable the transmit interrupt
        }
//        if(UARTHandler.bufferIndex<UARTHandler.messageSize){
//            UART_transmitData(UARTHandler.UARTModule, UARTHandler.buffer[UARTHandler.bufferIndex]);
//            UARTHandler.bufferIndex++;
//        }else{
//            UARTHandler.bufferIndex = 0;
//            UART_disableInterrupt(UARTHandler.UARTModule, EUSCI_A_UART_TRANSMIT_INTERRUPT); // Disable the transmit interrupt
//            UARTHandler.finishedTransmission=1;
//        }
    }else if(UART_getEnabledInterruptStatus(UARTHandler.UARTModule) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        // Read the data
        uint8_t data = UART_receiveData(UARTHandler.UARTModule);
        // Add it to the buffer if we aren't overwriting older data
        if(UARTHandler.bufferRXTail != UARTHandler.bufferRXHead+1){
            UARTHandler.ringBufferRX[UARTHandler.bufferRXHead] = data;
            if(UARTHandler.bufferRXHead >= (RX_BUFFER_SIZE-1)) UARTHandler.bufferRXHead = 0;
            else{
                UARTHandler.bufferRXHead++;
            }
        }
    }
}

extern "C" void TA0_N_IRQHandler(void)// Capture pulse lengths of RC channels
{
    int vector = TIMER_A0->IV;
    int pulseIndex = 0;
    int pulseArrIndex = 0;
    if(vector == 0x02){
        pulseIndex = 1;
        pulseArrIndex = 2;
        if(TIMER_A0->CCTL[pulseIndex] & TIMER_A_CCTLN_CCI){
              commandInterface.pulseStart[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
          }
          else{
              commandInterface.pulseEnd[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
              commandInterface.pulseLength[pulseArrIndex] = commandInterface.pulseEnd[pulseArrIndex] - commandInterface.pulseStart[pulseArrIndex];
              TIMER_A0->CCTL[pulseIndex] &= ~(TIMER_A_CCTLN_CCIFG);
          }
    }
    if(vector == 0x06){
        pulseIndex = 3;
        pulseArrIndex = 1;
        if(TIMER_A0->CCTL[pulseIndex] & TIMER_A_CCTLN_CCI){
            commandInterface.pulseStart[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
          }
          else{
              commandInterface.pulseEnd[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
              commandInterface.pulseLength[pulseArrIndex] = commandInterface.pulseEnd[pulseArrIndex] - commandInterface.pulseStart[pulseArrIndex];
              TIMER_A0->CCTL[pulseIndex] &= ~(TIMER_A_CCTLN_CCIFG);
          }
    }
    if(vector == 0x08){
        pulseIndex = 4;
        pulseArrIndex = 0;
        if(TIMER_A0->CCTL[pulseIndex] & TIMER_A_CCTLN_CCI){
            commandInterface.pulseStart[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
          }
          else{
              commandInterface.pulseEnd[pulseArrIndex] = TIMER_A0->CCR[pulseIndex];
              commandInterface.pulseLength[pulseArrIndex] = commandInterface.pulseEnd[pulseArrIndex] - commandInterface.pulseStart[pulseArrIndex];
              TIMER_A0->CCTL[pulseIndex] &= ~(TIMER_A_CCTLN_CCIFG);
          }
    }
}



