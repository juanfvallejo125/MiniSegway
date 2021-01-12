/*
 * initConfigs.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Valle
 */

#include "initConfigs.h"

void configPWM() {
    // Pinout PWMA: 6.6, PWMB 6.7
    P6SEL0 |= 0b11<<6 ; // Set bit 6 and 7 of P6SEL0 to enable TA2.3 and TA2.4 functionality on P6.6, P6.7
    P6SEL1 &= ~0b11<<6 ; // Clear bit 6 and 7 of P6SEL1 to enable TA2.3 and TA2.4 functionality on P6.6, P6.7
    P6DIR |= 0b11<<6 ; // Set pins 6.6 and 6.7 as output pins
    // Set Timer A period equivalent to 17.77kHz (PWM signal period)
    TA2CCR0 = MAX_PWM;
    // Set Duty cycle
    TA2CCR3 = 0;
    TA2CCR4 = 0;
    // Set output mode to Reset/Set
    TA2CCTL3 = OUTMOD_7 ; // Macro which is equal to 0x00e0, defined in msp432p401r.h
    TA2CCTL4 = OUTMOD_7 ; // Macro which is equal to 0x00e0, defined in msp432p401r.h
    // Initialize Timer A
    TA2CTL = TASSEL__SMCLK | MC__UP | TACLR ; // Tie Timer A to SMCLK, use Up mode, and clear TA2R
    // PWM signal will now be available
}

void configTimerCapture() {
    // Pinout ch1: p2.7 ch2: p2.6 ch3: p2.4
    P2->SEL0 |= BIT7; // Set bit 5 of P2SEL0 to enable TA0.2 functionality on P2.5
    P2->DIR &= ~BIT7;
    P2->SEL0 |= BIT6; // Set bit 5 of P2SEL0 to enable TA0.2 functionality on P2.5
    P2->DIR &= ~BIT6;
    P2->SEL0 |= BIT4; // Set bit 5 of P2SEL0 to enable TA0.2 functionality on P2.5
    P2->DIR &= ~BIT4;

    // Timer0_A4 Setup
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_CM__BOTH | // Capture rising and falling edges,
            TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A,
            TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
            TIMER_A_CCTLN_CAP |             // Enable capture mode,
            TIMER_A_CCTLN_SCS;              // Synchronous capture

    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_CM__BOTH | // Capture rising and falling edges,
                TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A,
                TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
                TIMER_A_CCTLN_CAP |             // Enable capture mode,
                TIMER_A_CCTLN_SCS;              // Synchronous capture

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CM__BOTH | // Capture rising and falling edges,
                TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A,
                TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
                TIMER_A_CCTLN_CAP |             // Enable capture mode,
                TIMER_A_CCTLN_SCS;              // Synchronous capture

    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK | // Use SMCLK as clock source,
            TIMER_A_CTL_MC__CONTINUOUS |    // Start timer in continuous mode
            TIMER_A_CTL_CLR |               // clear TA0R
            TIMER_A_CTL_ID_2;               // Divide by 4 to tick every microsec

    Interrupt_setPriority(INT_TA0_N, 1);
    Interrupt_enableInterrupt(INT_TA0_N);


}

void setupClocks(){
    // Set the DCO to DCO_Freq MHZ and set the sub master clock
    CS_setDCOFrequency(DCO_Freq);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, 1);
    // Init the submaster clock
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8);
}

void setupEncoderInterrupts(){
    //Interrupt declarations
    Interrupt_disableMaster();
    GPIO_interruptEdgeSelect(RIGHT_ENC_PORT, RIGHT_ENC_PIN_A, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(LEFT_ENC_PORT, LEFT_ENC_PIN_A, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(RIGHT_ENC_PORT, RIGHT_ENC_PIN_A);
    GPIO_clearInterruptFlag(LEFT_ENC_PORT, LEFT_ENC_PIN_A);
    GPIO_enableInterrupt(RIGHT_ENC_PORT, RIGHT_ENC_PIN_A);
    GPIO_enableInterrupt(LEFT_ENC_PORT, LEFT_ENC_PIN_A);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
}

void setupPins(){
    // Pinout: D3: P6.0, D2: P3.2, D1: P3.3, D0: P4.1, BIN2: P5.2, AIN2: P3.6, AIN1: P3.7, BIN1: P3.5
    GPIO_setAsOutputPin(CCW_1_PORT, CW_1_PIN|CW_2_PIN|CCW_1_PIN);
    GPIO_setAsOutputPin(CCW_2_PORT, CCW_2_PIN);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_PIN2|GPIO_PIN3);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_PIN0);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_PIN1);
    // Encoder Pins as input R_PIN_A: 4.0, R_PIN_B: 4.2, L_PIN_A: 5.4, L_PIN_B: 4.7
    GPIO_setAsInputPinWithPullUpResistor(RIGHT_ENC_PORT,RIGHT_ENC_PIN_A | RIGHT_ENC_PIN_B);
    GPIO_setAsInputPinWithPullUpResistor(LEFT_ENC_PORT,LEFT_ENC_PIN_A);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, LEFT_ENC_PIN_B);
    //LED 1 to indicate once calibration is done
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    //Led 2 Blue p2.2 as indicator
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void setupSystick(){
    SysTick_enableModule();
    //Interrupt every ms
    SysTick_setPeriod(DCO_Freq/1000);
    //Enable Interrupts
    SysTick_enableInterrupt();
}


