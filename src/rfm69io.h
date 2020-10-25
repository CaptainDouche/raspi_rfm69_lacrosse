#ifndef RFM69IO_h
#define RFM69IO_h


// http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO.html
//          Raspi2 Pinout-Header:
//                           +--+--+
//          3,3V             | 1|2 | 5V                
//  RFM-RST GPIO-02 (SDA1)   | 3|4 | 5V
//          GPIO-03 (SCL1)   | 5|6 | GND               
//  RFM-D0  GPIO-04 (GPCLK0) | 7|8 | GPIO-14 (TxD)
//  LCD-RW  GND              | 9|10| GPIO-15 (RxD)
//          GPIO-17 (CE1')   |11|12| GPIO-18 (PCM,CE0') 
//          GPIO-27          |13|14| GND
//          GPIO-22          |15|16| GPIO-23           
//  RFM-~   3,3V             |17|18| GPIO-24           
//  RFM-~   GPIO-10 (MOSI)   |19|20| GND
//  RFM-~   GPIO-09 (MISO)   |21|22| GPIO-25           
//  RFM-~   GPIO-11 (SCLK)   |23|24| GPIO-08 (CE0)     RFM-NSS (optional)
//  RFM-~   GND              |25|26| GPIO-07 (CE1)     RFM-D0
//          EE_SDA           |27|28| EE_SCL 
//          GPIO-05          |29|30| GND 
//          GPIO-06          |31|32| GPIO-12 
//          GPIO-13          |33|34| GND 
//          GPIO-19 (MISO')  |35|36| GPIO-16 (CE2')
//          GPIO-26          |37|38| GPIO-20 (MOSI')
//          GND              |39|40| GPIO-21 (SCLK')
//                           +--+--+

// spidev0.0 is [MOSI,MISO,SCLK,CE0]
// spidev0.1 is [MOSI,MISO,SCLK,CE1]
// spidev1.0 is [MOSI',MISO',SCLK',CE0'] 
// spidev1.1 is [MOSI',MISO',SCLK',CE1'] 
// spidev1.2 is [MOSI',MISO',SCLK',CE2'] 

// To enable spi1 spidev1: set 
// dtoverlay=spi1-1cs in /boot/config.txt. 
// You can change the CE0 pin (e.g. to pin 23) by adding ,cs0_pin=23 to the overlay. 
// See /boot/overlays/README for more info. 

// esp32 spi:
// http://www.electronicwings.com/nodemcu/nodemcu-spi-with-arduino-ide


#define SPI0_CE0				"/dev/spidev0.0" 
#define SPI0_CE1				"/dev/spidev0.1"
#define SPI1_CE0				"/dev/spidev1.0" 
#define SPI1_CE1				"/dev/spidev1.1"
#define SPI1_CE2				"/dev/spidev1.2"



#define RFM69_SPI_DEVFILE		SPI0_CE0
#define RFM69_IRQ_GPIONUM		-1 // 7

#if 1 // registers & bits

// Register names
#define RFM69_REG_FIFO                                 0x00
#define RFM69_REG_OPMODE                               0x01
#define RFM69_REG_DATAMODUL                            0x02
#define RFM69_REG_BITRATEMSB                           0x03
#define RFM69_REG_BITRATELSB                           0x04
#define RFM69_REG_FDEVMSB                              0x05
#define RFM69_REG_FDEVLSB                              0x06
#define RFM69_REG_FRFMSB                               0x07
#define RFM69_REG_FRFMID                               0x08
#define RFM69_REG_FRFLSB                               0x09
#define RFM69_REG_OSC1                                 0x0a
#define RFM69_REG_AFCCTRL                              0x0b
#define RFM69_REG_RESERVED_0C                          0x0c
#define RFM69_REG_LISTEN1                              0x0d
#define RFM69_REG_LISTEN2                              0x0e
#define RFM69_REG_LISTEN3                              0x0f
#define RFM69_REG_VERSION                              0x10
#define RFM69_REG_PALEVEL                              0x11
#define RFM69_REG_PARAMP                               0x12
#define RFM69_REG_OCP                                  0x13
#define RFM69_REG_RESERVED_14                          0x14
#define RFM69_REG_RESERVED_15                          0x15
#define RFM69_REG_RESERVED_16                          0x16
#define RFM69_REG_RESERVED_17                          0x17
#define RFM69_REG_LNA                                  0x18
#define RFM69_REG_RXBW                                 0x19
#define RFM69_REG_AFCBW                                0x1a
#define RFM69_REG_OOKPEAK                              0x1b
#define RFM69_REG_OOKAVG                               0x1c
#define RFM69_REG_OOKFIX                               0x1d
#define RFM69_REG_AFCFEI                               0x1e
#define RFM69_REG_AFCMSB                               0x1f
#define RFM69_REG_AFCLSB                               0x20
#define RFM69_REG_FEIMSB                               0x21
#define RFM69_REG_FEILSB                               0x22
#define RFM69_REG_RSSICONFIG                           0x23
#define RFM69_REG_RSSIVALUE                            0x24
#define RFM69_REG_DIOMAPPING1                          0x25
#define RFM69_REG_DIOMAPPING2                          0x26
#define RFM69_REG_IRQFLAGS1                            0x27
#define RFM69_REG_IRQFLAGS2                            0x28
#define RFM69_REG_RSSITHRESH                           0x29
#define RFM69_REG_RXTIMEOUT1                           0x2a
#define RFM69_REG_RXTIMEOUT2                           0x2b
#define RFM69_REG_PREAMBLEMSB                          0x2c
#define RFM69_REG_PREAMBLELSB                          0x2d
#define RFM69_REG_SYNCCONFIG                           0x2e
#define RFM69_REG_SYNCVALUE1                           0x2f
#define RFM69_REG_SYNCVALUE2                           0x30
#define RFM69_REG_SYNCVALUE3                           0x31
#define RFM69_REG_SYNCVALUE4                           0x32
#define RFM69_REG_SYNCVALUE5                           0x33
#define RFM69_REG_SYNCVALUE6                           0x34
#define RFM69_REG_SYNCVALUE7                           0x35
#define RFM69_REG_SYNCVALUE8                           0x36
#define RFM69_REG_PACKETCONFIG1                        0x37
#define RFM69_REG_PAYLOADLENGTH                        0x38
#define RFM69_REG_NODEADRS                             0x39
#define RFM69_REG_BROADCASTADRS                        0x3a
#define RFM69_REG_AUTOMODES                            0x3b
#define RFM69_REG_FIFOTHRESH                           0x3c
#define RFM69_REG_PACKETCONFIG2                        0x3d
#define RFM69_REG_AESKEY1                              0x3E
#define RFM69_REG_AESKEY2                              0x3F
#define RFM69_REG_AESKEY3                              0x40
#define RFM69_REG_AESKEY4                              0x41
#define RFM69_REG_AESKEY5                              0x42
#define RFM69_REG_AESKEY6                              0x43
#define RFM69_REG_AESKEY7                              0x44
#define RFM69_REG_AESKEY8                              0x45
#define RFM69_REG_AESKEY9                              0x46
#define RFM69_REG_AESKEY10                             0x47
#define RFM69_REG_AESKEY11                             0x48
#define RFM69_REG_AESKEY12                             0x49
#define RFM69_REG_AESKEY13                             0x4A
#define RFM69_REG_AESKEY14                             0x4B
#define RFM69_REG_AESKEY15                             0x4C
#define RFM69_REG_AESKEY16                             0x4D
#define RFM69_REG_TEMP1                                0x4e
#define RFM69_REG_TEMP2                                0x4f
#define RFM69_REG_TESTLNA                              0x58
#define RFM69_REG_TESTPA1                              0x5a
#define RFM69_REG_TESTPA2                              0x5c
#define RFM69_REG_TESTDAGC                             0x6f
#define RFM69_REG_TESTAFC                              0x71

// register names lookup map for debug:

#define RFM69_REGNAME_MAP \
	{ RFM69_REG_FIFO              , "FIFO"          }, \
	{ RFM69_REG_OPMODE            , "OPMODE"        }, \
	{ RFM69_REG_DATAMODUL         , "DATAMODUL"     }, \
	{ RFM69_REG_BITRATEMSB        , "BITRATEMSB"    }, \
	{ RFM69_REG_BITRATELSB        , "BITRATELSB"    }, \
	{ RFM69_REG_FDEVMSB           , "FDEVMSB"       }, \
	{ RFM69_REG_FDEVLSB           , "FDEVLSB"       }, \
	{ RFM69_REG_FRFMSB            , "FRFMSB"        }, \
	{ RFM69_REG_FRFMID            , "FRFMID"        }, \
	{ RFM69_REG_FRFLSB            , "FRFLSB"        }, \
	{ RFM69_REG_OSC1              , "OSC1"          }, \
	{ RFM69_REG_AFCCTRL           , "AFCCTRL"       }, \
	{ RFM69_REG_RESERVED_0C       , "RESERVED_0C"   }, \
	{ RFM69_REG_LISTEN1           , "LISTEN1"       }, \
	{ RFM69_REG_LISTEN2           , "LISTEN2"       }, \
	{ RFM69_REG_LISTEN3           , "LISTEN3"       }, \
	{ RFM69_REG_VERSION           , "VERSION"       }, \
	{ RFM69_REG_PALEVEL           , "PALEVEL"       }, \
	{ RFM69_REG_PARAMP            , "PARAMP"        }, \
	{ RFM69_REG_OCP               , "OCP"           }, \
	{ RFM69_REG_RESERVED_14       , "RESERVED_14"   }, \
	{ RFM69_REG_RESERVED_15       , "RESERVED_15"   }, \
	{ RFM69_REG_RESERVED_16       , "RESERVED_16"   }, \
	{ RFM69_REG_RESERVED_17       , "RESERVED_17"   }, \
	{ RFM69_REG_LNA               , "LNA"           }, \
	{ RFM69_REG_RXBW              , "RXBW"          }, \
	{ RFM69_REG_AFCBW             , "AFCBW"         }, \
	{ RFM69_REG_OOKPEAK           , "OOKPEAK"       }, \
	{ RFM69_REG_OOKAVG            , "OOKAVG"        }, \
	{ RFM69_REG_OOKFIX            , "OOKFIX"        }, \
	{ RFM69_REG_AFCFEI            , "AFCFEI"        }, \
	{ RFM69_REG_AFCMSB            , "AFCMSB"        }, \
	{ RFM69_REG_AFCLSB            , "AFCLSB"        }, \
	{ RFM69_REG_FEIMSB            , "FEIMSB"        }, \
	{ RFM69_REG_FEILSB            , "FEILSB"        }, \
	{ RFM69_REG_RSSICONFIG        , "RSSICONFIG"    }, \
	{ RFM69_REG_RSSIVALUE         , "RSSIVALUE"     }, \
	{ RFM69_REG_DIOMAPPING1       , "DIOMAPPING1"   }, \
	{ RFM69_REG_DIOMAPPING2       , "DIOMAPPING2"   }, \
	{ RFM69_REG_IRQFLAGS1         , "IRQFLAGS1"     }, \
	{ RFM69_REG_IRQFLAGS2         , "IRQFLAGS2"     }, \
	{ RFM69_REG_RSSITHRESH        , "RSSITHRESH"    }, \
	{ RFM69_REG_RXTIMEOUT1        , "RXTIMEOUT1"    }, \
	{ RFM69_REG_RXTIMEOUT2        , "RXTIMEOUT2"    }, \
	{ RFM69_REG_PREAMBLEMSB       , "PREAMBLEMSB"   }, \
	{ RFM69_REG_PREAMBLELSB       , "PREAMBLELSB"   }, \
	{ RFM69_REG_SYNCCONFIG        , "SYNCCONFIG"    }, \
	{ RFM69_REG_SYNCVALUE1        , "SYNCVALUE1"    }, \
	{ RFM69_REG_SYNCVALUE2        , "SYNCVALUE2"    }, \
	{ RFM69_REG_SYNCVALUE3        , "SYNCVALUE3"    }, \
	{ RFM69_REG_SYNCVALUE4        , "SYNCVALUE4"    }, \
	{ RFM69_REG_SYNCVALUE5        , "SYNCVALUE5"    }, \
	{ RFM69_REG_SYNCVALUE6        , "SYNCVALUE6"    }, \
	{ RFM69_REG_SYNCVALUE7        , "SYNCVALUE7"    }, \
	{ RFM69_REG_SYNCVALUE8        , "SYNCVALUE8"    }, \
	{ RFM69_REG_PACKETCONFIG1     , "PACKETCONFIG1" }, \
	{ RFM69_REG_PAYLOADLENGTH     , "PAYLOADLENGTH" }, \
	{ RFM69_REG_NODEADRS          , "NODEADRS"      }, \
	{ RFM69_REG_BROADCASTADRS     , "BROADCASTADRS" }, \
	{ RFM69_REG_AUTOMODES         , "AUTOMODES"     }, \
	{ RFM69_REG_FIFOTHRESH        , "FIFOTHRESH"    }, \
	{ RFM69_REG_PACKETCONFIG2     , "PACKETCONFIG2" }, \
	{ RFM69_REG_AESKEY1           , "AESKEY1"       }, \
	{ RFM69_REG_AESKEY2           , "AESKEY2"       }, \
	{ RFM69_REG_AESKEY3           , "AESKEY3"       }, \
	{ RFM69_REG_AESKEY4           , "AESKEY4"       }, \
	{ RFM69_REG_AESKEY5           , "AESKEY5"       }, \
	{ RFM69_REG_AESKEY6           , "AESKEY6"       }, \
	{ RFM69_REG_AESKEY7           , "AESKEY7"       }, \
	{ RFM69_REG_AESKEY8           , "AESKEY8"       }, \
	{ RFM69_REG_AESKEY9           , "AESKEY9"       }, \
	{ RFM69_REG_AESKEY10          , "AESKEY10"      }, \
	{ RFM69_REG_AESKEY11          , "AESKEY11"      }, \
	{ RFM69_REG_AESKEY12          , "AESKEY12"      }, \
	{ RFM69_REG_AESKEY13          , "AESKEY13"      }, \
	{ RFM69_REG_AESKEY14          , "AESKEY14"      }, \
	{ RFM69_REG_AESKEY15          , "AESKEY15"      }, \
	{ RFM69_REG_AESKEY16          , "AESKEY16"      }, \
	{ RFM69_REG_TEMP1             , "TEMP1"         }, \
	{ RFM69_REG_TEMP2             , "TEMP2"         }, \
	{ RFM69_REG_TESTLNA           , "TESTLNA"       }, \
	{ RFM69_REG_TESTPA1           , "TESTPA1"       }, \
	{ RFM69_REG_TESTPA2           , "TESTPA2"       }, \
	{ RFM69_REG_TESTDAGC          , "TESTDAGC"      }, \
	{ RFM69_REG_TESTAFC           , "TESTAFC"       }, \
	{ 0xff                        , NULL            }

// RFM69_REG_OPMODE

#define RFM69_OPMODE_SEQUENCER_ON                         0x00
#define RFM69_OPMODE_SEQUENCER_OFF                        0x80
#define RFM69_OPMODE_LISTEN_ON                            0x40
#define RFM69_OPMODE_LISTEN_OFF					          0x00  // Default
#define RFM69_OPMODE_LISTENABORT                          0x20
#define RFM69_OPMODE_MODE                                 0x1c
#define RFM69_OPMODE_MODE_SLEEP                           0x00
#define RFM69_OPMODE_MODE_STDBY                           0x04
#define RFM69_OPMODE_MODE_FS                              0x08
#define RFM69_OPMODE_MODE_TX                              0x0c
#define RFM69_OPMODE_MODE_RX                              0x10

// RFM69_REG_DATAMODUL
#define RFM69_DATAMODUL_DATAMODE                          0x60
#define RFM69_DATAMODUL_DATAMODE_PACKET                   0x00
#define RFM69_DATAMODUL_DATAMODE_CONT_WITH_SYNC           0x40
#define RFM69_DATAMODUL_DATAMODE_CONT_WITHOUT_SYNC        0x60
#define RFM69_DATAMODUL_MODULATIONTYPE                    0x18
#define RFM69_DATAMODUL_MODULATIONTYPE_FSK                0x00
#define RFM69_DATAMODUL_MODULATIONTYPE_OOK                0x08
#define RFM69_DATAMODUL_MODULATIONSHAPING                 0x03
#define RFM69_DATAMODUL_MODULATIONSHAPING_FSK_NONE        0x00
#define RFM69_DATAMODUL_MODULATIONSHAPING_FSK_BT1_0       0x01
#define RFM69_DATAMODUL_MODULATIONSHAPING_FSK_BT0_5       0x02
#define RFM69_DATAMODUL_MODULATIONSHAPING_FSK_BT0_3       0x03
#define RFM69_DATAMODUL_MODULATIONSHAPING_OOK_NONE        0x00
#define RFM69_DATAMODUL_MODULATIONSHAPING_OOK_BR          0x01
#define RFM69_DATAMODUL_MODULATIONSHAPING_OOK_2BR         0x02

// RFM69_REG_PALEVEL
#define RFM69_PALEVEL_PA0_OFF                             0x00
#define RFM69_PALEVEL_PA0_ON                              0x80
#define RFM69_PALEVEL_PA1_OFF                             0x00
#define RFM69_PALEVEL_PA1_ON                              0x40
#define RFM69_PALEVEL_PA2_OFF                             0x00
#define RFM69_PALEVEL_PA2_ON                              0x20
#define RFM69_PALEVEL_OUTPUTPOWER                         0x1f

// RFM69_REG_OCP
#define RFM69_OCP_ON                                      0x1a
#define RFM69_OCP_OFF                                     0x0f

// RFM69_REG_RSSICONFIG
#define RFM69_RSSICONFIG_RSSIDONE                         0x02
#define RFM69_RSSICONFIG_RSSISTART                        0x01

// RFM69_REG_DIOMAPPING1
#define RFM69_DIOMAPPING1_DIO0                     0xc0
#define RFM69_DIOMAPPING1_DIO0_00                  0x00
#define RFM69_DIOMAPPING1_DIO0_01                  0x40
#define RFM69_DIOMAPPING1_DIO0_10                  0x80
#define RFM69_DIOMAPPING1_DIO0_11                  0xc0

#define RFM69_DIOMAPPING1_DIO1                     0x30
#define RFM69_DIOMAPPING1_DIO1_00                  0x00
#define RFM69_DIOMAPPING1_DIO1_01                  0x10
#define RFM69_DIOMAPPING1_DIO1_10                  0x20
#define RFM69_DIOMAPPING1_DIO1_11                  0x30

#define RFM69_DIOMAPPING1_DIO2                     0x0c
#define RFM69_DIOMAPPING1_DIO2_00                  0x00
#define RFM69_DIOMAPPING1_DIO2_01                  0x04
#define RFM69_DIOMAPPING1_DIO2_10                  0x08
#define RFM69_DIOMAPPING1_DIO2_11                  0x0c

#define RFM69_DIOMAPPING1_DIO3                     0x03
#define RFM69_DIOMAPPING1_DIO3_00                  0x00
#define RFM69_DIOMAPPING1_DIO3_01                  0x01
#define RFM69_DIOMAPPING1_DIO3_10                  0x02
#define RFM69_DIOMAPPING1_DIO3_11                  0x03

// RFM69_REG_DIOMAPPING2
#define RFM69_DIOMAPPING2_DIO4                     0xc0
#define RFM69_DIOMAPPING2_DIO4_00                  0x00
#define RFM69_DIOMAPPING2_DIO4_01                  0x40
#define RFM69_DIOMAPPING2_DIO4_10                  0x80
#define RFM69_DIOMAPPING2_DIO4_11                  0xc0

#define RFM69_DIOMAPPING2_DIO5                     0x30
#define RFM69_DIOMAPPING2_DIO5_00                  0x00
#define RFM69_DIOMAPPING2_DIO5_01                  0x10
#define RFM69_DIOMAPPING2_DIO5_10                  0x20
#define RFM69_DIOMAPPING2_DIO5_11                  0x30

#define RFM69_DIOMAPPING2_CLKOUT                          0x07
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_                   0x00
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_2                  0x01
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_4                  0x02
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_8                  0x03
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_16                 0x04
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_32                 0x05
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_RC                 0x06
#define RFM69_DIOMAPPING2_CLKOUT_FXOSC_OFF                0x07

// RFM69_REG_IRQFLAGS1
#define RFM69_IRQFLAGS1_MODEREADY                         0x80
#define RFM69_IRQFLAGS1_RXREADY                           0x40
#define RFM69_IRQFLAGS1_TXREADY                           0x20
#define RFM69_IRQFLAGS1_PLLLOCK                           0x10
#define RFM69_IRQFLAGS1_RSSI                              0x08
#define RFM69_IRQFLAGS1_TIMEOUT                           0x04
#define RFM69_IRQFLAGS1_AUTOMODE                          0x02
#define RFM69_IRQFLAGS1_SYNADDRESSMATCH                   0x01

// RFM69_REG_IRQFLAGS2
#define RFM69_IRQFLAGS2_FIFOFULL                          0x80
#define RFM69_IRQFLAGS2_FIFONOTEMPTY                      0x40
#define RFM69_IRQFLAGS2_FIFOLEVEL                         0x20
#define RFM69_IRQFLAGS2_FIFOOVERRUN                       0x10
#define RFM69_IRQFLAGS2_PACKETSENT                        0x08
#define RFM69_IRQFLAGS2_PAYLOADREADY                      0x04
#define RFM69_IRQFLAGS2_CRCOK                             0x02

// RFM69_REG_SYNCCONFIG
#define RFM69_SYNCCONFIG_SYNCON                           0x80
#define RFM69_SYNCCONFIG_FIFOFILLCONDITION_MANUAL         0x40
#define RFM69_SYNCCONFIG_SYNCSIZE                         0x38
#define RFM69_SYNCCONFIG_SYNCSIZE_1                       0x00
#define RFM69_SYNCCONFIG_SYNCSIZE_2                       0x08
#define RFM69_SYNCCONFIG_SYNCSIZE_3                       0x10
#define RFM69_SYNCCONFIG_SYNCSIZE_4                       0x18
#define RFM69_SYNCCONFIG_SYNCSIZE_5                       0x20
#define RFM69_SYNCCONFIG_SYNCSIZE_6                       0x28
#define RFM69_SYNCCONFIG_SYNCSIZE_7                       0x30
#define RFM69_SYNCCONFIG_SYNCSIZE_8                       0x38
#define RFM69_SYNCCONFIG_SYNCSIZE_SYNCTOL                 0x07

// RFM69_REG_PACKETCONFIG1
#define RFM69_PACKETCONFIG1_PACKETFORMAT_VARIABLE         0x80
#define RFM69_PACKETCONFIG1_DCFREE                        0x60
#define RFM69_PACKETCONFIG1_DCFREE_NONE                   0x00
#define RFM69_PACKETCONFIG1_DCFREE_MANCHESTER             0x20
#define RFM69_PACKETCONFIG1_DCFREE_WHITENING              0x40
#define RFM69_PACKETCONFIG1_DCFREE_RESERVED               0x60
#define RFM69_PACKETCONFIG1_CRC_ON                        0x10
#define RFM69_PACKETCONFIG1_CRCAUTOCLEAROFF               0x08
#define RFM69_PACKETCONFIG1_ADDRESSFILTERING              0x06
#define RFM69_PACKETCONFIG1_ADDRESSFILTERING_NONE         0x00
#define RFM69_PACKETCONFIG1_ADDRESSFILTERING_NODE         0x02
#define RFM69_PACKETCONFIG1_ADDRESSFILTERING_NODE_BC      0x04
#define RFM69_PACKETCONFIG1_ADDRESSFILTERING_RESERVED     0x06

// RFM69_REG_FIFOTHRESH
#define RFM69_FIFOTHRESH_TXSTARTCONDITION_NOTEMPTY        0x80
#define RFM69_FIFOTHRESH_FIFOTHRESHOLD                    0x7f

// RFM69_REG_PACKETCONFIG2
#define RFM69_PACKETCONFIG2_INTERPACKETRXDELAY            0xf0
#define RFM69_PACKETCONFIG2_RESTARTRX                     0x04
#define RFM69_PACKETCONFIG2_AUTORXRESTARTON               0x02
#define RFM69_PACKETCONFIG2_AESON                         0x01

// RFM69_REG_TEMP1
#define RFM69_TEMP1_TEMPMEASSTART                         0x08
#define RFM69_TEMP1_TEMPMEASRUNNING                       0x04


// RegFdev - frequency deviation (Hz)
#define RFM69_FDEVMSB_2000							0x00
#define RFM69_FDEVLSB_2000							0x21
#define RFM69_FDEVMSB_5000							0x00  // Default
#define RFM69_FDEVLSB_5000							0x52  // Default
#define RFM69_FDEVMSB_7500							0x00
#define RFM69_FDEVLSB_7500							0x7B
#define RFM69_FDEVMSB_10000						0x00
#define RFM69_FDEVLSB_10000						0xA4
#define RFM69_FDEVMSB_15000						0x00
#define RFM69_FDEVLSB_15000						0xF6
#define RFM69_FDEVMSB_20000						0x01
#define RFM69_FDEVLSB_20000						0x48
#define RFM69_FDEVMSB_25000						0x01
#define RFM69_FDEVLSB_25000						0x9A
#define RFM69_FDEVMSB_30000						0x01
#define RFM69_FDEVLSB_30000						0xEC
#define RFM69_FDEVMSB_35000						0x02
#define RFM69_FDEVLSB_35000						0x3D
#define RFM69_FDEVMSB_40000						0x02
#define RFM69_FDEVLSB_40000						0x8F
#define RFM69_FDEVMSB_45000						0x02
#define RFM69_FDEVLSB_45000						0xE1
#define RFM69_FDEVMSB_50000						0x03
#define RFM69_FDEVLSB_50000						0x33
#define RFM69_FDEVMSB_55000						0x03
#define RFM69_FDEVLSB_55000						0x85
#define RFM69_FDEVMSB_60000						0x03
#define RFM69_FDEVLSB_60000						0xD7
#define RFM69_FDEVMSB_65000						0x04
#define RFM69_FDEVLSB_65000						0x29
#define RFM69_FDEVMSB_70000						0x04
#define RFM69_FDEVLSB_70000						0x7B
#define RFM69_FDEVMSB_75000						0x04
#define RFM69_FDEVLSB_75000						0xCD
#define RFM69_FDEVMSB_80000						0x05
#define RFM69_FDEVLSB_80000						0x1F
#define RFM69_FDEVMSB_85000						0x05
#define RFM69_FDEVLSB_85000						0x71
#define RFM69_FDEVMSB_90000						0x05
#define RFM69_FDEVLSB_90000						0xC3
#define RFM69_FDEVMSB_95000						0x06
#define RFM69_FDEVLSB_95000						0x14
#define RFM69_FDEVMSB_100000						0x06
#define RFM69_FDEVLSB_100000						0x66
#define RFM69_FDEVMSB_110000						0x07
#define RFM69_FDEVLSB_110000						0x0A
#define RFM69_FDEVMSB_120000						0x07
#define RFM69_FDEVLSB_120000						0xAE
#define RFM69_FDEVMSB_130000						0x08
#define RFM69_FDEVLSB_130000						0x52
#define RFM69_FDEVMSB_140000						0x08
#define RFM69_FDEVLSB_140000						0xF6
#define RFM69_FDEVMSB_150000						0x09
#define RFM69_FDEVLSB_150000						0x9A
#define RFM69_FDEVMSB_160000						0x0A
#define RFM69_FDEVLSB_160000						0x3D
#define RFM69_FDEVMSB_170000						0x0A
#define RFM69_FDEVLSB_170000						0xE1
#define RFM69_FDEVMSB_180000						0x0B
#define RFM69_FDEVLSB_180000						0x85
#define RFM69_FDEVMSB_190000						0x0C
#define RFM69_FDEVLSB_190000						0x29
#define RFM69_FDEVMSB_200000						0x0C
#define RFM69_FDEVLSB_200000						0xCD
#define RFM69_FDEVMSB_210000						0x0D
#define RFM69_FDEVLSB_210000						0x71
#define RFM69_FDEVMSB_220000						0x0E
#define RFM69_FDEVLSB_220000						0x14
#define RFM69_FDEVMSB_230000						0x0E
#define RFM69_FDEVLSB_230000						0xB8
#define RFM69_FDEVMSB_240000						0x0F
#define RFM69_FDEVLSB_240000						0x5C
#define RFM69_FDEVMSB_250000						0x10
#define RFM69_FDEVLSB_250000						0x00
#define RFM69_FDEVMSB_260000						0x10
#define RFM69_FDEVLSB_260000						0xA4
#define RFM69_FDEVMSB_270000						0x11
#define RFM69_FDEVLSB_270000						0x48
#define RFM69_FDEVMSB_280000						0x11
#define RFM69_FDEVLSB_280000						0xEC
#define RFM69_FDEVMSB_290000						0x12
#define RFM69_FDEVLSB_290000						0x8F
#define RFM69_FDEVMSB_300000						0x13
#define RFM69_FDEVLSB_300000						0x33

// RegFrf (MHz) - carrier frequency
// 315Mhz band
#define RFM69_FRFMSB_314							0x4E
#define RFM69_FRFMID_314							0x80
#define RFM69_FRFLSB_314							0x00
#define RFM69_FRFMSB_315							0x4E
#define RFM69_FRFMID_315							0xC0
#define RFM69_FRFLSB_315							0x00
#define RFM69_FRFMSB_316							0x4F
#define RFM69_FRFMID_316							0x00
#define RFM69_FRFLSB_316							0x00
// 433mhz band
#define RFM69_FRFMSB_433							0x6C
#define RFM69_FRFMID_433							0x40
#define RFM69_FRFLSB_433							0x00
#define RFM69_FRFMSB_434							0x6C
#define RFM69_FRFMID_434							0x80
#define RFM69_FRFLSB_434							0x00
#define RFM69_FRFMSB_435							0x6C
#define RFM69_FRFMID_435							0xC0
#define RFM69_FRFLSB_435							0x00
// 868Mhz band
#define RFM69_FRFMSB_863							0xD7
#define RFM69_FRFMID_863							0xC0
#define RFM69_FRFLSB_863							0x00
#define RFM69_FRFMSB_864							0xD8
#define RFM69_FRFMID_864							0x00
#define RFM69_FRFLSB_864							0x00
#define RFM69_FRFMSB_865							0xD8
#define RFM69_FRFMID_865							0x40
#define RFM69_FRFLSB_865							0x00
#define RFM69_FRFMSB_866							0xD8
#define RFM69_FRFMID_866							0x80
#define RFM69_FRFLSB_866							0x00
#define RFM69_FRFMSB_867							0xD8
#define RFM69_FRFMID_867							0xC0
#define RFM69_FRFLSB_867							0x00
#define RFM69_FRFMSB_868							0xD9
#define RFM69_FRFMID_868							0x00
#define RFM69_FRFLSB_868							0x00
#define RFM69_FRFMSB_869							0xD9
#define RFM69_FRFMID_869							0x40
#define RFM69_FRFLSB_869							0x00
#define RFM69_FRFMSB_870							0xD9
#define RFM69_FRFMID_870							0x80
#define RFM69_FRFLSB_870							0x00
// 915Mhz band
#define RFM69_FRFMSB_902							0xE1
#define RFM69_FRFMID_902							0x80
#define RFM69_FRFLSB_902							0x00
#define RFM69_FRFMSB_903							0xE1
#define RFM69_FRFMID_903							0xC0
#define RFM69_FRFLSB_903							0x00
#define RFM69_FRFMSB_904							0xE2
#define RFM69_FRFMID_904							0x00
#define RFM69_FRFLSB_904							0x00
#define RFM69_FRFMSB_905							0xE2
#define RFM69_FRFMID_905							0x40
#define RFM69_FRFLSB_905							0x00
#define RFM69_FRFMSB_906							0xE2
#define RFM69_FRFMID_906							0x80
#define RFM69_FRFLSB_906							0x00
#define RFM69_FRFMSB_907							0xE2
#define RFM69_FRFMID_907							0xC0
#define RFM69_FRFLSB_907							0x00
#define RFM69_FRFMSB_908							0xE3
#define RFM69_FRFMID_908							0x00
#define RFM69_FRFLSB_908							0x00
#define RFM69_FRFMSB_909							0xE3
#define RFM69_FRFMID_909							0x40
#define RFM69_FRFLSB_909							0x00
#define RFM69_FRFMSB_910							0xE3
#define RFM69_FRFMID_910							0x80
#define RFM69_FRFLSB_910							0x00
#define RFM69_FRFMSB_911							0xE3
#define RFM69_FRFMID_911							0xC0
#define RFM69_FRFLSB_911							0x00
#define RFM69_FRFMSB_912							0xE4
#define RFM69_FRFMID_912							0x00
#define RFM69_FRFLSB_912							0x00
#define RFM69_FRFMSB_913							0xE4
#define RFM69_FRFMID_913							0x40
#define RFM69_FRFLSB_913							0x00
#define RFM69_FRFMSB_914							0xE4
#define RFM69_FRFMID_914							0x80
#define RFM69_FRFLSB_914							0x00
#define RFM69_FRFMSB_915							0xE4  // Default
#define RFM69_FRFMID_915							0xC0  // Default
#define RFM69_FRFLSB_915							0x00  // Default
#define RFM69_FRFMSB_916							0xE5
#define RFM69_FRFMID_916							0x00
#define RFM69_FRFLSB_916							0x00
#define RFM69_FRFMSB_917							0xE5
#define RFM69_FRFMID_917							0x40
#define RFM69_FRFLSB_917							0x00
#define RFM69_FRFMSB_918							0xE5
#define RFM69_FRFMID_918							0x80
#define RFM69_FRFLSB_918							0x00
#define RFM69_FRFMSB_919							0xE5
#define RFM69_FRFMID_919							0xC0
#define RFM69_FRFLSB_919							0x00
#define RFM69_FRFMSB_920							0xE6
#define RFM69_FRFMID_920							0x00
#define RFM69_FRFLSB_920							0x00
#define RFM69_FRFMSB_921							0xE6
#define RFM69_FRFMID_921							0x40
#define RFM69_FRFLSB_921							0x00
#define RFM69_FRFMSB_922							0xE6
#define RFM69_FRFMID_922							0x80
#define RFM69_FRFLSB_922							0x00
#define RFM69_FRFMSB_923							0xE6
#define RFM69_FRFMID_923							0xC0
#define RFM69_FRFLSB_923							0x00
#define RFM69_FRFMSB_924							0xE7
#define RFM69_FRFMID_924							0x00
#define RFM69_FRFLSB_924							0x00
#define RFM69_FRFMSB_925							0xE7
#define RFM69_FRFMID_925							0x40
#define RFM69_FRFLSB_925							0x00
#define RFM69_FRFMSB_926							0xE7
#define RFM69_FRFMID_926							0x80
#define RFM69_FRFLSB_926							0x00
#define RFM69_FRFMSB_927							0xE7
#define RFM69_FRFMID_927							0xC0
#define RFM69_FRFLSB_927							0x00
#define RFM69_FRFMSB_928							0xE8
#define RFM69_FRFMID_928							0x00
#define RFM69_FRFLSB_928							0x00

// RF69/SX1231 bit control definition


// RegRssiThresh
#define RFM69_RSSITHRESH_VALUE						    0xE4  // Default

// RegRxTimeout1
#define RFM69_RXTIMEOUT1_RXSTART_VALUE				0x00  // Default

// RegRxTimeout2
#define RFM69_RXTIMEOUT2_RSSITHRESH_VALUE		0x00  // Default

// RegPreamble
#define RFM69_PREAMBLESIZE_MSB_VALUE				  0x00  // Default
#define RFM69_PREAMBLESIZE_LSB_VALUE				  0x03  // Default


// RegSyncConfig
#define RFM69_SYNC_ON								0x80  // Default
#define RFM69_SYNC_OFF								0x00

#define RFM69_SYNC_FIFOFILL_AUTO			0x00  // Default -- when sync interrupt occurs
#define RFM69_SYNC_FIFOFILL_MANUAL		0x40

#define RFM69_SYNC_SIZE_1						0x00
#define RFM69_SYNC_SIZE_2						0x08
#define RFM69_SYNC_SIZE_3						0x10
#define RFM69_SYNC_SIZE_4						0x18  // Default
#define RFM69_SYNC_SIZE_5						0x20
#define RFM69_SYNC_SIZE_6						0x28
#define RFM69_SYNC_SIZE_7						0x30
#define RFM69_SYNC_SIZE_8						0x38

#define RFM69_SYNC_TOL_0							0x00  // Default
#define RFM69_SYNC_TOL_1							0x01
#define RFM69_SYNC_TOL_2							0x02
#define RFM69_SYNC_TOL_3							0x03
#define RFM69_SYNC_TOL_4							0x04
#define RFM69_SYNC_TOL_5							0x05
#define RFM69_SYNC_TOL_6							0x06
#define RFM69_SYNC_TOL_7							0x07


// RegSyncValue1-8
#define RFM69_SYNC_BYTE1_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE2_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE3_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE4_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE5_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE6_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE7_VALUE				0x00  // Default
#define RFM69_SYNC_BYTE8_VALUE				0x00  // Default


// RegPacketConfig1
#define RFM69_PACKET1_FORMAT_FIXED			0x00  // Default
#define RFM69_PACKET1_FORMAT_VARIABLE		0x80

#define RFM69_PACKET1_DCFREE_OFF			0x00  // Default
#define RFM69_PACKET1_DCFREE_MANCHESTER	0x20
#define RFM69_PACKET1_DCFREE_WHITENING		0x40

#define RFM69_PACKET1_CRC_ON				0x10  // Default
#define RFM69_PACKET1_CRC_OFF				0x00

#define RFM69_PACKET1_CRCAUTOCLEAR_ON		0x00  // Default
#define RFM69_PACKET1_CRCAUTOCLEAR_OFF		0x08

#define RFM69_PACKET1_ADRSFILTERING_OFF			0x00  // Default
#define RFM69_PACKET1_ADRSFILTERING_NODE			0x02
#define RFM69_PACKET1_ADRSFILTERING_NODEBROADCAST	0x04


// RegPayloadLength
#define RFM69_PAYLOADLENGTH_VALUE					0x40  // Default

// RegBroadcastAdrs
#define RFM69_BROADCASTADDRESS_VALUE				0x00


// RegAutoModes
#define RFM69_AUTOMODES_ENTER_OFF					      0x00  // Default
#define RFM69_AUTOMODES_ENTER_FIFONOTEMPTY			  0x20
#define RFM69_AUTOMODES_ENTER_FIFOLEVEL			    0x40
#define RFM69_AUTOMODES_ENTER_CRCOK				      0x60
#define RFM69_AUTOMODES_ENTER_PAYLOADREADY			  0x80
#define RFM69_AUTOMODES_ENTER_SYNCADRSMATCH		  0xA0
#define RFM69_AUTOMODES_ENTER_PACKETSENT			    0xC0
#define RFM69_AUTOMODES_ENTER_FIFOEMPTY			    0xE0

#define RFM69_AUTOMODES_EXIT_OFF					        0x00  // Default
#define RFM69_AUTOMODES_EXIT_FIFOEMPTY		  		  0x04
#define RFM69_AUTOMODES_EXIT_FIFOLEVEL	  			  0x08
#define RFM69_AUTOMODES_EXIT_CRCOK					      0x0C
#define RFM69_AUTOMODES_EXIT_PAYLOADREADY		  	0x10
#define RFM69_AUTOMODES_EXIT_SYNCADRSMATCH			  0x14
#define RFM69_AUTOMODES_EXIT_PACKETSENT		  	  0x18
#define RFM69_AUTOMODES_EXIT_RXTIMEOUT				    0x1C

#define RFM69_AUTOMODES_INTERMEDIATE_SLEEP			  0x00  // Default
#define RFM69_AUTOMODES_INTERMEDIATE_STANDBY		  0x01
#define RFM69_AUTOMODES_INTERMEDIATE_RECEIVER		0x02
#define RFM69_AUTOMODES_INTERMEDIATE_TRANSMITTER	0x03


// RegFifoThresh
#define RFM69_FIFOTHRESH_TXSTART_FIFOTHRESH		  0x00
#define RFM69_FIFOTHRESH_TXSTART_FIFONOTEMPTY		0x80  // Default

#define RFM69_FIFOTHRESH_VALUE						        0x0F  // Default


// RegPacketConfig2
#define RFM69_PACKET2_RXRESTARTDELAY_1BIT			  0x00  // Default
#define RFM69_PACKET2_RXRESTARTDELAY_2BITS			  0x10
#define RFM69_PACKET2_RXRESTARTDELAY_4BITS	  		0x20
#define RFM69_PACKET2_RXRESTARTDELAY_8BITS		  	0x30
#define RFM69_PACKET2_RXRESTARTDELAY_16BITS		  0x40
#define RFM69_PACKET2_RXRESTARTDELAY_32BITS  		0x50
#define RFM69_PACKET2_RXRESTARTDELAY_64BITS	  	0x60
#define RFM69_PACKET2_RXRESTARTDELAY_128BITS		  0x70
#define RFM69_PACKET2_RXRESTARTDELAY_256BITS 		0x80
#define RFM69_PACKET2_RXRESTARTDELAY_512BITS	  	0x90
#define RFM69_PACKET2_RXRESTARTDELAY_1024BITS		0xA0
#define RFM69_PACKET2_RXRESTARTDELAY_2048BITS		0xB0
#define RFM69_PACKET2_RXRESTARTDELAY_NONE			  0xC0
#define RFM69_PACKET2_RXRESTART					        0x04

#define RFM69_PACKET2_AUTORXRESTART_ON				    0x02  // Default
#define RFM69_PACKET2_AUTORXRESTART_OFF			    0x00

#define RFM69_PACKET2_AES_ON						          0x01
#define RFM69_PACKET2_AES_OFF						        0x00  // Default



// RegOpMode
#define RFM69_OPMODE_SEQUENCER_OFF				0x80
#define RFM69_OPMODE_SEQUENCER_ON				0x00  // Default

#define RFM69_OPMODE_LISTEN_ON						0x40
#define RFM69_OPMODE_LISTEN_OFF					0x00  // Default

#define RFM69_OPMODE_LISTENABORT					0x20

#define RFM69_OPMODE_SLEEP							  0x00
#define RFM69_OPMODE_STANDBY						  0x04  // Default
#define RFM69_OPMODE_SYNTHESIZER					0x08
#define RFM69_OPMODE_TRANSMITTER					0x0C
#define RFM69_OPMODE_RECEIVER						0x10

// RegDataModul
#define RFM69_DATAMODUL_DATAMODE_PACKET			      0x00  // Default
#define RFM69_DATAMODUL_DATAMODE_CONTINUOUS		    0x40
#define RFM69_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC	0x60

#define RFM69_DATAMODUL_MODULATIONTYPE_FSK			    0x00  // Default
#define RFM69_DATAMODUL_MODULATIONTYPE_OOK			    0x08

#define RFM69_DATAMODUL_MODULATIONSHAPING_00		    0x00  // Default
#define RFM69_DATAMODUL_MODULATIONSHAPING_01		    0x01
#define RFM69_DATAMODUL_MODULATIONSHAPING_10		    0x02
#define RFM69_DATAMODUL_MODULATIONSHAPING_11		    0x03

// RegBitRate (bits/sec) example bit rates
#define RFM69_BITRATEMSB_1200						0x68
#define RFM69_BITRATELSB_1200						0x2B
#define RFM69_BITRATEMSB_2400						0x34
#define RFM69_BITRATELSB_2400						0x15
#define RFM69_BITRATEMSB_4800						0x1A  // Default
#define RFM69_BITRATELSB_4800						0x0B  // Default
#define RFM69_BITRATEMSB_9600						0x0D
#define RFM69_BITRATELSB_9600						0x05
#define RFM69_BITRATEMSB_19200						0x06
#define RFM69_BITRATELSB_19200						0x83
#define RFM69_BITRATEMSB_38400						0x03
#define RFM69_BITRATELSB_38400						0x41

#define RFM69_BITRATEMSB_38323						0x03
#define RFM69_BITRATELSB_38323						0x43

#define RFM69_BITRATEMSB_34482						0x03
#define RFM69_BITRATELSB_34482						0xA0

#define RFM69_BITRATEMSB_76800						0x01
#define RFM69_BITRATELSB_76800						0xA1
#define RFM69_BITRATEMSB_153600					0x00
#define RFM69_BITRATELSB_153600					0xD0
#define RFM69_BITRATEMSB_57600						0x02
#define RFM69_BITRATELSB_57600						0x2C
#define RFM69_BITRATEMSB_115200					0x01
#define RFM69_BITRATELSB_115200					0x16
#define RFM69_BITRATEMSB_12500						0x0A
#define RFM69_BITRATELSB_12500						0x00
#define RFM69_BITRATEMSB_25000						0x05
#define RFM69_BITRATELSB_25000						0x00
#define RFM69_BITRATEMSB_50000						0x02
#define RFM69_BITRATELSB_50000						0x80
#define RFM69_BITRATEMSB_100000					0x01
#define RFM69_BITRATELSB_100000					0x40
#define RFM69_BITRATEMSB_150000					0x00
#define RFM69_BITRATELSB_150000					0xD5
#define RFM69_BITRATEMSB_200000					0x00
#define RFM69_BITRATELSB_200000					0xA0
#define RFM69_BITRATEMSB_250000					0x00
#define RFM69_BITRATELSB_250000					0x80
#define RFM69_BITRATEMSB_300000					0x00
#define RFM69_BITRATELSB_300000					0x6B
#define RFM69_BITRATEMSB_32768						0x03
#define RFM69_BITRATELSB_32768						0xD1
//custom bit rates
#define RFM69_BITRATEMSB_55555     0x02
#define RFM69_BITRATELSB_55555     0x40
#define RFM69_BITRATEMSB_200KBPS      0x00
#define RFM69_BITRATELSB_200KBPS      0xa0



// RegOsc1
#define RFM69_OSC1_RCCAL_START				0x80
#define RFM69_OSC1_RCCAL_DONE				0x40

// RegLowBat
#define RFM69_LOWBAT_MONITOR					0x10
#define RFM69_LOWBAT_ON							0x08
#define RFM69_LOWBAT_OFF							0x00  // Default

#define RFM69_LOWBAT_TRIM_1695				0x00
#define RFM69_LOWBAT_TRIM_1764				0x01
#define RFM69_LOWBAT_TRIM_1835				0x02  // Default
#define RFM69_LOWBAT_TRIM_1905				0x03
#define RFM69_LOWBAT_TRIM_1976				0x04
#define RFM69_LOWBAT_TRIM_2045				0x05
#define RFM69_LOWBAT_TRIM_2116				0x06
#define RFM69_LOWBAT_TRIM_2185				0x07


// RegListen1
#define RFM69_LISTEN1_RESOL_64				0x50
#define RFM69_LISTEN1_RESOL_4100			0xA0  // Default
#define RFM69_LISTEN1_RESOL_262000		0xF0

#define RFM69_LISTEN1_CRITERIA_RSSI				  0x00  // Default
#define RFM69_LISTEN1_CRITERIA_RSSIANDSYNC	  0x08

#define RFM69_LISTEN1_END_00						      0x00
#define RFM69_LISTEN1_END_01						      0x02  // Default
#define RFM69_LISTEN1_END_10						      0x04


// RegListen2
#define RFM69_LISTEN2_COEFIDLE_VALUE				0xF5 // Default

// RegListen3
#define RFM69_LISTEN3_COEFRX_VALUE					0x20 // Default

// RegPaLevel
#define RFM69_PALEVEL_PA0_ON		  0x80  // Default
#define RFM69_PALEVEL_PA0_OFF		0x00
#define RFM69_PALEVEL_PA1_ON			0x40
#define RFM69_PALEVEL_PA1_OFF		0x00  // Default
#define RFM69_PALEVEL_PA2_ON			0x20
#define RFM69_PALEVEL_PA2_OFF		0x00  // Default

#define RFM69_PALEVEL_OUTPUTPOWER_00000			0x00
#define RFM69_PALEVEL_OUTPUTPOWER_00001			0x01
#define RFM69_PALEVEL_OUTPUTPOWER_00010			0x02
#define RFM69_PALEVEL_OUTPUTPOWER_00011			0x03
#define RFM69_PALEVEL_OUTPUTPOWER_00100			0x04
#define RFM69_PALEVEL_OUTPUTPOWER_00101			0x05
#define RFM69_PALEVEL_OUTPUTPOWER_00110			0x06
#define RFM69_PALEVEL_OUTPUTPOWER_00111			0x07
#define RFM69_PALEVEL_OUTPUTPOWER_01000			0x08
#define RFM69_PALEVEL_OUTPUTPOWER_01001			0x09
#define RFM69_PALEVEL_OUTPUTPOWER_01010			0x0A
#define RFM69_PALEVEL_OUTPUTPOWER_01011			0x0B
#define RFM69_PALEVEL_OUTPUTPOWER_01100			0x0C
#define RFM69_PALEVEL_OUTPUTPOWER_01101			0x0D
#define RFM69_PALEVEL_OUTPUTPOWER_01110			0x0E
#define RFM69_PALEVEL_OUTPUTPOWER_01111			0x0F
#define RFM69_PALEVEL_OUTPUTPOWER_10000			0x10
#define RFM69_PALEVEL_OUTPUTPOWER_10001			0x11
#define RFM69_PALEVEL_OUTPUTPOWER_10010			0x12
#define RFM69_PALEVEL_OUTPUTPOWER_10011			0x13
#define RFM69_PALEVEL_OUTPUTPOWER_10100			0x14
#define RFM69_PALEVEL_OUTPUTPOWER_10101			0x15
#define RFM69_PALEVEL_OUTPUTPOWER_10110			0x16
#define RFM69_PALEVEL_OUTPUTPOWER_10111			0x17
#define RFM69_PALEVEL_OUTPUTPOWER_11000			0x18
#define RFM69_PALEVEL_OUTPUTPOWER_11001			0x19
#define RFM69_PALEVEL_OUTPUTPOWER_11010			0x1A
#define RFM69_PALEVEL_OUTPUTPOWER_11011			0x1B
#define RFM69_PALEVEL_OUTPUTPOWER_11100			0x1C
#define RFM69_PALEVEL_OUTPUTPOWER_11101			0x1D
#define RFM69_PALEVEL_OUTPUTPOWER_11110			0x1E
#define RFM69_PALEVEL_OUTPUTPOWER_11111			0x1F  // Default


// RegPaRamp
#define RFM69_PARAMP_3400						0x00
#define RFM69_PARAMP_2000						0x01
#define RFM69_PARAMP_1000						0x02
#define RFM69_PARAMP_500							0x03
#define RFM69_PARAMP_250							0x04
#define RFM69_PARAMP_125							0x05
#define RFM69_PARAMP_100							0x06
#define RFM69_PARAMP_62							0x07
#define RFM69_PARAMP_50							0x08
#define RFM69_PARAMP_40							0x09  // Default
#define RFM69_PARAMP_31							0x0A
#define RFM69_PARAMP_25							0x0B
#define RFM69_PARAMP_20							0x0C
#define RFM69_PARAMP_15							0x0D
#define RFM69_PARAMP_12							0x0E
#define RFM69_PARAMP_10							0x0F


// RegOcp
#define RFM69_OCP_OFF								0x0F
#define RFM69_OCP_ON								  0x1A  // Default

#define RFM69_OCP_TRIM_45						0x00
#define RFM69_OCP_TRIM_50						0x01
#define RFM69_OCP_TRIM_55						0x02
#define RFM69_OCP_TRIM_60						0x03
#define RFM69_OCP_TRIM_65						0x04
#define RFM69_OCP_TRIM_70						0x05
#define RFM69_OCP_TRIM_75						0x06
#define RFM69_OCP_TRIM_80						0x07
#define RFM69_OCP_TRIM_85						0x08
#define RFM69_OCP_TRIM_90						0x09
#define RFM69_OCP_TRIM_95						0x0A
#define RFM69_OCP_TRIM_100						0x0B  // Default
#define RFM69_OCP_TRIM_105						0x0C
#define RFM69_OCP_TRIM_110						0x0D
#define RFM69_OCP_TRIM_115						0x0E
#define RFM69_OCP_TRIM_120						0x0F


// RegAgcRef
#define RFM69_AGCREF_AUTO_ON					0x40  // Default
#define RFM69_AGCREF_AUTO_OFF				0x00

#define RFM69_AGCREF_LEVEL_MINUS80		0x00  // Default
#define RFM69_AGCREF_LEVEL_MINUS81		0x01
#define RFM69_AGCREF_LEVEL_MINUS82		0x02
#define RFM69_AGCREF_LEVEL_MINUS83		0x03
#define RFM69_AGCREF_LEVEL_MINUS84		0x04
#define RFM69_AGCREF_LEVEL_MINUS85		0x05
#define RFM69_AGCREF_LEVEL_MINUS86		0x06
#define RFM69_AGCREF_LEVEL_MINUS87		0x07
#define RFM69_AGCREF_LEVEL_MINUS88		0x08
#define RFM69_AGCREF_LEVEL_MINUS89		0x09
#define RFM69_AGCREF_LEVEL_MINUS90		0x0A
#define RFM69_AGCREF_LEVEL_MINUS91		0x0B
#define RFM69_AGCREF_LEVEL_MINUS92		0x0C
#define RFM69_AGCREF_LEVEL_MINUS93		0x0D
#define RFM69_AGCREF_LEVEL_MINUS94		0x0E
#define RFM69_AGCREF_LEVEL_MINUS95		0x0F
#define RFM69_AGCREF_LEVEL_MINUS96		0x10
#define RFM69_AGCREF_LEVEL_MINUS97		0x11
#define RFM69_AGCREF_LEVEL_MINUS98		0x12
#define RFM69_AGCREF_LEVEL_MINUS99		0x13
#define RFM69_AGCREF_LEVEL_MINUS100	0x14
#define RFM69_AGCREF_LEVEL_MINUS101	0x15
#define RFM69_AGCREF_LEVEL_MINUS102	0x16
#define RFM69_AGCREF_LEVEL_MINUS103	0x17
#define RFM69_AGCREF_LEVEL_MINUS104	0x18
#define RFM69_AGCREF_LEVEL_MINUS105	0x19
#define RFM69_AGCREF_LEVEL_MINUS106	0x1A
#define RFM69_AGCREF_LEVEL_MINUS107	0x1B
#define RFM69_AGCREF_LEVEL_MINUS108	0x1C
#define RFM69_AGCREF_LEVEL_MINUS109	0x1D
#define RFM69_AGCREF_LEVEL_MINUS110	0x1E
#define RFM69_AGCREF_LEVEL_MINUS111	0x1F
#define RFM69_AGCREF_LEVEL_MINUS112	0x20
#define RFM69_AGCREF_LEVEL_MINUS113	0x21
#define RFM69_AGCREF_LEVEL_MINUS114	0x22
#define RFM69_AGCREF_LEVEL_MINUS115	0x23
#define RFM69_AGCREF_LEVEL_MINUS116	0x24
#define RFM69_AGCREF_LEVEL_MINUS117	0x25
#define RFM69_AGCREF_LEVEL_MINUS118	0x26
#define RFM69_AGCREF_LEVEL_MINUS119	0x27
#define RFM69_AGCREF_LEVEL_MINUS120	0x28
#define RFM69_AGCREF_LEVEL_MINUS121	0x29
#define RFM69_AGCREF_LEVEL_MINUS122	0x2A
#define RFM69_AGCREF_LEVEL_MINUS123	0x2B
#define RFM69_AGCREF_LEVEL_MINUS124	0x2C
#define RFM69_AGCREF_LEVEL_MINUS125	0x2D
#define RFM69_AGCREF_LEVEL_MINUS126	0x2E
#define RFM69_AGCREF_LEVEL_MINUS127	0x2F
#define RFM69_AGCREF_LEVEL_MINUS128	0x30
#define RFM69_AGCREF_LEVEL_MINUS129	0x31
#define RFM69_AGCREF_LEVEL_MINUS130	0x32
#define RFM69_AGCREF_LEVEL_MINUS131	0x33
#define RFM69_AGCREF_LEVEL_MINUS132	0x34
#define RFM69_AGCREF_LEVEL_MINUS133	0x35
#define RFM69_AGCREF_LEVEL_MINUS134	0x36
#define RFM69_AGCREF_LEVEL_MINUS135	0x37
#define RFM69_AGCREF_LEVEL_MINUS136	0x38
#define RFM69_AGCREF_LEVEL_MINUS137	0x39
#define RFM69_AGCREF_LEVEL_MINUS138	0x3A
#define RFM69_AGCREF_LEVEL_MINUS139	0x3B
#define RFM69_AGCREF_LEVEL_MINUS140	0x3C
#define RFM69_AGCREF_LEVEL_MINUS141	0x3D
#define RFM69_AGCREF_LEVEL_MINUS142	0x3E
#define RFM69_AGCREF_LEVEL_MINUS143	0x3F


// RegAgcThresh1
#define RFM69_AGCTHRESH1_SNRMARGIN_000		0x00
#define RFM69_AGCTHRESH1_SNRMARGIN_001		0x20
#define RFM69_AGCTHRESH1_SNRMARGIN_010		0x40
#define RFM69_AGCTHRESH1_SNRMARGIN_011		0x60
#define RFM69_AGCTHRESH1_SNRMARGIN_100		0x80
#define RFM69_AGCTHRESH1_SNRMARGIN_101		0xA0  // Default
#define RFM69_AGCTHRESH1_SNRMARGIN_110		0xC0
#define RFM69_AGCTHRESH1_SNRMARGIN_111		0xE0

#define RFM69_AGCTHRESH1_STEP1_0					0x00
#define RFM69_AGCTHRESH1_STEP1_1					0x01
#define RFM69_AGCTHRESH1_STEP1_2					0x02
#define RFM69_AGCTHRESH1_STEP1_3					0x03
#define RFM69_AGCTHRESH1_STEP1_4					0x04
#define RFM69_AGCTHRESH1_STEP1_5					0x05
#define RFM69_AGCTHRESH1_STEP1_6					0x06
#define RFM69_AGCTHRESH1_STEP1_7					0x07
#define RFM69_AGCTHRESH1_STEP1_8					0x08
#define RFM69_AGCTHRESH1_STEP1_9					0x09
#define RFM69_AGCTHRESH1_STEP1_10				0x0A
#define RFM69_AGCTHRESH1_STEP1_11				0x0B
#define RFM69_AGCTHRESH1_STEP1_12				0x0C
#define RFM69_AGCTHRESH1_STEP1_13				0x0D
#define RFM69_AGCTHRESH1_STEP1_14				0x0E
#define RFM69_AGCTHRESH1_STEP1_15				0x0F
#define RFM69_AGCTHRESH1_STEP1_16				0x10  // Default
#define RFM69_AGCTHRESH1_STEP1_17				0x11
#define RFM69_AGCTHRESH1_STEP1_18				0x12
#define RFM69_AGCTHRESH1_STEP1_19				0x13
#define RFM69_AGCTHRESH1_STEP1_20				0x14
#define RFM69_AGCTHRESH1_STEP1_21				0x15
#define RFM69_AGCTHRESH1_STEP1_22				0x16
#define RFM69_AGCTHRESH1_STEP1_23				0x17
#define RFM69_AGCTHRESH1_STEP1_24				0x18
#define RFM69_AGCTHRESH1_STEP1_25				0x19
#define RFM69_AGCTHRESH1_STEP1_26				0x1A
#define RFM69_AGCTHRESH1_STEP1_27				0x1B
#define RFM69_AGCTHRESH1_STEP1_28				0x1C
#define RFM69_AGCTHRESH1_STEP1_29				0x1D
#define RFM69_AGCTHRESH1_STEP1_30				0x1E
#define RFM69_AGCTHRESH1_STEP1_31				0x1F


// RegAgcThresh2
#define RFM69_AGCTHRESH2_STEP2_0					0x00
#define RFM69_AGCTHRESH2_STEP2_1					0x10
#define RFM69_AGCTHRESH2_STEP2_2					0x20
#define RFM69_AGCTHRESH2_STEP2_3					0x30  // XXX wrong -- Default
#define RFM69_AGCTHRESH2_STEP2_4					0x40
#define RFM69_AGCTHRESH2_STEP2_5					0x50
#define RFM69_AGCTHRESH2_STEP2_6					0x60
#define RFM69_AGCTHRESH2_STEP2_7					0x70	// default
#define RFM69_AGCTHRESH2_STEP2_8					0x80
#define RFM69_AGCTHRESH2_STEP2_9					0x90
#define RFM69_AGCTHRESH2_STEP2_10				0xA0
#define RFM69_AGCTHRESH2_STEP2_11				0xB0
#define RFM69_AGCTHRESH2_STEP2_12				0xC0
#define RFM69_AGCTHRESH2_STEP2_13				0xD0
#define RFM69_AGCTHRESH2_STEP2_14				0xE0
#define RFM69_AGCTHRESH2_STEP2_15				0xF0

#define RFM69_AGCTHRESH2_STEP3_0					0x00
#define RFM69_AGCTHRESH2_STEP3_1					0x01
#define RFM69_AGCTHRESH2_STEP3_2					0x02
#define RFM69_AGCTHRESH2_STEP3_3					0x03
#define RFM69_AGCTHRESH2_STEP3_4					0x04
#define RFM69_AGCTHRESH2_STEP3_5					0x05
#define RFM69_AGCTHRESH2_STEP3_6					0x06
#define RFM69_AGCTHRESH2_STEP3_7					0x07
#define RFM69_AGCTHRESH2_STEP3_8					0x08
#define RFM69_AGCTHRESH2_STEP3_9					0x09
#define RFM69_AGCTHRESH2_STEP3_10				0x0A
#define RFM69_AGCTHRESH2_STEP3_11				0x0B  // Default
#define RFM69_AGCTHRESH2_STEP3_12				0x0C
#define RFM69_AGCTHRESH2_STEP3_13				0x0D
#define RFM69_AGCTHRESH2_STEP3_14				0x0E
#define RFM69_AGCTHRESH2_STEP3_15				0x0F


// RegAgcThresh3
#define RFM69_AGCTHRESH3_STEP4_0					0x00
#define RFM69_AGCTHRESH3_STEP4_1					0x10
#define RFM69_AGCTHRESH3_STEP4_2					0x20
#define RFM69_AGCTHRESH3_STEP4_3					0x30
#define RFM69_AGCTHRESH3_STEP4_4					0x40
#define RFM69_AGCTHRESH3_STEP4_5					0x50
#define RFM69_AGCTHRESH3_STEP4_6					0x60
#define RFM69_AGCTHRESH3_STEP4_7					0x70
#define RFM69_AGCTHRESH3_STEP4_8					0x80
#define RFM69_AGCTHRESH3_STEP4_9					0x90  // Default
#define RFM69_AGCTHRESH3_STEP4_10				0xA0
#define RFM69_AGCTHRESH3_STEP4_11				0xB0
#define RFM69_AGCTHRESH3_STEP4_12				0xC0
#define RFM69_AGCTHRESH3_STEP4_13				0xD0
#define RFM69_AGCTHRESH3_STEP4_14				0xE0
#define RFM69_AGCTHRESH3_STEP4_15				0xF0

#define RFM69_AGCTHRESH3_STEP5_0					0x00
#define RFM69_AGCTHRESH3_STEP5_1					0x01
#define RFM69_AGCTHRESH3_STEP5_2					0x02
#define RFM69_AGCTHRESH3_STEP5_3					0x03
#define RFM69_AGCTHRESH3_STEP5_4					0x04
#define RFM69_AGCTHRESH3_STEP5_5					0x05
#define RFM69_AGCTHRESH3_STEP5_6					0x06
#define RFM69_AGCTHRESH3_STEP5_7					0x07
#define RFM69_AGCTHRES33_STEP5_8					0x08
#define RFM69_AGCTHRESH3_STEP5_9					0x09
#define RFM69_AGCTHRESH3_STEP5_10				0x0A
#define RFM69_AGCTHRESH3_STEP5_11				0x0B  // Default
#define RFM69_AGCTHRESH3_STEP5_12				0x0C
#define RFM69_AGCTHRESH3_STEP5_13				0x0D
#define RFM69_AGCTHRESH3_STEP5_14				0x0E
#define RFM69_AGCTHRESH3_STEP5_15				0x0F


// RegLna
#define RFM69_LNA_ZIN_50							    0x00
#define RFM69_LNA_ZIN_200							  0x80  // Default

#define RFM69_LNA_LOWPOWER_OFF						0x00  // Default
#define RFM69_LNA_LOWPOWER_ON						0x40

#define RFM69_LNA_CURRENTGAIN						0x08

#define RFM69_LNA_GAINSELECT_AUTO				0x00  // Default
#define RFM69_LNA_GAINSELECT_MAX					0x01
#define RFM69_LNA_GAINSELECT_MAXMINUS6		0x02
#define RFM69_LNA_GAINSELECT_MAXMINUS12	0x03
#define RFM69_LNA_GAINSELECT_MAXMINUS24	0x04
#define RFM69_LNA_GAINSELECT_MAXMINUS36	0x05
#define RFM69_LNA_GAINSELECT_MAXMINUS48	0x06


// RegRxBw
#define RFM69_RXBW_DCCFREQ_000						0x00
#define RFM69_RXBW_DCCFREQ_001						0x20
#define RFM69_RXBW_DCCFREQ_010						0x40  // Default
#define RFM69_RXBW_DCCFREQ_011						0x60
#define RFM69_RXBW_DCCFREQ_100						0x80
#define RFM69_RXBW_DCCFREQ_101						0xA0
#define RFM69_RXBW_DCCFREQ_110						0xC0
#define RFM69_RXBW_DCCFREQ_111						0xE0

#define RFM69_RXBW_MANT_16							  0x00
#define RFM69_RXBW_MANT_20							  0x08
#define RFM69_RXBW_MANT_24							  0x10  // Default

#define RFM69_RXBW_EXP_0							    0x00
#define RFM69_RXBW_EXP_1		  			  		0x01
#define RFM69_RXBW_EXP_2			  	  			0x02
#define RFM69_RXBW_EXP_3				    			0x03
#define RFM69_RXBW_EXP_4			  		  		0x04
#define RFM69_RXBW_EXP_5		  				  	0x05  // Default
#define RFM69_RXBW_EXP_6	  						  0x06
#define RFM69_RXBW_EXP_7						  	  0x07


// RegAfcBw
#define RFM69_AFCBW_DCCFREQAFC_000				0x00
#define RFM69_AFCBW_DCCFREQAFC_001				0x20
#define RFM69_AFCBW_DCCFREQAFC_010				0x40
#define RFM69_AFCBW_DCCFREQAFC_011				0x60
#define RFM69_AFCBW_DCCFREQAFC_100				0x80  // Default
#define RFM69_AFCBW_DCCFREQAFC_101				0xA0
#define RFM69_AFCBW_DCCFREQAFC_110				0xC0
#define RFM69_AFCBW_DCCFREQAFC_111				0xE0

#define RFM69_AFCBW_MANTAFC_16						0x00
#define RFM69_AFCBW_MANTAFC_20						0x08  // Default
#define RFM69_AFCBW_MANTAFC_24						0x10

#define RFM69_AFCBW_EXPAFC_0						  0x00
#define RFM69_AFCBW_EXPAFC_1	  					0x01
#define RFM69_AFCBW_EXPAFC_2		  				0x02
#define RFM69_AFCBW_EXPAFC_3			  			0x03  // Default
#define RFM69_AFCBW_EXPAFC_4				  		0x04
#define RFM69_AFCBW_EXPAFC_5					  	0x05
#define RFM69_AFCBW_EXPAFC_6						  0x06
#define RFM69_AFCBW_EXPAFC_7					  	0x07


// RegOokPeak
#define RFM69_OOKPEAK_THRESHTYPE_FIXED				0x00
#define RFM69_OOKPEAK_THRESHTYPE_PEAK				0x40  // Default
#define RFM69_OOKPEAK_THRESHTYPE_AVERAGE			0x80

#define RFM69_OOKPEAK_PEAKTHRESHSTEP_000			0x00  // Default
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_001			0x08
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_010			0x10
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_011			0x18
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_100			0x20
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_101			0x28
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_110			0x30
#define RFM69_OOKPEAK_PEAKTHRESHSTEP_111			0x38

#define RFM69_OOKPEAK_PEAKTHRESHDEC_000			0x00  // Default
#define RFM69_OOKPEAK_PEAKTHRESHDEC_001			0x01
#define RFM69_OOKPEAK_PEAKTHRESHDEC_010			0x02
#define RFM69_OOKPEAK_PEAKTHRESHDEC_011			0x03
#define RFM69_OOKPEAK_PEAKTHRESHDEC_100			0x04
#define RFM69_OOKPEAK_PEAKTHRESHDEC_101			0x05
#define RFM69_OOKPEAK_PEAKTHRESHDEC_110			0x06
#define RFM69_OOKPEAK_PEAKTHRESHDEC_111			0x07


// RegOokAvg
#define RFM69_OOKAVG_AVERAGETHRESHFILT_00		0x00
#define RFM69_OOKAVG_AVERAGETHRESHFILT_01		0x40
#define RFM69_OOKAVG_AVERAGETHRESHFILT_10		0x80  // Default
#define RFM69_OOKAVG_AVERAGETHRESHFILT_11		0xC0


// RegOokFix
#define RFM69_OOKFIX_FIXEDTHRESH_VALUE				0x06  // Default


// RegAfcFei
#define RFM69_AFCFEI_FEI_DONE						    0x40
#define RFM69_AFCFEI_FEI_START						    0x20
#define RFM69_AFCFEI_AFC_DONE						    0x10
#define RFM69_AFCFEI_AFCAUTOCLEAR_ON			  	0x08
#define RFM69_AFCFEI_AFCAUTOCLEAR_OFF				0x00  // Default

#define RFM69_AFCFEI_AFCAUTO_ON					    0x04
#define RFM69_AFCFEI_AFCAUTO_OFF					    0x00  // Default

#define RFM69_AFCFEI_AFC_CLEAR						    0x02
#define RFM69_AFCFEI_AFC_START						    0x01

// RegRssiConfig
#define RFM69_RSSI_FASTRX_ON						      0x08
#define RFM69_RSSI_FASTRX_OFF						    0x00  // Default
#define RFM69_RSSI_DONE							        0x02
#define RFM69_RSSI_START							        0x01

/*
// RegDioMapping1
#define RFM69_DIOMAPPING1_DIO0_00	  				0x00  // Default
#define RFM69_DIOMAPPING1_DIO0_01		  			0x40
#define RFM69_DIOMAPPING1_DIO0_10			  		0x80
#define RFM69_DIOMAPPING1_DIO0_11				  	0xC0

#define RFM69_DIOMAPPING1_DIO1_00   					0x00  // Default
#define RFM69_DIOMAPPING1_DIO1_01		  			0x10
#define RFM69_DIOMAPPING1_DIO1_10			  		0x20
#define RFM69_DIOMAPPING1_DIO1_11				  	0x30

#define RFM69_DIOMAPPING1_DIO2_00	  				0x00  // Default
#define RFM69_DIOMAPPING1_DIO2_01		  			0x04
#define RFM69_DIOMAPPING1_DIO2_10			  		0x08
#define RFM69_DIOMAPPING1_DIO2_11				  	0x0C

#define RFM69_DIOMAPPING1_DIO3_00	  				0x00  // Default
#define RFM69_DIOMAPPING1_DIO3_01		  			0x01
#define RFM69_DIOMAPPING1_DIO3_10			  		0x02
#define RFM69_DIOMAPPING1_DIO3_11				  	0x03


// RegDioMapping2
#define RFM69_DIOMAPPING2_DIO4_00	  				0x00  // Default
#define RFM69_DIOMAPPING2_DIO4_01		  			0x40
#define RFM69_DIOMAPPING2_DIO4_10			  		0x80
#define RFM69_DIOMAPPING2_DIO4_11				  	0xC0

#define RFM69_DIOMAPPING2_DIO5_00	  				0x00  // Default
#define RFM69_DIOMAPPING2_DIO5_01		  			0x10
#define RFM69_DIOMAPPING2_DIO5_10			  		0x20
#define RFM69_DIOMAPPING2_DIO5_11				  	0x30

#define RFM69_DIOMAPPING2_CLKOUT_32	  			0x00
#define RFM69_DIOMAPPING2_CLKOUT_16		  		0x01
#define RFM69_DIOMAPPING2_CLKOUT_8				  	0x02
#define RFM69_DIOMAPPING2_CLKOUT_4					  0x03
#define RFM69_DIOMAPPING2_CLKOUT_2		  			0x04
#define RFM69_DIOMAPPING2_CLKOUT_1			  		0x05
#define RFM69_DIOMAPPING2_CLKOUT_RC			  	0x06
#define RFM69_DIOMAPPING2_CLKOUT_OFF				  0x07  // Default


// RegIrqFlags1
#define RFM69_IRQFLAGS1_MODEREADY					  0x80
#define RFM69_IRQFLAGS1_RXREADY					    0x40
#define RFM69_IRQFLAGS1_TXREADY					    0x20
#define RFM69_IRQFLAGS1_PLLLOCK					    0x10
#define RFM69_IRQFLAGS1_RSSI						      0x08
#define RFM69_IRQFLAGS1_TIMEOUT					    0x04
#define RFM69_IRQFLAGS1_AUTOMODE					    0x02
#define RFM69_IRQFLAGS1_SYNCADDRESSMATCH			0x01

// RegIrqFlags2
#define RFM69_IRQFLAGS2_FIFOFULL					    0x80
#define RFM69_IRQFLAGS2_FIFONOTEMPTY				  0x40
#define RFM69_IRQFLAGS2_FIFOLEVEL					  0x20
#define RFM69_IRQFLAGS2_FIFOOVERRUN				  0x10
#define RFM69_IRQFLAGS2_PACKETSENT					  0x08
#define RFM69_IRQFLAGS2_PAYLOADREADY				  0x04
#define RFM69_IRQFLAGS2_CRCOK						    0x02
#define RFM69_IRQFLAGS2_LOWBAT						    0x01
*/



/*
// RegAesKey1-16
#define RFM69_AESKEY1_VALUE						0x00  // Default
#define RFM69_AESKEY2_VALUE						0x00  // Default
#define RFM69_AESKEY3_VALUE						0x00  // Default
#define RFM69_AESKEY4_VALUE						0x00  // Default
#define RFM69_AESKEY5_VALUE						0x00  // Default
#define RFM69_AESKEY6_VALUE						0x00  // Default
#define RFM69_AESKEY7_VALUE						0x00  // Default
#define RFM69_AESKEY8_VALUE						0x00  // Default
#define RFM69_AESKEY9_VALUE						0x00  // Default
#define RFM69_AESKEY10_VALUE						0x00  // Default
#define RFM69_AESKEY11_VALUE						0x00  // Default
#define RFM69_AESKEY12_VALUE						0x00  // Default
#define RFM69_AESKEY13_VALUE						0x00  // Default
#define RFM69_AESKEY14_VALUE						0x00  // Default
#define RFM69_AESKEY15_VALUE						0x00  // Default
#define RFM69_AESKEY16_VALUE						0x00  // Default
*/

// RegTemp1
#define RFM69_TEMP1_MEAS_START					0x08
#define RFM69_TEMP1_MEAS_RUNNING				0x04
#define RFM69_TEMP1_ADCLOWPOWER_ON			0x01  // Default
#define RFM69_TEMP1_ADCLOWPOWER_OFF		0x00

// RegTestDagc 0x6F: demodulator config and IO mode config
#define RFM69_DAGC_NORMAL              0x00  // Reset value
#define RFM69_DAGC_IMPROVED_LOWBETA1   0x20  //
#define RFM69_DAGC_IMPROVED_LOWBETA0   0x30  // Recommended default




// RFM69_REG_TESTPA1
#define RFM69_TESTPA1_NORMAL                              0x55
#define RFM69_TESTPA1_BOOST                               0x5d

// RFM69_REG_TESTPA2
#define RFM69_TESTPA2_NORMAL                              0x70
#define RFM69_TESTPA2_BOOST                               0x7c

// RFM69_REG_TESTDAGC
#define RFM69_TESTDAGC_CONTINUOUSDAGC_NORMAL              0x00
#define RFM69_TESTDAGC_CONTINUOUSDAGC_IMPROVED_LOWBETAON  0x20
#define RFM69_TESTDAGC_CONTINUOUSDAGC_IMPROVED_LOWBETAOFF 0x30


// RFM69_REG_OSC1
#define RFM69_OSC1_RCCAL_START       0x80
#define RFM69_OSC1_RCCAL_DONE        0x40

#define RFM69_COARSE_TEMP_COEF    							-90

#define RFM69_FIFO_SIZE										66
#define RFM69_MAX_ENCRYPTABLE_PAYLOAD_LEN 					64
#define RFM69_HEADER_LEN 									4
#define RFM69_BROADCAST_ADDRESS 							0xff
#define RFM69_MAX_MESSAGE_LEN 								(RFM69_MAX_ENCRYPTABLE_PAYLOAD_LEN - RFM69_HEADER_LEN)

// The crystal oscillator frequency of the RF69 module
#define RFM69_FXOSC 										32000000.

// The Frequency Synthesizer step = RH_RF69_FXOSC / 2^^19
#define RFM69_FSTEP  										(RFM69_FXOSC/524288.)

#define SPI_READ_REG(REG)									((REG) & 0x7F)
#define SPI_WRITE_REG(REG)									((REG) | 0x80)

#endif // defines

#include <stdint.h>
#include <stdbool.h>

#include "defs.h"


bool rfm69io_init(void);
void rfm69io_deinit(void);

uint8_t rfm69_readReg(uint8_t addr);
void rfm69_writeReg(uint8_t addr, uint8_t value);
//void rfm69_writeRegBurst(uint8_t addr, const uint8_t* vals, int len);
void rfm69_writeFifo(const uint8_t* vals, int len);
void rfm69_readFifo(uint8_t* vals, int len);
bool rfm69_waitForRegBitSet(uint8_t reg, uint8_t mask, int timeout_ms);
bool rfm69_waitForRegBitClear(uint8_t reg, uint8_t mask, int timeout_ms);

bool rfm69_waitForIrq(int timeout_secs, int timeout_us);

void rfm69_setFrequency(uint32_t kHz);
void rfm69_setDataRate(uint32_t dataRate);
void rfm69_clearFifo(void); 
void rfm69_setOpMode(uint8_t opMode);
void rfm69_powerDown(void);
void rfm69_standBy(void);
void rfm69_enableReceiver(void);
void rfm69_enableTransmitter(void);
int8_t rfm69_readTemperature(int8_t CalibrationTempVal);

bool rfm69_test_reg_rw(void);

#if (defined __RASPI__)
extern int rfm69_irq_gpionum;
//extern int rfm69_debuglevel;
extern int verbosity;
//#define RFM69_VERBOSITY(level)			{ rfm69_debuglevel = (level); }
#define _RFMDBG(level, format, ...)			{ if (verbosity > (level)) fprintf(stderr, "rfm69: "format "\n", ## __VA_ARGS__); }
#endif

#endif // #ifndef RFM69IO_h
