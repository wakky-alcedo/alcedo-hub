#ifndef MATTER_ON_UPDATE_HPP_
#define MATTER_ON_UPDATE_HPP_

#include "IrSendLight.hpp"
#include "IrSendAC.hpp"
constexpr uint8_t IR_SEND_PIN = 32;
constexpr uint8_t IR_RECV_PIN = 33;

IrSendLight ir_send_light(IR_SEND_PIN);

namespace ac {
// enum class Attributes : uint8_t {
//     SystemMode = 0x0000001C,
//     OccupiedCoolingSetpoint = 0x00000011,
//     OccupiedHeatingSetpoint = 0x00000012,
// };
enum class SystemMode : uint8_t {
    Off = 0x00,
    Auto = 0x01,
    Cool = 0x03,
    Heat = 0x04,
};
}

namespace light {
// enum class Attributes : uint8_t {
//     OnOff = 0x00,
//     CurrentLevel = 0x0008,
//     ColorTemperatureMireds = 0x0007,
// };
enum class OnOff : uint8_t {
    Off = 0x00,
    On = 0x01,
};
}

void on_ac_system_mode_update(ac::SystemMode system_mode) {
    
    Serial.printf("SystemMode: %d\n", system_mode);
}


void on_light_onoff_update(light::OnOff state) {
    switch (state) {
    case light::OnOff::Off:
        ir_send_light.send(LightCmmand::Off);
        Serial.printf("Light Off\n");
        break;
    case light::OnOff::On:
        ir_send_light.send(LightCmmand::On);
        Serial.printf("Light On\n");
        break;
    }
    Serial.printf("Light: %d\n", state);
}



#endif // MATTER_ON_UPDATE_HPP_