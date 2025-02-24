# AttaHell #
## Bat Call Simulator for Pi-pico and a 40khz Piezo sounder ##

Intended use - testing Bat Listener / Detector devices.

The Attahell will emit ultrasonic bat calls of varying species via a speaker or ultrasonic transducer. 

Designed to be VERY simple to construct and use as a quick test.

## NOT TO BE USED FOR ATTRACTING BATS ## 
It's illegal (in most countries) to simulate bat calls for attracting bats, without special permission.


## Connections ##

Sounder across GPIO 14 and GPIO 12 - See hardware for details.
Button on GPIO 6 to GND (optional)
Switch or shorting link on GPIO 10 to GND (optional)
LED on GPIO 9 to GND (optional)


## Operation ##

Sends 325Khz samples/second encoded Bat calls to a Piezo Sounder connected between pins GPIO12 & GPIO14  
With No other connections the bat calls will rotate through each. 

The calls are now selected with a single button on GPIO 6 to Gnd.
At startup the calls cycle through automatically.
To Select a call, press the button, each press will cycle through to the next call, after the playing one has stopped, That call will then repeat. 
To restore Auto, reset, or re-apply power to the pico.

Details of the Calls are in the sound/sample.c 

GPIO16 & 17 pins have a 40khz unmodulated drive for a piezo, ideal for testing hetrodyne receivers (heard as a tone only)

To halve the sound output level of either of the above outputs connect the sounder to only one of the GPIO's above and GND

Calls are repeted every 3-4 seconds and transmission is indicated by the LED on GPIO 9

Earthing GPIO10 slows down the output by 20x so the call can be heard in a human audio range.  If you are using a 40khz sounder, you will need to repleace with a small speaker to hear this. 

As this is pure square wave its louder than the modulated signals above. Ideal for testing hetrodyne receivers. (especially deaf ones)


## Hardware ##

Pico 1 and a sounder, yep thats it, no other connections needed. 

Sounders tried 

40khz transmitter (generic), OK but not very wide band

https://rcl.lt/files/a37371507b9a8a701055?inline

Tweeter, Better, but gives auidable harmonics?

https://www.aliexpress.com/item/1005006896572004.html

## Notes ##
Compiled with SDK1 - not tried SDK 2 yet. 

# Bat Ears 
This was created for the development of Bat Ears an easy to build full featured PiPico based Bat Listener/Detector
https://extkits.co.uk/product/bat-ears-bat-detector/

![20250222_105222](https://github.com/user-attachments/assets/fc354019-bebe-4b4f-8d34-88982bd66b75)
