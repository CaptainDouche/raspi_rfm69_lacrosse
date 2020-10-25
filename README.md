# raspi_rfm69_lacrosse
Receive wireless sensor for outdoor temperature/humidity like TX29, TX29DTH-IT or similar.

Wire a rfm69 module to the spi pins of a raspberry as shown below.
The RFM's IRQ-Pin `D0` is optional.
If provided/wired (`-irq <GPIONUM>`), it can be used for interrupt driven data reception ("Pin Change Detect"). 
If not provided/wired (`-noirq`), then polling is used (about 1/10 seconds), which is fast enough and not CPU-hungry.

__Wiring:__

```
        Raspi Pinout-Header:
                           +--+--+
          3,3V             | 1|2 | 5V                
  RFM-RST GPIO-02 (SDA1)   | 3|4 | 5V
          GPIO-03 (SCL1)   | 5|6 | GND               
  RFM-D0  GPIO-04 (GPCLK0) | 7|8 | GPIO-14 (TxD)
  LCD-RW  GND              | 9|10| GPIO-15 (RxD)
          GPIO-17 (CE1')   |11|12| GPIO-18 (PCM,CE0') 
          GPIO-27          |13|14| GND
          GPIO-22          |15|16| GPIO-23           
  RFM-~   3,3V             |17|18| GPIO-24           
  RFM-~   GPIO-10 (MOSI)   |19|20| GND
  RFM-~   GPIO-09 (MISO)   |21|22| GPIO-25           
  RFM-~   GPIO-11 (SCLK)   |23|24| GPIO-08 (CE0)     RFM-NSS (optional)
  RFM-~   GND              |25|26| GPIO-07 (CE1)     RFM-D0
          EE_SDA           |27|28| EE_SCL 
          GPIO-05          |29|30| GND 
          GPIO-06          |31|32| GPIO-12 
          GPIO-13          |33|34| GND 
          GPIO-19 (MISO')  |35|36| GPIO-16 (CE2')
          GPIO-26          |37|38| GPIO-20 (MOSI')
          GND              |39|40| GPIO-21 (SCLK')
                           +--+--+

```

![my wiring](https://raw.githubusercontent.com/CaptainDouche/raspi_rfm69_lacrosse/master/doc/rpi_rfm69hw_tx29dht-it.jpg)
