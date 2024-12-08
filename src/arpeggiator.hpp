#pragma once

#include <cstdint>
#include <thread>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include <conio.h> // for _kbhit() and _getch()
#include <libremidi/libremidi.hpp>
#include "number_generator.hpp"

struct ArpStep
{
    ArpStep() : channel(0), note(0), velocity(0) {}
    ArpStep(uint8_t channel, uint8_t note, uint8_t velocity) : channel(channel), note(note), velocity(velocity) {}
    uint8_t channel;
    uint8_t note;
    uint8_t velocity;
};

std::ostream &operator<<(std::ostream &s, const ArpStep &step)
{
    s << "ArpStep channel: " << (int)step.channel << ", note: " << (int)step.note << ", velocity: " << (int)step.velocity;
    return s;
}

class Arpeggiator
{
public:
    Arpeggiator(const libremidi::output_port &port, const std::vector<ArpStep> &arpsteps, float bpm, float dutycycle);
    ~Arpeggiator();

    // initialize the arpeggiator midi messages
    void init_messages();

    // run the arpeggiator
    void run();

    void regenerate(int length);

    void transpose(int semitones);

private:
    float bpm_;
    float dutycycle_;
    std::vector<ArpStep> arpsteps_;                           // arpeggiator steps
    std::vector<std::array<libremidi::message, 2>> messages_; // note on and note off messages
    libremidi::output_port port_;                             // libremidi output port
    libremidi::midi_out midi_out_;                            // libremidi midi out object
};

Arpeggiator::Arpeggiator(const libremidi::output_port &port, const std::vector<ArpStep> &arpsteps, float bpm, float dutycycle)
    : port_(port), arpsteps_(arpsteps), bpm_(bpm), dutycycle_(dutycycle)
{
    // initialize the midi out object
    if (auto err = midi_out_.open_port(port_); err != stdx::error{})
        err.throw_exception();

    init_messages();
}

Arpeggiator::~Arpeggiator()
{
    std::cout << "Arpeggiator destructor" << std::endl;
    // make sure all notes are off
    for (const auto &msg : messages_)
    {
        const auto note_off = msg[1];
        if (auto err = midi_out_.send_message(note_off); err != stdx::error{})
            err.throw_exception();
    }
}

void Arpeggiator::init_messages()
{
    // initialize the messages
    messages_.clear();
    for (const auto &arpstep : arpsteps_)
    {
        messages_.push_back({
            libremidi::channel_events::note_on(arpstep.channel, arpstep.note, arpstep.velocity),
            libremidi::channel_events::note_off(arpstep.channel, arpstep.note, arpstep.velocity),
        });
    }
}

void Arpeggiator::regenerate(int length)
{
    length = (length > 0) ? length : random_number(2, 10);
    arpsteps_.clear();
    for (int i = 0; i < length; i++)
    {
        arpsteps_.push_back(ArpStep(1, random_number(30, 70), random_number(40, 127)));
    }

    init_messages();
}

void Arpeggiator::transpose(int semitones)
{
    for (auto &arpstep : arpsteps_)
    {
        arpstep.note += semitones;
    }

    init_messages();
}

void Arpeggiator::run()
{
    const float duration_ms = 60000.0f / bpm_;
    const long long duration_on_ms = static_cast<long long>(duration_ms * dutycycle_);
    const long long duration_off_ms = static_cast<long long>(duration_ms * (1.0f - dutycycle_));

    // run the arpeggiator
    while (true)
    {
        for (const auto &msg : messages_)
        {
            const auto note_on = msg[0];
            const auto note_off = msg[1];

            // std::cout << "Sending Step" << note_on << std::endl;

            // send message ON
            if (auto err = midi_out_.send_message(note_on); err != stdx::error{})
                err.throw_exception();
            std::this_thread::sleep_for(std::chrono::milliseconds(duration_on_ms));

            // and OFF
            if (auto err = midi_out_.send_message(note_off); err != stdx::error{})
                err.throw_exception();
            std::this_thread::sleep_for(std::chrono::milliseconds(duration_off_ms));
        }

        // Check if a key has been pressed
        if (_kbhit())
        {
            const char c = _getch(); // Consume the key press (don't need to store it)
            switch (c)
            {
            case 'q':
                return; // break the loop
            case 'r':
                regenerate(0);
                break;
            case ',':
                transpose(-5);
                break;
            case '.':
                transpose(5);
                break;

            case '1':
                regenerate(1);
                break;
            case '2':
                regenerate(2);
                break;
            case '3':
                regenerate(3);
                break;
            case '4':
                regenerate(4);
                break;
            case '5':
                regenerate(5);
                break;
            case '6':
                regenerate(6);
                break;
            case '7':
                regenerate(7);
                break;
            case '8':
                regenerate(8);
                break;
            case '9':
                regenerate(9);
                break;
            case '0':
                regenerate(10);
                break;

            default:
                break;
            }
        }
    }
}