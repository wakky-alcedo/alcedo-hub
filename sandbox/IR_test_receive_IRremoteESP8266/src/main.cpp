/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino (or later) example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>


const uint8_t IR_SEND_PIN = 32;
const uint16_t kRecvPin = 33;

IRrecv irrecv(kRecvPin);
IRsend irsend=IRsend(IR_SEND_PIN);  // Set the GPIO to be used.


decode_results results;

void setup() {
    Serial.begin(115200);

    irsend.begin();

    irrecv.enableIRIn();  // Start the receiver
    while (!Serial)  // Wait for the serial connection to be establised.
        delay(50);
    Serial.println();
    Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
    Serial.println(kRecvPin);
}

void loop() {
    if (irrecv.decode(&results)) {

        // uint32_t address = 0x4004; 0b100000000000100
        // uint32_t command = 0x7200000; 0b111001000000000000000000000
        // // uint64_t data = 0x400407200000;
        // uint64_t data = 0x600000004E02002; // 0b11000000000000000000000000000000100111000000010000000000010
        uint64_t data = 0x200203900000003; // 0b01000000000001000000011100100000000000000000000000000000011
        // // irsend.sendPanasonic(address, command, 48, 0);
        irsend.sendPanasonic64(data, 64, 0);

        // print() & println() can't handle printing long longs. (uint64_t)
        Serial.println(resultToHumanReadableBasic(&results));
        Serial.println(resultToHexidecimal(&results));
        serialPrintUint64(results.value, HEX);
        //  = resultToRawArray(&results);
            
        // Serial.println();
        Serial.println(resultToTimingInfo(&results));
        Serial.println(resultToSourceCode(&results));
        Serial.println("--------------------------------");
        irrecv.resume();  // Receive the next value
    }
}
