#ifndef IRSENDAC_HPP_
#define IRSENDAC_HPP_

#include <IRremoteESP8266.h>
#include <ir_Panasonic.h>

enum class AC_Mode : uint8_t {
    Auto = kPanasonicAcAuto,
    Cool = kPanasonicAcCool,
    Dry  = kPanasonicAcDry,
    // Fan = 3,
    Heat = kPanasonicAcHeat,
};

class IrSendAc {
private:
    IRPanasonicAc pana = IRPanasonicAc(0);
    bool power = false;
    AC_Mode mode = AC_Mode::Auto;
    uint8_t temp_auto = 25;
    uint8_t temp_cooling = 28;
    uint8_t temp_heating = 17;
    uint8_t temp_dry = 25;
public:
    IrSendAc(uint16_t IRsendPin);
    ~IrSendAc();
    void begin();
    void setTemp(uint8_t temp);
    void setMode(AC_Mode mode);
    void setPower(bool power);
    uint8_t getTemp();
};

IrSendAc::IrSendAc(uint16_t IRsendPin) {
    pana = IRPanasonicAc(IRsendPin); //set the IRsend object.
}

IrSendAc::~IrSendAc() {
}

void IrSendAc::begin() {
    pana.begin();
}

void IrSendAc::setTemp(uint8_t temp) {
    if (temp < 16) {
        temp = 16;
    } else if (temp > 30) {
        temp = 30;
    }
    switch (mode) {
        case AC_Mode::Auto:
            temp_auto = temp;
            break;
        case AC_Mode::Cool:
            temp_cooling = temp;
            break;
        case AC_Mode::Heat:
            temp_heating = temp;
            break;
        case AC_Mode::Dry:
            temp_dry = temp;
            break;
    }
    if (power) {
        pana.setTemp(temp);
        pana.send();
    }
}

void IrSendAc::setMode(AC_Mode mode) {
    this->mode = mode;
    if (power) {
        pana.setMode((uint8_t)mode);
        pana.send();
    }
}

void IrSendAc::setPower(bool power) {
    this->power = power;
    if (power) {
        pana.setPower(true);
        pana.setMode((uint8_t)mode);
        pana.setTemp(getTemp());
        pana.send();
    } else {
        pana.setPower(false);
        pana.send();
    }
}

uint8_t IrSendAc::getTemp() {
    switch (mode) {
        case AC_Mode::Auto:
            return temp_auto;
        case AC_Mode::Cool:
            return temp_cooling;
        case AC_Mode::Heat:
            return temp_heating;
        case AC_Mode::Dry:
            return temp_dry;
        default:
            return 25;
    }
}

#endif // IRSENDAC_HPP_