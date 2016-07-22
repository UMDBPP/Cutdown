/*
 * Cutdown.cpp
 * You must write code to check the boolean value "release" and release
 */

#include "Cutdown.h"

char::Cutdown::begin()
{
    pinMode(ARMED_LED_PIN, OUTPUT);

    Serial.begin(9600);

    status = InitXBee(XBEE_ADDR, XBEE_PAN_ID, Serial);

    if (!status)
    {
        one_byte_message(INIT_RESPONSE);

        armed = false;
        pkt_type = 0;
        bytes_read = 0;
        fcn_code = 0;
        tlm_pos = 0;
        cycles_armed = -1;

        return 0;
    }
    else
    {
        // you're fucked
        one_byte_message(BAD_COMMAND_RESPONSE);
        one_byte_message(status);
        one_byte_message(status >> 8);
        return 1;
    }
}

void Cutdown::check_input()
{
    // look for any new messages
    read_input();

    // if system is armed, increment the timer indicating for how long
    if (cycles_armed > 0)
    {
        cycles_armed++;
    }

    // if the system has been armed for more than the timeout, disarm
    if (cycles_armed > ARM_MILLISECOND_TIMEOUT / CYCLE_MILLISECOND_INTERVAL)
    {
        disarm_system();
    }

    // wait
    delay(CYCLE_MILLISECOND_INTERVAL);
}

void Cutdown::arm_system()
{
    armed = true;
    digitalWrite(ARMED_LED_PIN, HIGH);
    one_byte_message(ARMED_RESPONSE);
    cycles_armed = 1;
}

void Cutdown::disarm_system()
{
    armed = false;
    digitalWrite(ARMED_LED_PIN, LOW);
    one_byte_message(DISARMED_RESPONSE);
    cycles_armed = -1;
}

void Cutdown::read_input()
{
    if ((pkt_type = readMsg(1)) == 0)
    {
        // Read something else, try again
    }
    // if we didn't have a read error, process it
    if (pkt_type > -1)
    {
        if (pkt_type)
        {
            bytes_read = readCmdMsg(incoming_bytes, fcn_code);
            command_response(fcn_code, incoming_bytes, bytes_read);
        }
        else
        {    // unknown packet type?
            one_byte_message (READ_FAIL_RESPONSE);
        }
    }
}

void Cutdown::command_response(uint8_t _fcncode, uint8_t data[], uint8_t length)
{
    // process a command to arm the system
    if (_fcncode == ARM_FCNCODE)
    {
        arm_system();
    }
    // process a command to disarm the system
    else if (_fcncode == DISARM_FCNCODE)
    {
        disarm_system();
        release = false;
    }
    // process a command to release
    else if (_fcncode == FIRE_FCNCODE)
    {
        release = true;
    }
    // process a command to report the arm status
    else if (_fcncode == ARM_STATUS_FCNCODE)
    {
        if (armed)
        {
            one_byte_message(ARMED_RESPONSE);
        }
        else
        {
            one_byte_message(DISARMED_RESPONSE);
        }
    }
    else
    {
        one_byte_message(BAD_COMMAND_RESPONSE);
    }
}

void Cutdown::one_byte_message(uint8_t msg)
{
    tlm_pos = 0;
    tlm_pos = addIntToTlm<uint8_t>((uint8_t) msg, tlm_data, tlm_pos);
    sendTlmMsg(TLM_ADDR, tlm_data, tlm_pos);
}
