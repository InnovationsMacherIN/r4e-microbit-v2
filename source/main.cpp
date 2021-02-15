#include "MicroBit.h"
#include "Tests.h"
#include "MusicalNotes.h"
#include "Pictures.h"

MicroBit uBit;
MicroBitUARTService *uart;

void onConnected(MicroBitEvent)
{
    ManagedString con = "C";
    uBit.display.scroll(con);
    
    ManagedString msg = "R4g";
    ManagedString eom = ":";
    /*
    while(1) {
        msg = uart->readUntil(eom);
        uBit.display.scroll(msg);
        uart->send("OKS\n");   
    }
    */
}

void onDisconnected(MicroBitEvent)
{
    ManagedString discon = "D";
    uBit.display.scroll(discon);
}

void showPictureOrText(ManagedString msg) {
    //>! Show picture or scroll text according to msg from app
    int idx = msg.charAt(1) - '0';
    if(idx > storedPictures){
        uBit.display.scroll(msg);
        return;
    }
    int time_to_shine = (msg.charAt(2) - '0') * 1000;
    if(time_to_shine < 0 || time_to_shine > 9000){
        uBit.display.scroll(msg);
        return;
    }
    // MicroBitImage i((ImageData*)PICTURES[idx-1]);
    uBit.display.print(PICTURES[idx-1],0,0,0,time_to_shine);
    uBit.display.clear();
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


int main()
{
    uBit.init();
    uBit.display.scrollAsync("R4G");
    uBit.audio.setVolume(255);
    uBit.audio.soundExpressions.playAsync("giggle");
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    while(1) {
        ManagedString msg = uart->readUntil(":");
        // uBit.display.scroll(msg);
        switch(msg.charAt(0)) {
            case 'A': {
                showPictureOrText(msg);
                break;}
            case 'W': {
                wait(msg);
                break;}
            case 'M': {
                playMelody(msg);
                break;
            }
            default:
                uBit.display.scroll(msg);
        }
        uart->send("OK\n");
    }
    
    /*
    {
        DMESG("TEMPERATURE: %d", uBit.thermometer.getTemperature());
        uBit.display.scroll("Temp= ");
        uBit.display.scroll(uBit.thermometer.getTemperature());
        uBit.sleep(1000);
    }
    */  
}