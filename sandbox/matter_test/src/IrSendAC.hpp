#ifndef IRSENDAC_HPP_
#define IRSENDAC_HPP_

#include <IRremoteESP8266.h>
#include <ir_Panasonic.h>

class IrSendAc {
private:
    IRPanasonicAc pana = IRPanasonicAc(0);
public:
    IrSendAc(uint16_t IRsendPin);
    ~IrSendAc();
    void begin();
    void setTemp(uint8_t temp);
};

IrSendAc::IrSendAc(uint16_t IRsendPin) {
    pana = IRPanasonicAc(IRsendPin); //set the IRsend object.
}

IrSendAc::~IrSendAc() {
}

void IrSendAc::begin() {
    pana.begin();
}





#endif // IRSENDAC_HPP_