#include <PS3BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;

BTD Btd(&Usb); // Create the Bluetooth Dongle instance

PS3BT PS3(&Btd); // This will also store the bluetooth address

bool printTemperature, printAngle;

#define PSXP 41         // Powerscrew in the X axis P
#define PSXN 42         // Powerscrew in the X Axis N
#define PSYP 43         // Powerscrew in the Y Axis P
#define PSYN 44         // Powerscrew in the Y Axis N
#define RF 22           // GO FORWARD
#define RB 24           // GO BACKWORD
#define RR 21           // GO RIGHT
#define RL 23           // GO LEFT
#define RTR 26          // TURN RIGH
#define RTL 2           // TURN LEFT
#define LP 28           // launcher Piston
int RPWM_Output = 8;    // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 9;     // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
int val;


void setup() {
    Serial.begin(115200);
    #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
    #endif
    if (Usb.Init() == -1) {
        Serial.print(F("\r\nOSC did not start"));
        while (1); //halt
    }
    Serial.print(F("\r\nPS3 Bluetooth Library Started"));
    
    pinMode(RF, OUTPUT);    //GO FORWARD
    pinMode(RB, OUTPUT);    //GO BACKWORD
    pinMode(RR, OUTPUT);    //GO RIGHT
    pinMode(RL, OUTPUT);    //GO LEFT
    pinMode(RTR, OUTPUT);   // TURN RIGH
    pinMode(RTL, OUTPUT);   // TURN LEFT
    
    pinMode(RPWM_Output, OUTPUT);
    pinMode(LPWM_Output, OUTPUT);
    pinMode(PSXN, OUTPUT);
    pinMode(PSXP, OUTPUT);
    pinMode(PSYN, OUTPUT);
    pinMode(PSYP, OUTPUT);
    
    pinMode(LP, OUTPUT);
    
    digitalWrite(RF ,HIGH);
    digitalWrite(RB ,HIGH);
    digitalWrite(RR ,HIGH);
    digitalWrite(RL ,HIGH);
    digitalWrite(RTR ,HIGH);
    digitalWrite(RTL ,HIGH);
    digitalWrite(PSYP ,HIGH);
    digitalWrite(PSYN ,HIGH);
    digitalWrite(PSXP ,HIGH);
    digitalWrite(PSXN ,HIGH);
    digitalWrite(LP ,HIGH);
}


void loop() {
    Usb.Task();
    
    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        
        if (PS3.getButtonPress(UP)) {
            Serial.print(F("\r\nUP"));
            
            up();
            
        }
            
        else if (PS3.getButtonPress(RIGHT)) {
            down();
            Serial.print(F("\r\nDown"));
        }
            
        else if (PS3.getButtonPress(DOWN)) {
            left();
            Serial.print(F("\r\nLeft"));
        }
            
            
        else if (PS3.getButtonPress(LEFT)) {
            right();
            Serial.print(F("\r\nRight"));
        }
            
        else if (PS3.getButtonClick(TRIANGLE))
        {
            if(val>=255){
                val=255;
            }
            val+=5 ;
            analogWrite(LPWM_Output, 0);
            analogWrite(RPWM_Output,val);
            Serial.println("++++++++");
            Serial.println(val);
            
        }
            
        else if (PS3.getButtonClick(CIRCLE))
        {
            if(val<=10) {
                val=10;
            }
            val-=5;
            analogWrite(LPWM_Output, 0);
            analogWrite(RPWM_Output,val);
            Serial.println("----------");
            Serial.println(val);
            
        }
            
        else if(PS3.getButtonPress(CROSS))
        {
            digitalWrite(LP ,LOW);
            delay(10);
            digitalWrite(LP ,HIGH);
        }
            
        else if(PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
            if(PS3.getAnalogButton(L2)>25) {
                Serial.println("Power screw X Down") ;
                digitalWrite(PSXN , LOW ) ;
                digitalWrite(PSXP , HIGH) ;
            }
            
            if (!PS3.PS3NavigationConnected) {
                if(PS3.getAnalogButton(R2)>25) {
                    Serial.println("Power screw X Up") ;
                    digitalWrite(PSXP , LOW ) ;
                    digitalWrite(PSXN , HIGH) ;
                    
                }
            }
        }
            
        else if(PS3.getButtonPress(L1))
        {
            Serial.print(F("\r\nL1"));
            digitalWrite(PSYN , LOW ) ;
            digitalWrite(PSYP , HIGH) ;
        }
            
        else if(PS3.getButtonPress(R1))
        {
            Serial.print(F("\r\nR1"));
            digitalWrite(PSYP , LOW ) ;
            digitalWrite(PSYN , HIGH) ;
        }
            
        else if (PS3.getButtonPress(SELECT)) {
            Serial.print(F("\r\nSelect - "));
            TURN_LEFT();
        }
        else  if (PS3.getButtonPress(START)) {
            Serial.print(F("\r\nStart"));
            TURN_RIGHT();
        }
            
        else {
            STOP_ALL_OMNI() ;
            Serial.println("lol");
        }
    }
    
}


void STOP_ALL_OMNI()
    {
        digitalWrite(RF ,HIGH);
        digitalWrite(RB ,HIGH);
        digitalWrite(RR ,HIGH);
        digitalWrite(RL ,HIGH);
        digitalWrite(PSYP ,HIGH);
        digitalWrite(PSYN ,HIGH);
        digitalWrite(PSXP ,HIGH);
        digitalWrite(PSXN ,HIGH);
        digitalWrite(LP ,HIGH);
        digitalWrite(RTL,HIGH);
        digitalWrite(RTR,HIGH);
}

//FORWARDMovement
void up()
    {
        digitalWrite(RF,LOW);
}

//LEFTMovement
void left()
    {
        digitalWrite(RL,LOW);
}

//DOWNmovement
void down()
    {
        digitalWrite(RB,LOW);
}

//RIGHTmovement
void right()
    {
        digitalWrite(RR,LOW);
}

void TURN_RIGHT(){
    digitalWrite(RTR,LOW);
}

void TURN_LEFT(){
    digitalWrite(RTL,LOW);
}
