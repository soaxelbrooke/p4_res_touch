p4_res_touch
============

PSoC 4 Resitive, Pressure Sensitive Touch Screen
Built in PSoC Creator 2.2 SP 1, intended for the [PSoC 4 Pioneer kit](http://www.cypress.com/?rID=77780).

This example can be broken up into 2 separate problems:

1. Measuring applied force
2. Calculating press location using sensor data

As these are largely separate challenges, they’ll be split into separate sections below.

Measuring Force
---------------
Force measurement can be very simple or very difficult depending on the constraints of your system.  In this system, luckily, constraints make it easy.  Were this a real phone, the sensors would need to be much small, screen printable, cheap, etc.  Suitable technology for these needs exists, but I chose to use a similar but more convenient force sensor.  
Eight of these resistive force sensors are placed underneath the glass, such that they are the only support for it.  This ensures that no force “escapes”, and that the calculation made with this data will be accurate.  A large source of error in these sorts of calculations is unplanned translational forces getting into the measurements, augmenting the location and force of the touch(es).

These force sensors, being essentially variable resistors, are measured with a half-bridge configuration.  This configuration removes common mode noise, which is a significant portion of the noise in the measured signals.  10 kOhm resistors are used, as they were about the resistance of the sensor under the target loading.   The middle of each bridge connection is connected to pins on the PSoC4 for measurement, through the Pioneer Kit’s J2 connector.  The PSoC4 simply measures these signals with a sequencing SAR, with the bridge reference tied to the SAR’s reference input.

![](http://i.imgur.com/hDTtcmn.png "Hardware Configuration")

![](http://i.imgur.com/STedCRT.png "PSoC 4 Internal Configuration")

Calculating Touch
-----------------
Calculation of the touch location is done using the SAR sequencer measurements and the known sensor locations.  The following equations define the location of a single force applied to the screen.  y_p is the vertical location of the touch, and x_p is the horizontal location of the touch. 

![](http://i.imgur.com/wedfSXa.png "X and Y locations of touch")

These equations are implemented in C, relying primarily on multiply-accumulates, and one divide.  PSoC4’s CM0 core handles these calculations easily.

Multitouch
----------
These days, customers assume that smart phones will have multitouch.  Every smart phone does, and they have for the last 4 years.  Multitouch with pressure sensing is possible, but not easy.  With the techniques used above, the locations of the touches would need to be known for their applied force to be resolvable.   This can be done with a traditional capacitive touch screen.

The math for resolving multiple touches is also significantly more complicated, though still well inside the realm of efficient computability for the PSoC4.  
