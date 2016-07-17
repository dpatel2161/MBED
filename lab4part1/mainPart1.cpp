#include "mbed.h"
#include "TMP36.h"
#include "PinDetect.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"
#include <iostream>
#include <string>
#include <cmath>

TMP36 myTMP36(p15);

SDFileSystem sd(p5, p6, p7, p8, "sd");

PinDetect pb1(p18); //pushbuttons
PinDetect pb2(p19);
//PinDetect pb3(p20);

uLCD_4DGL uLCD(p28, p27, p29);
// setup instance of new Speaker class, mySpeaker using pin 21
// the pin must be a PWM output pin
//Speaker mySpeaker(p21);

enum InputType {YES, NO, NOT_PRESSED};
enum StateType {WAIT, COPY, END};

InputType input = NOT_PRESSED;
StateType state = COPY;
//---------------------------------------------------------------------------------------------------
// Callback routine is interrupt activated by a debounced pb1 hit
// That is ... this code runs with interrupt is generated by first button press
void pb1_hit_callback (void)
{
    
    input = YES;
}

// ---------------------------------------------------------------------------------------------------
// Callback routine is interrupt activated by a debounced pb2 hit
// That is ... this code runs with interrupt is generated by second button press
void pb2_hit_callback (void)
{
    
    input = NO;
}

//reads Sender position file
void generateSenderPos()
{
    mkdir("/sd/mydir", 0777);
    FILE *fp2 = fopen("/sd/mydir/positionCipherSender.txt", "w+");
    if(fp2 == NULL) {
        uLCD.printf("Error Open\n");
    } else {
        fprintf(fp2, "0") ; //write line
    }
    fclose(fp2);
}
//reads Receiver position file
void generateReceiverPos()
{
    mkdir("/sd/mydir", 0777);
    FILE *fp3 = fopen("/sd/mydir/positionCipherReceiver.txt", "w+");
    if(fp3 == NULL) {
        uLCD.printf("Error Open\n");
    } else {
        fprintf(fp3, "0") ; //write line
    }
    fclose(fp3);
}
//creates 1000 character key file
void generateOTP()
{
    mkdir("/sd/mydir", 0777);
    FILE *fp = fopen("/sd/mydir/OTP.txt", "w+");
    if(fp == NULL) {
        uLCD.printf("Error Open\n");
    } else {
        //uLCD.printf("Opened\n");
        std::srand(std::time(NULL));
        string result = "";
        for (int i=0; i<1000; i++) {
            float temp1 = myTMP36.read();
            wait(.005);
            float temp2 = myTMP36.read();
            int temp = int(1000* (temp1-temp2));
            int number = abs(((std::rand() % 26) + temp) % 26);
            char letter = char('A' + number);
            fprintf(fp, "%c", letter) ; //write line

        }
    }
    fclose(fp);
}
// ---------------------------------------------------------------------------------------------------
int main()
{
    //setup push buttons
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    //pb3.mode(PullUp);

    // Delay for initial pullup to take effect
    wait(.01);

    // Setup Interrupt callback functions for a pb hit
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);

    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    // pushbuttons now setup and running


    //while loop intitial condition
    bool getout = false;




    generateReceiverPos(); //creating receiver position file
    generateSenderPos(); //creating sender position file
    while(!getout) {
        wait(0.1);

        switch(state) {
            case (COPY):
                //perform copy operation
                //generate one type pad
                generateOTP();

                //open files..copy one time pad to sd card
                uLCD.printf("I have created the pad! Do you want another? \n");

                state = WAIT;
                //calculate next state
                break;
            case (WAIT):
                //calculate the next state
                if (input == YES) {
                    state = COPY;
                    input = NOT_PRESSED;
                } else if (input == NO) {
                    state = END;
                    input = NOT_PRESSED;
                }
                break;
            case (END):
                getout = true; //condtion to exit while loop
                uLCD.printf("Goodbye \n");

                break;
        } //end of switch statement

    } //end while loop
} //end main