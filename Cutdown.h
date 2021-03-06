/*
 * Cutdown.h
 * XBEE_ADDR must be defined in client code.
 * will throw compile error otherwise
 */

#ifndef Cutdown_h
#define Cutdown_h

#include <Arduino.h>
#include <Wire.h>
#include <ccsds_xbee.h>

#ifndef XBEE_ADDR
#define XBEE_ADDR 04
#endif

// physical definitions
#define ARMED_LED_PIN 13
#define TLM_ADDR 02     // XBee channel of LINK
#define XBEE_PAN_ID 0x0B0B
#define ARM_FCNCODE 0x0A
#define ARM_STATUS_FCNCODE 0x01
#define DISARM_FCNCODE 0x0D
#define FIRE_FCNCODE 0x0F

// behavioral constants
#define CYCLE_DELAY 100     // time between execution cycles [ms]
#define ARM_TIMEOUT (60000 / CYCLE_DELAY)

/* response definitions */
#define INIT_RESPONSE 0xAC
#define READ_FAIL_RESPONSE 0xAF
#define BAD_COMMAND_RESPONSE 0xBB
#define ARMED_RESPONSE 0xAA
#define DISARMED_RESPONSE 0xDD
#define FIRED_RESPONSE 0xFF

class Cutdown
{
    public:
        void begin();
        void check_input();
        void arm_system();
        void disarm_system();
        bool system_is_armed();
        void send_release_confirmation();
        void log(String message);
        boolean release;
    private:
        void read_input();
        void command_response(uint8_t _fcn_code, uint8_t data[],
                uint8_t length);
        void one_byte_message(uint8_t msg);
        boolean armed;
        int pkt_type;
        int bytes_read;
        uint8_t incoming_bytes[100];
        uint8_t fcn_code;
        uint8_t tlm_pos;
        uint8_t tlm_data[1];
        int armed_ctr;    // counter tracking number of cycles system has been armed
};

#endif
