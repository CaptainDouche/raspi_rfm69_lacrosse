# raspi_rfm69_lacrosse

Receive wireless sensor for outdoor temperature/humidity like TX29, TX29DTH-IT or similar models.

__Compile, Test, Install__

Wire RFM as shown, compile (`make`), test (`/tmp/rfm69_lacrosse --debug`) and install (`sudo make install`).

Use the `-format` Parameter to specify how the received Sensors shall be printed.

Also there is support for publishing via MQTT:

To publish the received Sensors via MQTT, install prequesites via `sudo apt-get install libmosquitto-dev`, and set (`USE_MOSQUITTO = 1`) in the Makefile.
Use the `-format` Parameter to specify the topic how the received Sensors shall be published.
You must include the `=`sign in the format string as delimiter between the topic and the payload.

See `-help` for a reference of the MQTT- and format variables.


__Wiring:__

Wire a rfm69 module to the spi pins of a raspberry as shown below.
The RFM's IRQ-Pin `D0` is optional.
If provided/wired (`-irq <GPIONUM>`), it can be used for interrupt driven data reception ("Pin Change Detect"). 
If not provided/wired (`-noirq`), then polling is used (about 1/10 seconds), which is fast enough and not CPU-hungry.

```
                    Raspi Pinout-Header:
                           +--+--+
          3,3V             | 1|2 | 5V                
          GPIO-02 (SDA1)   | 3|4 | 5V
          GPIO-03 (SCL1)   | 5|6 | GND               
          GPIO-04 (GPCLK0) | 7|8 | GPIO-14 (TxD)
          GND              | 9|10| GPIO-15 (RxD)
          GPIO-17 (CE1')   |11|12| GPIO-18 (PCM,CE0') 
          GPIO-27          |13|14| GND
          GPIO-22          |15|16| GPIO-23           
  RFM-~   3,3V             |17|18| GPIO-24           
  RFM-~   GPIO-10 (MOSI)   |19|20| GND
  RFM-~   GPIO-09 (MISO)   |21|22| GPIO-25           
  RFM-~   GPIO-11 (SCLK)   |23|24| GPIO-08 (CE0)     RFM-NSS
  RFM-~   GND              |25|26| GPIO-07 (CE1)     RFM-D0 (optional)
          EE_SDA           |27|28| EE_SCL 
          GPIO-05          |29|30| GND 
          GPIO-06          |31|32| GPIO-12 
          GPIO-13          |33|34| GND 
          GPIO-19 (MISO')  |35|36| GPIO-16 (CE2')
          GPIO-26          |37|38| GPIO-20 (MOSI')
          GND              |39|40| GPIO-21 (SCLK')
                           +--+--+

```

__Usage example:__
```
pi@raspi$ rfm69_lacrosse -format "@H:@M:@S: %x_%t=%v %u\\n"
18:17:45: temp_2c=+12.0 C
18:17:45: humid_2c=72 %
18:17:45: batt_new_2c=0
18:17:45: batt_weak_2c=0
18:17:46: temp_c0=+20.9 C
18:17:46: humid_c0=76 %
18:17:46: batt_new_c0=0
18:17:46: batt_weak_c0=0
18:17:46: temp_58=+10.1 C
18:17:46: humid_58=89 %
18:17:46: batt_new_58=0
18:17:46: batt_weak_58=0
```

![my wiring](https://raw.githubusercontent.com/CaptainDouche/raspi_rfm69_lacrosse/master/doc/rpi_rfm69hw_tx29dht-it.jpg)
