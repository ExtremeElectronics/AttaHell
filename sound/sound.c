/**
 * https://github.com/ExtremeElectronics/pippestrelle
 *
 * sound.c part of attahell
 * https://github.com/ExtremeElectronics/AttaHell
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
**/

#include <string.h>
#include "malloc.h"
#include "stdarg.h"
#include <stdio.h>
#include <stdlib.h>

//pico stuff
#include "pico/multicore.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"

#include "sound.h"
#include "../settings.h"

#include "sample.c"

//sound buffer
//const uint8_t * bufferptr = &BatSounds[0];

//pwmslices
uint PWMslice1; //dma pwm1
uint PWMslice2; //dma pwm2

uint PWMslice40khz; //40khz

//dma channels
int pwm_dma_chan1;
int pwm_dma_chan2;

dma_channel_config pwm_dma_chan1_config;
dma_channel_config pwm_dma_chan2_config;

int ptimer1 ; //dma pacing timer
int ptimer2 ;

void SetPWM2(void){
    //setup PWM GPIO
    gpio_init(soundIO3);
    gpio_set_dir(soundIO3,GPIO_OUT);
    gpio_set_function(soundIO3, GPIO_FUNC_PWM);

    gpio_init(soundIO4);
    gpio_set_dir(soundIO4,GPIO_OUT);
    gpio_set_function(soundIO4, GPIO_FUNC_PWM);

    //get slice from gpio pin
    PWMslice40khz=pwm_gpio_to_slice_num(soundIO3);

    //setup pwm
    pwm_set_clkdiv(PWMslice40khz,PICOCLOCK*1000/40000/256);//40khz
    pwm_set_both_levels(PWMslice40khz,128,128);
    pwm_set_output_polarity(PWMslice40khz,true,false);
    pwm_set_wrap (PWMslice40khz, 255);
    pwm_set_enabled(PWMslice40khz,true);
}

void PWMOn(int x){
    if (x){
        pwm_set_both_levels(PWMslice40khz,128,128);
    }else{
        pwm_set_both_levels(PWMslice40khz,0,0);
    }
}

void SetPWM(void){
    //setup PWM GPIO
    gpio_init(soundIO1);
    gpio_set_dir(soundIO1,GPIO_OUT);
    gpio_set_function(soundIO1, GPIO_FUNC_PWM);

    gpio_init(soundIO2);
    gpio_set_dir(soundIO2,GPIO_OUT);
    gpio_set_function(soundIO2, GPIO_FUNC_PWM);

    //get slice from gpio pin
    PWMslice1=pwm_gpio_to_slice_num(soundIO1);
    PWMslice2=pwm_gpio_to_slice_num(soundIO2);
 
    //setup pwm
    pwm_set_clkdiv(PWMslice1,1);//loads of mhz
    pwm_set_both_levels(PWMslice1,0,0);
    pwm_set_output_polarity(PWMslice1,false,true);
    pwm_set_wrap (PWMslice1, 255);
    
    //setup pwm2 inverted
    pwm_set_clkdiv(PWMslice2,1);//loads of mhz
    pwm_set_both_levels(PWMslice2,0,0);
    pwm_set_output_polarity(PWMslice2,true,false);
    pwm_set_wrap (PWMslice2, 255);

    //enable
    pwm_set_enabled(PWMslice1,true);
    pwm_set_enabled(PWMslice2,true);
    
    //setup DMA
    
    //pwm DMA channel config
    pwm_dma_chan1 = dma_claim_unused_channel(true);    
    pwm_dma_chan1_config = dma_channel_get_default_config(pwm_dma_chan1);
    channel_config_set_transfer_data_size(&pwm_dma_chan1_config, DMA_SIZE_16);
    channel_config_set_read_increment(&pwm_dma_chan1_config, true);
    channel_config_set_write_increment(&pwm_dma_chan1_config, false);

    pwm_dma_chan2 = dma_claim_unused_channel(true);
    pwm_dma_chan2_config = dma_channel_get_default_config(pwm_dma_chan2);
    channel_config_set_transfer_data_size(&pwm_dma_chan2_config, DMA_SIZE_16);
    channel_config_set_read_increment(&pwm_dma_chan2_config, true);
    channel_config_set_write_increment(&pwm_dma_chan2_config, false);

    //dma dreq by timer  
    int treq1 = dma_get_timer_dreq(ptimer1);
    int treq2 = dma_get_timer_dreq(ptimer2);
    channel_config_set_dreq(&pwm_dma_chan1_config, treq1); //Select a transfer request signal. timer0??
    channel_config_set_dreq(&pwm_dma_chan2_config, treq2); //Select a transfer request signal. timer0??
    
    printf("Set PWM done\n");    
}

void startDMA(int sample,int speed){
//      printf("Start DMA Single\n");
    if(sample<MAXSAMPLES){
        uint32_t f=BatFrameRate[sample] ;
        if(speed){
            f=f/20;        
        }

        printf("Sample %i, Rate %i, (Div %i)\n",sample,f,speed*20);
        uint32_t dmafreq=PICOCLOCK*1000/f;

        dma_timer_set_fraction(ptimer1, 1, dmafreq);  
        dma_timer_set_fraction(ptimer2, 1, dmafreq);  

//        printf("DMA Freq %f\n",(float)PICOCLOCK*1000/dmafreq);

        dma_channel_configure(
            pwm_dma_chan1, &pwm_dma_chan1_config,
            &pwm_hw->slice[PWMslice1].cc, // Write to PWM counter compare
            //BatSounds, // Read values from waveshapes
            BatArray[sample],
            //BatSound4,
      //      (uint32_t)SAMPLESIZE, // no values to stream
            BatLength[sample], // no values to stream
            false // Start.
        );

        dma_channel_configure(
            pwm_dma_chan2, &pwm_dma_chan2_config,
            &pwm_hw->slice[PWMslice2].cc, // Write to PWM counter compare
            //BatSounds, // Read values from waveshapes
            //BatSound4,
            BatArray[sample],
     //       (uint32_t)SAMPLESIZE, // no values to stream
            BatLength[sample], // no values to stream
            false // Start.
        );

        //start DMA's
        dma_channel_start(pwm_dma_chan1);
        dma_channel_start(pwm_dma_chan2);
    }else{
        printf("Sample selection %i not valid\n",sample);
    }
//    printf("Started ");
}

int DMADone(void){
    return (dma_channel_is_busy(pwm_dma_chan1) || dma_channel_is_busy(pwm_dma_chan2))==0;
}

void init_sound(void){

//    uint32_t f=FRAMERATE;

    ptimer1 = dma_claim_unused_timer(true /* required */);
    ptimer2 = dma_claim_unused_timer(true /* required */);
    
    SetPWM();

    printf("Sound INIT Done\n");    
}


// DFA in core1
void Sound_Loop(void){
    while(1){
        sleep_us(100);
//        printf(".");
    }
}
