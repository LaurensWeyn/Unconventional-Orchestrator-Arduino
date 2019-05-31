# Unconventional-Orchestrator-Arduino
'Everything you need to play music on things never designed to play music'

# About

This is the Arduino code for the 'Universal Instrument Controller' used in my UNcoventional Orchestrator project. The main program is here: https://github.com/thatdude624/Unconventional-Orchestrator-Java

This Arduino sketch will take UIC commands from the main program and send them to standard floppy drives. If you ignore the 'direction' (and 'enable', if you want) line, this could control just about any stepper motor.


# How to use this

Open this program in pretty much any version of the Arduino IDE.

At the top of the file are the settings. The first two are called 'FINST' and 'DINST'. This is for the number of 'Floppies' and 'Drums', respectively. Set these to how many floppies and/or drums you are connecting to this Arduino.

## Setting up Floppies

The 'startFlop' variable defines which pin is the first pin to be used for the floppy drives. Since pins 0 and 1 are used for the computer-Arduino communication, the lowest value you can use here is 2. You can set a higher value though, if you want to use the first few pins for percussion (or have a Mega with I/O to spare). By default, the pin arrangement is:

- Pin 2: floppy 0 STEP
- Pin 3: floppy 0 DIRECTION
- Pin 4: floppy 0 ENABLE
- Pin 5: floppy 1 STEP
- Pin 6: floppy 1 DIRECTION
- etc.

If you, for some reason, want the 'direction' line to come before the 'step line' for each floppy, you can set the 'stepOfff', 'dirOff' and 'enOff' variables to something different than their current default 0, 1 and 2 respectively. 

the step, direction and enable lines are acessible on the floppy drive data header. Look online for the pinout of this, and be sure to supply each drive with enough power. Technically you could just wire the 'enable' pin to 5V, as many tutorials will tell you, and yes this will work here too, but you'll miss out on the blinky lights, since the enable pin also controls the front panel 'activity' LED.

## Setting up Percussion

The Arduino does not have enough power on any of its I/O pins to drive solenoids and such directly. You'll need a driver of some sort; a simple transistor switch will work. You can find many tutorials online for this.

the length of the pulse for all drums is controlled by the 'maxDrum' variable, it's 50 milliseconds by default. This variable may require tweaking depending on what things you use as percussion.


For each drum, you'll need to add an element to the 'drumPin' and 'drumStat' array. The drumPin array holds the pin number for each percussion output. For the drumStat array, just add an extra 'false' to the list. I should really get rid of that in a later version.



