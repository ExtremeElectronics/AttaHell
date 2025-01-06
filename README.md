# AttaHell
Bat Call Simulator for Pi-pico and a 40khz Piezo sounder

Intended use, testing Bat Listining devices.

Sends 325Khz samples/second encoded Bat calls to a Piezo Sounder connected between pins GPIO12 & GPIO14  
With No other connections the bat calls will rotate through each. 

The calls (10 currently) can be selected by grounding GPIO6 GPIO7 GPIO8 & GPIO9.
Confusingly grounding a pin is seen as a 1, and the calls are numbered from 0

|       | GPIO6    | GPIO7    | GPIO8    | GPIO9 |
|Auto   | floating | floating | floating | floating |
|Call 0 | Gnd      | floating | floating | floating |      
|Call 1 | floating | Gnd      | floating | floating |
|...    |          | | | |
|Call 9 | floating | Gnd    | floating | Gnd |

Details of the Calls are in the sound/sample.c 

GPIO16 & 17 pins have a 40khz unmodulated drive for a piezo, ideal for testing hetrodyne receivers (heard as a tone only)

Calls are repeted every 3-4 seconds and transmission is indicated by the Pico's onboard LED

Earthing GPIO10 slows down the output by 20x so the call can be heard in a human audio range.  If you are using a 40khz sounder, you will need to repleace with a small speaker to hear this. 

As this is pure square wave its louder than the modulated signals above. Ideal for testing hetrodyne receivers.

# NOT TO BE USED FOR ATTRACTING BATS # 
It's illegal to simulate bat calls for attracting bats.

Sounders tried 

40khz transmitter (generic), OK but not very wide band

https://rcl.lt/files/a37371507b9a8a701055?inline

Tweeter, Better, but gives auidable harmonics?

https://www.aliexpress.com/item/1005006896572004.html

## notes ##
Samples 7 8 & 9 appear to be corrupt :) 
