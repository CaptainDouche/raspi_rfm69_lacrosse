# raspi_rfm69_lacrosse
Receive wireless sensor for outdoor temperature/humidity like TX29, TX29DTH-IT or similar.

wire a rfm69 module to the spi pins of a raspberry.
one extra gpi pin ("D0" on rfm) is needed to receive interrupts from the rfm69 without polling.
Note: The RFM-D0 Pin may also be wired to a different GPIO-Pin (by changing the `#define` for `RFM69_IRQ_GPIONUM`).
This would be required, if you have a secondary SPI-slave on the Bus.
Using `GPIO 7` has the advantage, that all pins fit into a 2 x 5 pin header.

__Wiring:__

```
        Raspi2 Pinout-Header:
                        +--+--+
        3,3V            | 1| 2| 5V                 
        GPIO 2 (SDA1)   | 3| 4| 5V                    
        GPIO 3 (SCL1)   | 5| 6| GND                
        GPIO 4 (GPCLK0) | 7| 8| GPIO 14 (TxD)      
        GND             | 9|10| GPIO 15 (RxD)      
        GPIO 17         |11|12| GPIO 18 (PCM_CLK)  
        GPIO 27         |13|14| GND                
        GPIO 22         |15|16| GPIO 23            
RFM-~   3,3V            |17|18| GPIO 24            
RFM-~   GPIO 10 (MOSI)  |19|20| GND                
RFM-~   GPIO 9  (MISO)  |21|22| GPIO 25            
RFM-~   GPIO 11 (SCLK)  |23|24| GPIO 8 (CE0)      RFM-NSS
RFM-~   GND             |25|26| GPIO 7 (CE1)      RFM-D0

```

![my wiring](https://raw.githubusercontent.com/CaptainDouche/raspi_rfm69_lacrosse/master/doc/raspi_rfm69_20180415_211208_50p.jpg)
