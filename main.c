
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
volatile int temp=0;
volatile int x=0;
volatile int y=0xFF;
void datawrite(int data, int conditions);
volatile bool increment = true;

int v = 1;

int main(void)

{
    SYSCTL_RCGCI2C_R |= ( 1 << 1);

    SYSCTL_RCGC2_R |= (1 << 0);
    GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTA_CR_R = (1 << 6) | ( 1 << 7);
    GPIO_PORTA_DEN_R = (1 << 6) | ( 1 << 7);
    GPIO_PORTA_AFSEL_R = (1 << 6) | ( 1 << 7);
    GPIO_PORTA_PCTL_R = (3 << 24) | ( 3 << 28);
    GPIO_PORTA_PUR_R =  ( 1 << 6 ) |( 1 << 7);            //
    GPIO_PORTA_ODR_R = ( 1 << 7);

    I2C1_MCR_R = ( 1<<4 );
    I2C1_MTPR_R = ( 1 << 0) | ( 1 << 1 ) | ( 1 << 2 ); // setting up for 100kbps with system clock at 16MHz
    I2C1_MSA_R  =  0xC2 ;   // 0xC3 for device connection testing                           // Slave address, connecting A0 to Vdd , setting R/W = 0, device is written to



    SYSCTL_RCGC2_R |= (1 << 5);                        // LED config
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DEN_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DIR_R = (1 << 1) | ( 1 << 2)|(1 << 3);
    GPIO_PORTF_DATA_R &= ~((1 << 1)|( 1 << 2)|(1 << 3));
    GPIO_PORTF_PUR_R = 0x11;



    while(1){

        if(temp==0){
                    datawrite(0x40,( 1 << 0) | ( 1 << 1 ));
                    temp=1;
                    }
                    else{
                        datawrite(0x40,( 1 << 0) );

                    }


        if (increment) {
            // Increment mode
            if (x < 0xFF) {
                datawrite(x, (1 << 0)); // Write the current value of x
                datawrite(y, (1 << 0)); // Write the current value of y
                x += v; // Increment x by v

                // Check if the limit is reached for x
                if (x >= 0xFF) {
                    increment = false; // Switch to decrement mode
                    x = 0xFF; // Clamp x to its maximum value
                }
            }
        } else {
            // Decrement mode
            if (x > 0x00) {
                datawrite(x, (1 << 0)); // Write the current value of x
                datawrite(y, (1 << 0)); // Write the current value of y
                x -= v; // Decrement x by v

                // Check if the lower limit is reached for x
                if (x <= 0) {
                    increment = true; // Switch back to increment mode
                    x = 0; // Clamp x to its minimum value
                }
            }
        }


    }
        }



void datawrite( int data , int conditions){

  //  while(I2C1_MCS_R & ( 1 << 0) != 0)                 // checking for end of transmission
      //    {
        //   GPIO_PORTF_DATA_R = (1 << 1);                                           // doing nothing
       // }

    I2C1_MDR_R = data;

    I2C1_MCS_R = conditions;



    //GPIO_PORTF_DATA_R &= ~(1 << 1);
    while(I2C1_MCS_R&0x01 !=0)
     if ( I2C1_MCS_R & ( 1 << 1) != 0 ) {               // checking for errors


         GPIO_PORTF_DATA_R = (1 << 1);                  // The error can be from the slave address not being acknowledged

           // GPIO_PORTF_DATA_R = (1 << 3);

                                                          // or the transmit data not being acknowledged

         }
     }

