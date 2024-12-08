#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <print>
#include <bitset>

#include <libremidi/libremidi.hpp>

#include "libremidi_utils.hpp"
#include "midi_message.hpp"
#include "number_generator.hpp"
#include "arpeggiator.hpp"

int main()
{
    std::cout << "This is MidiArp" << std::endl;

    libremidi::observer observer;
    if (observer.get_input_ports().empty())
        return 1;
    if (observer.get_output_ports().empty())
        return 1;

    // find the EP-133 in the ports, under port_name
    auto port_name = "EP-133";
    const auto &input_ports = observer.get_input_ports();
    const auto input_port = *std::find_if(input_ports.begin(), input_ports.end(),
                                          [&](const libremidi::input_port &port)
                                          {
                                              return port.port_name == port_name;
                                          });
    const auto &output_ports = observer.get_output_ports();
    const auto output_port = *std::find_if(output_ports.begin(), output_ports.end(),
                                           [&](const libremidi::output_port &port)
                                           {
                                               return port.port_name == port_name;
                                           });

    std::cout << "Selected Input port:  " << input_port << std::endl;
    std::cout << "Selected Output port: " << input_port << std::endl;

    std::vector<ArpStep> steps;
    for (int i = 0; i < random_number(2, 10); i++)
    {
        steps.push_back(ArpStep(1, random_number(30, 70), random_number(40, 127)));
    }
    Arpeggiator arp{output_port, steps,
                    2 * 160.0f,
                    1.0f};

    arp.run();

    //  ArpStep(1, random_number(50, 70), random_number(80, 127)),
    //  ArpStep(1, random_number(50, 70), random_number(80, 127)),
    //  ArpStep(1, random_number(50, 70), random_number(80, 127)),
    //  ArpStep(1, random_number(50, 70), random_number(80, 127)),
    //  ArpStep(1, random_number(50, 70), random_number(80, 127)),

    // libremidi::midi_in midi_in{{.on_message = [](const libremidi::message &message)
    //                             {
    //                                 // std::cout << std::bitset<8>(message[0]) << " || ";
    //                                 // std::cout << message << " -- ";
    //                                 // std::cout << message.is_note_on_or_off() << " -- ";
    //                                 std::cout << midi_message{message} << std::endl;
    //                             }}};
    // libremidi::midi_out midi_out;

    // try
    // {
    //     if (auto err = midi_in.open_port(input_port); err != stdx::error{})
    //         err.throw_exception();
    //     if (auto err = midi_out.open_port(observer.get_output_ports()[0]); err != stdx::error{})
    //         err.throw_exception();
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Could not open MIDI device :(\n";
    //     std::cerr << e.what() << std::endl;
    //     return 1;
    // }

    // if (auto err = midi_in.open_port(input_port); err != stdx::error{})
    //     err.throw_exception();
    // if (auto err = midi_out.open_port(output_port); err != stdx::error{})
    //     err.throw_exception();

    // // status: 1, channel: 8, note: 60, velocity: 119
    // // status: 1, channel: 5, note: 60, velocity: 127
    // const auto n1_on = libremidi::channel_events::note_on(5, 60, 127);
    // const auto n2_on = libremidi::channel_events::note_on(8, 60, 119);
    // const auto n1_off = libremidi::channel_events::note_off(5, 60, 127);
    // const auto n2_off = libremidi::channel_events::note_off(8, 60, 119);

    // while (true)
    // {
    //     std::cout << "sending note: " << n1_on << " -- " << midi_message{n1_on} << std::endl;
    //     if (auto err = midi_out.send_message(n1_on); err != stdx::error{})
    //         err.throw_exception();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     if (auto err = midi_out.send_message(n1_off); err != stdx::error{})
    //         err.throw_exception();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }

    // std::cout << "This was MidiArp" << std::endl;

    return 0;
}