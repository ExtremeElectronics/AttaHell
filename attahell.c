/*
 * AttaHell - Bat Caller
 *
 * https://github.com/ExtremeElectronics/AttaHell
 *
 * attahell.c
 *
 * Copyright (c) 2025 Derek Woodroffe <tesla@extremeelectronics.co.uk>
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
//#include "hardware/i2c.h"

#include "pico/multicore.h"

#include "settings.h"

#include "sound/sound.h"
#include "sound/sample.h"

//#include "sound/sample.c"

#define MINDISTANCE 30
#define MAXDISTANCE 450 //was 350
#define MAXDISTANCEV 300

#define SMOOTHING 2.7 //was 4 larger smoother/slower

//globals
float frequency=1000;
uint8_t volume=255;
uint8_t extern mute;
uint8_t extern vmute;
uint8_t extern fmute;
int8_t wave=0; //from table in waveshapes.c

char dtemp[100];

char timecompiled[20];

uint8_t nextsample=0;
uint8_t autosample=1;

//button timer
struct repeating_timer btimer;
int lb=0;//debounce

int sb;//sample by button

/*
int GetSampNo(void){
    int r=0;
    if(gpio_get(ChSel0)==0)r+=1;
    if(gpio_get(ChSel1)==0)r+=2;
    if(gpio_get(ChSel2)==0)r+=3;
    if(gpio_get(ChSel3)==0)r+=8;
    if (r>=MAXSAMPLES)r=0;
    if(r==0){
      r=nextsample; 
      printf("Auto selecting sample %i\n",r);
      nextsample++;
      if(nextsample==MAXSAMPLES)nextsample=0;
    }else{
      r--;
      printf("Switches set to sample %i\n",r);

    }
    
    return r;
}
*/


bool Button_Callback(struct repeating_timer *t){
    if(gpio_get(Button)==0){
      gpio_put(PICO_DEFAULT_LED_PIN, 0);
      if(lb==0){
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sb=sb+1;
      }  
      lb=1;
      if (sb==MAXSAMPLES) sb=0;
      //stop auto selecting samples
      autosample=0;
    }else{
      lb=0;
    }
    return true; //keep going       
}

int GetSampNo(void){
    int r=0;

//    if (r>=MAXSAMPLES)r=0;
    if(autosample==1){
      r=nextsample;
      printf("Auto selecting sample %i\n",r);
      nextsample++;
      if(nextsample==MAXSAMPLES)nextsample=0;
    }else{
       //selection from buttons
       printf("Button Sample %i\n",sb);
       r=sb;
    }

    return r;
}



void gpio_conf(){
    gpio_set_dir(Button, GPIO_IN);
    gpio_pull_up(Button);
    
/*    gpio_set_dir(ChSel0, GPIO_IN);
    gpio_pull_up(ChSel0);
    
    gpio_set_dir(ChSel1, GPIO_IN);
    gpio_pull_up(ChSel1);
    
    gpio_set_dir(ChSel2, GPIO_IN);
    gpio_pull_up(ChSel2);
    
    gpio_set_dir(ChSel3, GPIO_IN);
    gpio_pull_up(ChSel3);
*/
    gpio_set_dir(ChSpeed, GPIO_IN);
    gpio_pull_up(ChSpeed);

    gpio_set_dir(LEDPIN, GPIO_OUT);
    
    
}

void splash(){
    sprintf(timecompiled,"%s",__DATE__);
    printf("\n                                                  ");
    printf("\n                                                  ");
    printf("\n                                                  ");
    printf("\n                                                  ");
    printf("\n              Extreme Kits                      ");
    printf("\n              extkits.uk                        ");
    printf("\n");
    printf("\n             /\\               /\\               ");
    printf("\n            /  \\             /  \\                   ");
    printf("\n           /____\\___________/____\\                ");
    printf("\n          /        AttaHell        \\               ");
    printf("\n         /                          \\              ");
    printf("\n         ----------------------------              ");
    printf("\n                                                  ");
    printf("\n      Copyright (c) 2025 Derek Woodroffe          ");
    printf("\n                %s  ",timecompiled);
    printf("\n\n");  
        

}





// ******************************** Main ************************************
int main() {


    set_sys_clock_khz(PICOCLOCK, false);
    stdio_init_all();

    gpio_conf();
    sleep_ms(2000); //wait for serial usb

    splash();

    SetPWM2();
    init_sound();

    printf("Starting Button timer\n");
    add_repeating_timer_ms( 50, Button_Callback, NULL, &btimer); //
    

    printf("**************** STARTING **************** \n\n");

   //FLASH LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while(1)
    {       
       PWMOn(0);
       sleep_ms(2900);
       gpio_put(LEDPIN, 1);
       if (DMADone()){
           int sn=GetSampNo();   
           int sp=(gpio_get(ChSpeed)==0);
           startDMA(sn,sp);
       }
       PWMOn(1);
       sleep_ms(100);
       gpio_put(LEDPIN, 0);
       printf(".");
    } 
}



