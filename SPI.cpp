/*
 * SPI.cpp
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Valle
 */

#include "SPI.h"

void configSPI(){
    /* Pinout CS: 4.6, MISO: 6.5, MOSI: 6.4, SCK: 6.3 */
    P6->SEL0 |= BIT3 | BIT4 | BIT5;  // set 4-SPI pin as second function

    /* Selecting P4.6 as the CS pin     */
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put state machine in reset

    EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST | // Remain in reset state
            EUSCI_B_CTLW0_MST |             // SPI master
            EUSCI_B_CTLW0_SYNC |            // Synchronous mode
            //EUSCI_B_CTLW0_CKPH |            // Clock phase
            EUSCI_B_CTLW0_CKPL |            // Clock polarity low
            EUSCI_B_CTLW0_MSB |             // MSB first
            EUSCI_B_CTLW0_MODE_0 |          // 3-pin mode
            //EUSCI_B_CTLW0_STEM |            // STE mode select
            EUSCI_B_CTLW0_SSEL__SMCLK;       // SMCLK

    EUSCI_B1->BRW = 0x1E;                   // /30,fBitClock = fBRCLK/(UCBRx+1). fBitClock = 4e6/31 = 133kHz
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;// **Initialize USCI state machine**

    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
}

void SPI_Read(uint8_t byteCount, uint8_t* rx_buffer, uint8_t* tx_buffer){
    uint32_t byteCountRX;
    uint32_t read_dummy;
    for(byteCountRX=0; byteCountRX<byteCount; byteCountRX++){
        //Chip select low
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
        //Poll transmit flag
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG));
        /* Transmitting data to slave */
        EUSCI_B1->TXBUF = tx_buffer[byteCountRX];
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) || !(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG));
        read_dummy = EUSCI_B1->RXBUF;
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG));
        /* Transmitting data to slave */
        EUSCI_B1->TXBUF = tx_buffer[byteCountRX];
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) || !(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG));
        // Read needed byte
        rx_buffer[byteCountRX] = EUSCI_B1->RXBUF;
        //Chip select high
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
        __delay_cycles(30);
    }
}

void SPI_Write(uint8_t byteCount, uint8_t* buffer){
    uint32_t byteCountTX = 0;
    byteCount--;
    uint32_t read_dummy;
    while(byteCountTX<byteCount){
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
        /* Transmitting data to slave */
        EUSCI_B1->TXBUF = buffer[byteCountTX];
        /* Polling to see if the TX and RX buffer is ready */
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) || !(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG));
        read_dummy = EUSCI_B1->RXBUF; // Clear buffer
        EUSCI_B1->TXBUF = buffer[byteCountTX+1];
        /* Polling to see if the TX and RX buffer is ready */
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) || !(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG));
        read_dummy = EUSCI_B1->RXBUF; // Clear buffer
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
        __delay_cycles(30);
        byteCountTX++;
        byteCountTX++;
    }
}


