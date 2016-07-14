#include <Wire.h>
#include <XBee.h>
#include <CCSDS_Xbee/ccsds_xbee.h>

// physical definitions
#define TRIGGER_PIN 3
#define ARMED_LED_PIN 13
// XBEE_ADDR must be defined in client code, will throw compile error otherwise
// #define XBEE_ADDR 03
#define TLM_ADDR 02
#define XBEE_PAN_ID 0x0B0B
#define ARM_FCNCODE 0x0A
#define ARM_STATUS_FCNCODE 0x01
#define DISARM_FCNCODE 0x0D
#define FIRE_FCNCODE 0x0F

// behavioral constants
#define CYCLE_DELAY 100 // time between execution cycles [ms]
#define ARM_TIMEOUT (60000/CYCLE_DELAY) // 60 * 1000 / CYCLE_DELAY

// function prototypes
class Cutdown
{
    public:
        char begin();
        void read_input();
        void arm_system();
        void disarm_system();
        bool system_is_armed();
        void send_release_confirmation();
    private:
        void command_response(uint8_t _fcn_code, uint8_t data[], uint8_t length);
        boolean armed;
        int pkt_type;
        int bytes_read;
        uint8_t incoming_bytes[100];
        uint8_t fcn_code;
        uint8_t tlm_pos;
        uint8_t tlm_data[1];
        int armed_ctr;    // counter tracking number of cycles system has been armed
}
