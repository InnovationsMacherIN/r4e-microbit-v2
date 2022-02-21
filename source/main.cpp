#include "MicroBit.h"
#include "MusicalNotes.h"
#include "Pictures.h"
#include "CodalDmesg.h"

MicroBit uBit;
MicroBitUARTService *uart;
//  MOTOR CONTROL VARIABLES
uint32_t velocity_1 = 1023;  // initial velocity motor1 (motor velocity from (1*64 - 1) = 63 to (16*64 - 1) = 1023
uint32_t velocity_2 = 1023;  // initial velocity motor2
// Pins for waveshare motor board
MicroBitPin P8(MICROBIT_ID_IO_P8, MICROBIT_PIN_P8, PIN_CAPABILITY_ANALOG);
MicroBitPin P16(MICROBIT_ID_IO_P16, MICROBIT_PIN_P16, PIN_CAPABILITY_ANALOG);
// Pins for keyestudio- or elecfreak motor board
//MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ANALOG);
//MicroBitPin P2(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_ANALOG);

const ManagedString forwards("v");  //name forwards to delineate from std::forward
const ManagedString backwards("z");
const ManagedString curveRight("r");
const ManagedString curveLeft("l");
const ManagedString turnRight("R"); //turns right around own middle axis
const ManagedString turnLeft("L");  //turns left around own middle axis
const ManagedString stop("s");

// FUNCTION DECLARATIONS
unsigned char set_direction(ManagedString direction, unsigned char *bitmasks);
void setMotorPins(ManagedString msg);
void changeMotorVelocity(ManagedString msg);
void playMelody(ManagedString msg);
void wait(ManagedString msg);
void showPictureOrText(ManagedString msg);
void moveBot(ManagedString msg);
void showSensorValue(ManagedString msg);
void playSound(ManagedString msg);
// void turnDisplay(ManagedString msg); // TODO: implement for microbit v2

// CALLBACK FUNCTIONS
void onConnected(MicroBitEvent)
{
    uBit.display.scroll("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.scroll("D");
}

void onButtonB(MicroBitEvent)
{
    //<! stops motors and clears display immediately
    setMotorPins("s");
    uBit.display.clear();
    //uart->send("STOP\n");
}

int main() {
    uBit.init();
    uBit.display.scrollAsync("R4G");
    uBit.audio.setVolume(255);
    // uBit.audio.soundExpressions.playAsync("giggle");
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonB);

    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    //uBit.thermometer.setPeriod(2000);
    //new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    
    while(1) {
        ManagedString msg = uart->readUntil(":");
        // uBit.display.scroll(msg);
        switch(msg.charAt(0)) {
            case 'A': {
                showPictureOrText(msg);
                break;}
            case 'B': {
                moveBot(msg);
                break;}
            case 'G': {
                changeMotorVelocity(msg);
                break;}
            case 'K': {
                playSound(msg);
                break;}
            case 'M': {
                playMelody(msg);
                break;}
            case 'W': {
                wait(msg);
                break;}
            default: {
                uBit.display.scroll(msg);
                break;}
        }
        uart->send("OK\n");
    }
}

// FUNCTIONS
/* Microbit v2 does not offer rotateTo method for display
void turnDisplay(ManagedString msg) {
    int degrees = msg.charAt(1);
    if(degrees == '0') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);
    }
    else if(degrees == '9' && msg.charAt(2) == '0') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_90);
    }
    else if(degrees == '1' && msg.charAt(2) == '8') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_180);
    }
    else if(degrees == '2' && msg.charAt(2) == '7') {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_270);
    }
    else {
        uBit.display.scroll(msg);
    }
}
*/

void showPictureOrText(ManagedString msg) {
    //>! Show picture or scroll text according to msg from app
    int idx = msg.charAt(1) - '0';
    if(idx > storedPictures){
        uBit.display.scroll(msg);
        return;
    } else if(idx == 0) {
        uBit.display.clear();
        return;
    }
    int time_to_shine = (msg.charAt(2) - '0') * 1000;
    if(time_to_shine < 0 || time_to_shine > 9000){
        uBit.display.scroll(msg);
        return;
    } else if(time_to_shine == 0) {
        uBit.display.printAsync(PICTURES[idx-1]);
        uart->send("OK\n");
    } else {
        uBit.display.print(PICTURES[idx-1],0,0,0,time_to_shine);
        uBit.display.clear();
    }
    // MicroBitImage i((ImageData*)PICTURES[idx-1]);
}

void wait(ManagedString msg) {
    int wait_time = (int)((msg.charAt(1)-'0') * 1000);
    wait_time = wait_time + (int)((msg.charAt(3)-'0') * 100);
    if (wait_time >= 100 && wait_time <= 9900) {
        uBit.sleep(wait_time);
    }
    else {
        uBit.display.scroll(msg);
    }
}

void playMelody(ManagedString msg) {
    //>! play Song from songbook in musicalNotes.h
    if((msg.charAt(1)-'0') < 1 || (msg.charAt(1)-'0') > storedSongs){
        uBit.display.scroll(msg);
        return;
    }
    int songidx = (int)(msg.charAt(1)-'0') - 1;
    for(int i = 0; SONGS[songidx][i] != -1; i++){
        uBit.audio.virtualOutputPin.setAnalogValue(511);  // set duty cycle
        uBit.audio.virtualOutputPin.setAnalogPeriodUs((int)(1000000/SONGS[songidx][i]));
        uBit.sleep(BEATS[songidx][i]);
        uBit.audio.virtualOutputPin.setAnalogValue(0);
        uBit.sleep(50);
    }
}

void playSound(ManagedString msg) {
    int soundIndex = (int)((msg.charAt(1)-'0') * 10 + (msg.charAt(2)-'0'));
    if(soundIndex < 1 || soundIndex > 10){
        uBit.display.scroll(msg);
        return;
    }
    uBit.audio.soundExpressions.playAsync(sounds[soundIndex-1]);
}

void moveBot(ManagedString msg) {
    uint32_t duration = 0;
    if((msg.charAt(2)-'0') < 0 || (msg.charAt(2)-'0') > 10){
        uBit.display.scroll(msg);
        return;
    }
    duration = (uint32_t)((msg.charAt(2)-'0') * 1000);
    duration = duration + (uint32_t)((msg.charAt(4)-'0') * 100);
    setMotorPins(msg);
    uBit.sleep(duration);
    setMotorPins("s");
}

void changeMotorVelocity(ManagedString msg) {
    char motor = msg.charAt(1);
    uint32_t val = (uint32_t)((msg.charAt(2)-'0') * 10 + (msg.charAt(3)-'0'));
    if(val < 1 || val > 31){
        uBit.display.scroll(msg);
        return;
    }
    int vel = int(val * 32 + 31);
    switch(motor){
        case '1': {
            velocity_1 = vel;
            break;}
        case '2': {
            velocity_2 = vel;
            break;}
        case 'b': {
            velocity_1 = vel;
            velocity_2 = vel;
            break;}
        default: {
            uBit.display.scroll(msg);
            break;}
    }
}

unsigned char set_direction(ManagedString direction, unsigned char *bitmasks){
    /**
     * Set a bitmask for given direction
     * Returns bitmask for direction
     */
    unsigned char moveMask = 0;
    if(direction == forwards)
        moveMask = bitmasks[0];
    else if(direction == backwards)
        moveMask = bitmasks[1];
    else if(direction == curveLeft)
        moveMask = bitmasks[2];
    else if(direction == curveRight)
        moveMask = bitmasks[3];
    else if(direction == turnLeft)
        moveMask = bitmasks[4];
    else if(direction == turnRight)
        moveMask = bitmasks[5];
    else
        moveMask = 0;   //stop
    return moveMask;
}

// Choose your board or write your own motor control
void setMotorPins(ManagedString msg) {
    /**
     * Set pins of motor board, choose your board on the bottom of the file
     * Waveshare Motor Driver Board for micro:bit
     * Motor A in1 = pin 13,    Motor A in2 = pin 12
     * Motor B in1 = pin 14,    Motor B in2 = pin 15
     * Motor A PWM = pin 8,     Motor B PWM = pin 16
     * bitmask for motor control: 0 0 P16 P15 P14 P8 P12 P13
     */
    unsigned char m1_pin1 = 1, m1_pin2 = 2, m1_pwm = 4, m2_pin1 = 8, m2_pin2 = 16, m2_pwm = 32; // bitmasks for motorPins
    unsigned char bitmasks[6] = {45,54,5,40,53,46}; // bitmasks for directions
    ManagedString direction(msg.charAt(1));
    unsigned char moveMask = 0;
    moveMask = set_direction(direction,bitmasks);
    //set the pin values according to direction
    uBit.io.P13.setDigitalValue(m1_pin1 & moveMask);                // dir motor1
    uBit.io.P12.setDigitalValue((m1_pin2 & moveMask)/m1_pin2);
    uBit.io.P14.setDigitalValue((m2_pin1 & moveMask)/m2_pin1);      // dir motor2
    uBit.io.P15.setDigitalValue((m2_pin2 & moveMask)/m2_pin2);
    P8.setAnalogValue(((m1_pwm & moveMask)/m1_pwm) * velocity_1);   // pwm motor1
    P16.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2);  // pwm motor2
}

// void setMotorPins(ManagedString msg) {
//    /**
//     * ElecFreaks Motor:bit Board
//     * Motor 1 PWM = P1,    Motor1 direction = P8 (LOW = CC, HIGH = C)
//     * Motor 2 PWM = P2,    Motor2 direction = P12(LOW = CC, HIGH = C)
//     * bitmask motor control: X X X X P12 P2 P8 P1
//     * bitmasks for motor pins:
//     */
//    unsigned char m1_pwm = 1, m1_dir = 2, m2_pwm = 4, m2_dir = 8;
//    unsigned char bitmasks[6] = {15,5,12,3,13,7};   // bitmasks for directions
//    ManagedString direction(msg.charAt(1));
//    unsigned char moveMask = set_direction(direction,bitmasks);
//    // Set the pin values according to direction
//    uBit.io.P12.setDigitalValue((m2_dir & moveMask)/m2_dir);              //dir motor2
//    uBit.io.P8.setDigitalValue((m1_dir & moveMask)/m1_dir);               //dir motor1
//    uBit.io.P1.setAnalogValue((m1_pwm & moveMask)* velocity_1);           //pwm motor1
//    uBit.io.P2.setAnalogValue(((m2_pwm & moveMask)/m2_pwm) * velocity_2); //pwm motor2
// }

//void setMotorPins(ManagedString msg) {
//    /**
//     * Keyestudio Motor Driver Board v1.8
//     * Enable = pin14
//     * Motor1 CW = pin12,   Motor1 CCW = pin13
//     * Motor2 CW = pin15,   Motor2 CCW = pin16
//     * Motor1 PWM = pin1,   Motor2 PWM = pin2
//     * Bitmask motor control: X X X P15 P16 P12 P13 P14
//     */
//    unsigned char bitmasks[6] = {11,21,9,3,13,19};  // bitmasks for directions
//    ManagedString direction(msg.charAt(1));
//    unsigned char moveMask = set_direction(direction,bitmasks);
//    // set motor pins according to direction
//    uBit.io.P1.setAnalogValue(velocity_1);
//    uBit.io.P2.setAnalogValue(velocity_2);
//    uBit.io.P12.setDigitalValue((4 & moveMask)/4);
//    uBit.io.P13.setDigitalValue((2 & moveMask)/2);
//    uBit.io.P15.setDigitalValue((16 & moveMask)/16);
//    uBit.io.P16.setDigitalValue((8 & moveMask)/8);
//    uBit.io.P14.setDigitalValue(1 & moveMask);
//}