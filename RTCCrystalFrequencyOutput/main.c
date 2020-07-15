/**
 * \file main.c
 *
 * \author
 *      Microchip Technology: http://www.microchip.com \n
 *      Support at http://www.microchip.com/support/ \n
 *
 * Copyright (C) 2018 Microchip Technology. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Microchip may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Microchip AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY Microchip "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL Microchip BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *****************************************************
 * Introduction
 * ============
 *
 * This application will output the crystal clock on a pin. There are two functions made
 * available that will accomplish this. The xosc_clock_out_system_clock function
 * will change the system clock source to the 32.768 kHz crystal and use the clock
 * out pin. The xosc_clock_out_evsys_rtc function will output the crystal frequency
 * divided by 64 using the Peripheral Interrupt Timer (PIT) and the Event System. The system
 * clock will not be switched to 32.768 kHz.
 *
 * When switching the system clock to 32.768 kHz, the CPU and all peripherals will
 * also be clocked at the same frequency. This severely limits system performance.
 *
 * The xosc_clock_out_evsys_rtc will use one event channel and the PIT. The RTC is
 * available for other use. The PIT will output an event every 64 clock cycles, giving
 * an output frequency of 512 Hz. By connecting e.g. an oscilloscope to the pin, the
 * frequency of the crystal can be measured.
 *
 * The _PROTECTED_WRITE macro is used for writing to change protected registers.
 *
 * Please ensure that the steps to connect the crystal to the device has been followed.
 * See the ATtiny817 Xplained Pro User Guide for more information.
 *
 * Related documents / Application notes
 * -------------------------------------
 *
 * This application is described in the following application note: to be published
 *
 * Supported evaluation kit
 * ------------------------
 *
 *  - ATtiny817-XPRO
 *
 * Running the demo
 * ----------------
 *
 * 1. Press Download Pack and save the .atzip file
 * 2. Import .atzip file into Atmel Studio 7, File->Import->Atmel Start Project.
 * 3. Follow the instructions in the ATtiny817 Xplained Pro User Guide to connect the crystal to the device.
 * 4. Build the project and program the supported evaluation board
 *****************************************************
 */
#include <avr/io.h>

static inline void xosc_clock_out_system_clock(void)
{
	/*
	 * The crystal is started at the beginning of main in order to give the crystal time to start.
	 * The startup time of the crystal is set to the max value to give the crystal time to settle.
	 * The startup time is based on counting 64K cycles of the crystal. With this startup time it
	 * will take approximately 2 seconds before the crystal is ready to be used by peripherals.
	 *
	 * The run in standby bit is set for the crystal. This will allow the crystal to run in standby
	 * sleep mode. In addition it will allow the crystal to start up even if no peripherals are
	 * requesting the clock.
	 */
	_PROTECTED_WRITE(CLKCTRL_XOSC32KCTRLA, CLKCTRL_ENABLE_bm | CLKCTRL_RUNSTDBY_bm | CLKCTRL_CSUT_64K_gc);

	// Deactivate change protection for register, switch main clock to the 32K oscillator and output the
	// clock to the clkout pin
	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, 1 << CLKCTRL_CLKSEL1_bp | CLKCTRL_CLKOUT_bm);

	// Wait for XOSC32K to be reported as stable
	while (!(CLKCTRL_MCLKSTATUS & CLKCTRL_XOSC32KS_bm))
		;

	// Deactivate change protection for register, set sysclock prescaler to zero
	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0);
}

static inline void xosc_clock_out_evsys_rtc(void)
{
	// Deactivate change protection for register and start XOSC32K
	// Start-up time is set to the maximum 64K cycles (~2 seconds)
	_PROTECTED_WRITE(CLKCTRL_XOSC32KCTRLA, CLKCTRL_ENABLE_bm | CLKCTRL_CSUT_64K_gc | CLKCTRL_RUNSTDBY_bm);

	// Use event system channel 3 as path for RTC PIT DIV64 event
	EVSYS_ASYNCCH3 = EVSYS_ASYNCCH3_PIT_DIV64_gc;

	// Connect channel 3 to async user 8 (evout0)
	EVSYS_ASYNCUSER8 = EVSYS_ASYNCUSER8_ASYNCCH3_gc;

	// muxout evout0 to PA2
	PORTMUX_CTRLA = PORTMUX_EVOUT0_bm;

	// The RTC_STATUS needs to be 0 before writing to the RTC (could be used for start-up time).
	while (RTC.STATUS != 0) {
	}

	// Set crystal as clock source for RTC
	RTC_CLKSEL = RTC_CLKSEL_TOSC32K_gc;

	// Enable PIT in RTC
	RTC_PITCTRLA = RTC_PITEN_bm;
}

int main(void)
{
	// Call one or the other, not both
	xosc_clock_out_system_clock();
	// xosc_clock_out_evsys_rtc();

	while (1) {
	}
}
