# AttaHell
Bat Call Simulator for Pi-pico and a 40khz Piezo sounder

Intended to fuse testing Bat Listining devices.

Sends 325Khz sampled Bat calls to a Piezo Sounder connected between pins GPIO12 & GPIO14  
The calls (10 currently) can be selected by grounding GPIO6 GPIO7 GPIO8 & GPIO9
Details of the Calls are in the sound/sample.c 

GPIO16 & 17 give a 40khz unmodulated drive for a piezo  

Calls are repeted every 1-2 seconds and indicated by the onboard LED

Earthing GPIO10 slows down the output by 20x so the call can be heard in a human audio range. You will need to repleace the 40khz sounder above with a small speaker to hear this. 

For testing Earthing GPIO6 and GPIO7 gives a good signal with a 40khz sounder. As this is pure square wave its louder than the modulated signals above. Ideal for testing hetrodyne receivers.

# NOT TO BE USED FOR ATTRACTING BATS # 
Its illegal to simulate bat calls for attracting bats
