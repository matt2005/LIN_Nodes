// AUTOMATICALLY GENERATED - DO NOT EDIT
#ifdef __AVR__
# include <avr/pgmspace.h>
#else
# define PROGMEM
#endif

// Encodings
namespace Encoding
{

bool
validate_pwm_duty_cycle(uint16_t value) const
{
    switch (value) {
    case 0 ... 100:
        return true;
    }
    return false;
}

bool
validate_output_assignment(uint16_t value) const
{
    switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
        return true;
    }
    return false;
}

bool
validate_sec(uint16_t value) const
{
    switch (value) {
    case 0 ... 65535:
        return true;
    }
    return false;
}

bool
validate_PSI(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}

bool
validate_MPH(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}

bool
validate_F(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}

bool
validate_V(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}

bool
validate_up_to_10(uint16_t value) const
{
    switch (value) {
    case 0 ... 10:
        return true;
    }
    return false;
}

bool
validate_input(uint16_t value) const
{
    switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
        return true;
    }
    return false;
}

bool
validate_msec(uint16_t value) const
{
    switch (value) {
    case 0 ... 65535:
        return true;
    }
    return false;
}

bool
validate_lambda(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}

bool
validate_output_type(uint16_t value) const
{
    switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return true;
    }
    return false;
}

bool
validate_service_id(uint16_t value) const
{
    switch (value) {
    case 0x22:
    case 0x2e:
    case 0x3e:
    case 0xb2:
    case 0xb4:
    case 0x7f:
        return true;
    }
    return false;
}

bool
validate_RPM(uint16_t value) const
{
    switch (value) {
    case 0 ... 255:
        return true;
    }
    return false;
}
} // namepsace Encoding

// Node: Dashboard
namespace Dashboard
{
} // namespace Dashboard

// Node: PowerV1
namespace PowerV1
{
const PROGMEM encoding_table[kNumEncodings] = {
    Encoding::validate_output_assignment,
};
const PROGMEM Parameter::ParamInfo page4_info = {
    { 0, Encoding::validate_output_assignment },
    { 0, Encoding::validate_output_assignment },
    { 0, Encoding::validate_output_assignment },
    { 0, Encoding::validate_output_assignment },
};
} // namespace PowerV1

// Node: Master
namespace Master
{
const PROGMEM encoding_table[kNumEncodings] = {
    Encoding::validate_input,
    Encoding::validate_msec,
    Encoding::validate_sec,
    Encoding::validate_up_to_10,
};
const PROGMEM Parameter::ParamInfo page4_info = {
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 0, Encoding::validate_input },
    { 500, Encoding::validate_msec },
    { 0, Encoding::validate_up_to_10 },
    { 30, Encoding::validate_sec },
    { 30, Encoding::validate_sec },
    { 30, Encoding::validate_sec },
    { 0, Encoding::validate_msec },
    { 0, Encoding::validate_up_to_10 },
    { 0, Encoding::validate_msec },
};
} // namespace Master

// Node: Generic
namespace Generic
{
} // namespace Generic

// Node: ECUBridge
namespace ECUBridge
{
} // namespace ECUBridge

// Node: PowerV3
namespace PowerV3
{
const PROGMEM encoding_table[kNumEncodings] = {
    Encoding::validate_output_assignment,
    Encoding::validate_output_type,
    Encoding::validate_pwm_duty_cycle,
};
const PROGMEM Parameter::ParamInfo page4_info = {
    { 0, Encoding::validate_output_type },
    { 0, Encoding::validate_output_type },
    { 0, Encoding::validate_output_type },
    { 0, Encoding::validate_output_type },
    { 0, Encoding::validate_output_type },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
    { 255, Encoding::validate_output_assignment },
    { 0, Encoding::validate_pwm_duty_cycle },
};
} // namespace PowerV3

// Node: Tester
namespace Tester
{
} // namespace Tester
