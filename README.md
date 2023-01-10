# <h1>Comp-444-Project</h1>

My project for Embedded/Robotic Programming (COMP 444) from Athabasca University. Students were required to design a completely autonmous robot that takes in at least one input from the environment and performed at least one physical function. Additionally students were required to only use the Arduino as their primary microcontroller. What the robot did and how it completed the task was left completely to the student.

My idea was to create a robot to help me with my hearing disability. As a result of my hearing disability I am unable to hear any high pitch sounds without some form of assistence. The robot was to detect high pitch noise, locate the origin of the noise and then alert me when found. 

The hardware for the project:
FIRST HALF
1x Arduino/Genuino Uno 1x Sparkfun baseplate 1x Motor driver 2x Gear motors 2x Wheels 14x jumper wires 1x battery holder 4x AA batteries 1x Dual lock tape

SECOND HALF
1x Arduino/Genuino Uno 1x Switch 2x LED 3x 330 Ohm resistor 1x LM 386 Sound Sensor 1x Adafruit Electret Microphone Amplifier - MAX4466 with Adjustable Gain 12x Jumper Wires 3x Male/Female Jumper Wires 4x Jumper Wires

The set up can be found in <a href="https://github.com/moranski7/Comp-444-Project/blob/master/Michael%20Oransi%202743708%20Project.pdf">Michael Oransi 2743708 Project.pdf</a>

The project make use of I2C_Anything library found at https://github.com/nickgammon/I2C_Anything.

What works:
- Sound detect
- movement behavoir
- PID feedback
