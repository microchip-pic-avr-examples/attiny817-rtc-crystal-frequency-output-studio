<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# RTC Crystal Frequency Output

This application will output the crystal clock on a pin. Two functions are made available to accomplish this. The xosc_clock_out_system_clock function will change the system clock source to the 32.768 kHz crystal and use the clock out pin (PB5). The xosc_clock_out_evsys_rtc function will output the crystal frequency divided by 64 using the Peripheral Interrupt Timer (PIT) and the Event System on pin PA2. The system clock will not be switched to 32.768 kHz.

This example is based on the Application Note [AN2711 - Real-Time Clock Calibration and Compensation on AVR® Microcontrollers](https://www.microchip.com/DS00002711), which should be referred to for a more detailed understanding of the concepts.
## Related Documentation

- [AN2711 - Real-Time Clock Calibration and Compensation on AVR® Microcontrollers](https://www.microchip.com/DS00002711)
- [ATtiny817 Device Page](https://www.microchip.com/wwwproducts/en/ATTINY817)

## Software Used

- [Atmel Studio](https://www.microchip.com/mplab/avr-support/atmel-studio-7) 7.0.2397 or later
- [ATtiny DFP](http://packs.download.atmel.com/) 1.6.316 or later
- AVR/GNU C Compiler (Built-in compiler) 5.4.0 or later


## Hardware Used

- [ATtiny817 Xplained Pro](https://www.microchip.com/DevelopmentTools/ProductDetails/attiny817-xpro)
- Micro-USB cable (Type-A/Micro-B)

## Setup

1. The crystal on the ATtiny817 Xplained Pro board is by default not connected to the TOSC pins, as they are used for UART communication. To connect the pins, remove resistors R307 and R308 and place them on the footprints of R312 and R313. Refer to [ATtiny817 Xplained Pro User's Guide](https://www.microchip.com/DS50002684) for more information on how to do this.

## Operation

1. Connect the board to the PC.

2. Download the zip file or clone the example to get the source code.

3. Open the .atsln file with Atmel Studio.

4. Build the solution and program the ATtiny817. Press *Start without debugging* or use CTRL+ALT+F5 hotkeys to run the application for programming the device.

## Conclusion

This example has now illustrated how you can output the crystal clock on a pin.
