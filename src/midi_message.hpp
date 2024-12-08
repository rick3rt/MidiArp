#pragma once

#include <cstdint>
#include <iostream>

#include <libremidi/libremidi.hpp>

struct midi_message
{
    bool status;
    uint8_t channel;
    uint8_t note;
    uint8_t velocity;

    midi_message() : status(false), channel(0), note(0), velocity(0) {}
    midi_message(const libremidi::message &message)
    {
        status = message.get_message_type() == libremidi::message_type::NOTE_ON;
        channel = message.get_channel();
        note = message[1];
        velocity = status ? message[2] : 0;
    }
};

std::ostream &operator<<(std::ostream &s, const midi_message &message)
{
    s << "status: " << (int)message.status << ", channel: " << (int)message.channel << ", note: " << (int)message.note << ", velocity: " << (int)message.velocity;
    return s;
}
