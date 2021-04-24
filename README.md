# Microbalance

Idk what to put here really but heres the basic details: (also i wish i could use main instead of master but VS code doesnt like that so this it is)

This is the arduino code for a microbalance im building, roughly based off of this persons work: http://fab.cba.mit.edu/classes/863.19/CBA/people/zach/week09.html

I've 3D printed a flexure very similar to his, and mounted a mirror to the pivot and top arm, both of which are actually silicon wafers from old memory i dissassembled as per this video: https://www.youtube.com/watch?v=ZQeHHYJYWXo
They have horrible(~30%) reflectivity with the ~650nm laser (from a dollar store) im using, but theyre very flat so its the best i have

Right now im working on adding full servo feedback, with 2 ultrabright IR leds from adafruit as photodiodes, used in photovoltaic mode with 10Mohm feedback and a TLV2462 dual rail-to-rail op amp.
Without any feedback and just with a readout (averaged cause theres a lot of noise from air movement and whatever) on a Nokia 5110 LCD, it looks to be able to measure about 0.25mg.
That's with ~0.25mm thinned down flexures(by hand with an x-acto knife), and about a 1m beam length. if anyone ever sees this, i guess add a pull request or an issue and ill post pictures or help you out.

Current targeted time for adding servo functionality in a rudimentary form is about 2021-04-25. if it needs better PID control it might take a while.

Oh and the actuator is a voice coil and magnet from a pair of airplane headphones, 0.5v max ish. planning on using a low pass filtered pwm signal, hopefully the capacitor needed for reasonable vibration isnt too large.

Anyways I know this isnt really the right style but who cares, peace and love and good luck if you try to use this code lol

Knotimpressed
