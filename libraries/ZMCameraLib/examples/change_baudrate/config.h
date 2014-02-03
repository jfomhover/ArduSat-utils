/*
    Description :  automatic configuration of Serial/SoftwareSerial/SD parameters
                   depending on the Board you choose in ArduinoIDE->Tools->Board
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __AVR_ATmega2560__    // Arduino Mega 2560
// *** ON THE MEGA, USE SOFTWARE SERIAL ***

    #define SERIAL_USEHARDWARESERIAL
    #define SERIAL_PORT  Serial1
    #define SERIAL_BAUDRATE  115200

    #define CS_PIN  4
    #define SS_PIN  53
#endif


#ifdef __AVR_ATmega328P__    // Arduino Uno (Ethernet)
// *** ON UNO, USE SOFTWARE SERIAL ***

    #define SERIAL_USESOFTWARE
    #define SERIAL_SOFT_RX  7
    #define SERIAL_SOFT_TX  8
    #define SERIAL_PORT  mySerial
    #define SERIAL_BAUDRATE  9600  // BEWARE, SoftwareSerial can't handle baudrate over 38400

    #define CS_PIN  4
    #define SS_PIN  10

    SoftwareSerial mySerial(SERIAL_SOFT_RX, SERIAL_SOFT_TX); // RX, TX
#endif

/* UNTESTED CONFIGURATION
#ifdef __AVR_ATmega1280__    // Arduino Mega
// *** ON THE MEGA, USE SOFTWARE SERIAL ***

    #define SERIAL_USEHARDWARESERIAL
    #define SERIAL_PORT  Serial1
    #define SERIAL_BAUDRATE  115200

    #define CS_PIN  4
    #define SS_PIN  53
#endif
*/

/* UNKNOWN CONFIG
#ifdef __AVR_ATmega168__    // Arduino Decimilia and older
#endif
*/

/* UNKNOWN CONFIG
#ifdef __AVR_ATmega32U4__    // Arduino Leonardo
#endif
*/

/* UNKNOWN CONFIG
#ifdef __SAM3X8E__           // Arduino Due
#endif
*/


#endif /* _CONFIG_H_ */
