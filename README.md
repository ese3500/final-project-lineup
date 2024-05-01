[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/2TmiRqwI)
# final-project-skeleton

    * Team Name: Line Up
    * Team Members: William Tun & Zwe Tun
    * Github Repository URL: https://github.com/ese3500/final-project-lineup
    * Github Pages Website URL: (https://ese3500.github.io/final-project-lineup/)
    * Description of hardware: laptops

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a Github pages website before, you can follow this webpage (though, substitute your final project repository for the Github username one in the quickstart guide):  <https://docs.github.com/en/pages/quickstart>

### 1. Video

Demo: https://youtu.be/CZTWxAPe4Xg

I2C: https://youtu.be/aLGZ5tMLts8

US Sensor: https://youtu.be/iNkS-_jFNoQ

### 2. Images
![alt text](https://github.com/ese3500/final-project-lineup/blob/751d02cdddba78f7090d7a660f3e2fa0d1aff0e5/BLock%20Diagram.jpg)
![alt text](https://github.com/ese3500/final-project-lineup/blob/751d02cdddba78f7090d7a660f3e2fa0d1aff0e5/Circuit%20Diagram.png)
![alt text](https://github.com/ese3500/final-project-lineup/blob/751d02cdddba78f7090d7a660f3e2fa0d1aff0e5/20240430_222506.jpg)
![alt text](https://github.com/ese3500/final-project-lineup/blob/751d02cdddba78f7090d7a660f3e2fa0d1aff0e5/20240430_222501.jpg)

### 3. Results

What were your results? Namely, what was the final solution/design to your problem?

Our final design was a motorized pulley that utilized an atmega328pb and three sensors to control the motors. Our pulley used two DC geared motors to move up and down a zipline. These two geared motors were controlled through an H bridge that the atmega communicated to via PWM signal for speed and setting certain pins high or low on the H bridge to set direction. A twelve volt battery was connected to the H bridge to power the motors. Two ultrasonic sensors were attached on either side of the pulley to read distance. Those two sensors were used to detect the ends of the zipline, and would either send the pulley back or stop the pulley before hitting a pole. When the device approaches the end, the pulley stops and then turns both servos attached to the bottom of the pulley to drop off whatever package it's carrying. Afterwards the pulley's direction would be reversed to allow the device to go back to where it started. Finally, the last sensor was an accelerometer that sent acceleration data to the atmega328pb to control speed. The atmega would take that data and calculate the pulleys current velocity by adding up acceleration values over small periods of time.  

#### 3.1 Software Requirements Specification (SRS) Results

At the end our device did not need ADC instead it utilized timers, PWM, and input capture interrupts to control the motors. 

On the atmega328PB, we utilized Timer 0 to send a PWM signal to the motor whose duty cycles we controlled through code. Timers 1 and 3 used input capture for either ultrasonic, and timer 2 also controlled a PWM signal to control the servo motors. 

Instead of time of flight sensors, we ended up using ultrasonic sensors due to time restraints. However, our ultrasonic sensors were capable of detecting an object within 35 cm, giving us enough time to slow down our pulley. Input capture interrupts allowed us to calculate distance via measuring the pulse time on the ultrasonic's echo pin. 

Our accelerometer was capable of calculating an approximate velocity but that velocity was not accurate as the velocity calculation had no priority and was simply done round robin with the other tasks. 
Similarly, the 20 ms update rate was not achieved. Although would've been done through a external pin update connected to accelerometer, as the accelerometer was capable of sending a interrupt whenever new data was captured, and the frequency that the accelerometer captured data was controllable through a register accessible via I2C. Acceleration was simply calculated by keeping a running velocity value that added signed acceleration values multiplied by a small constant time factor. Accuracy was measured for constant acceleration 


#### 3.2 Hardware Requirements Specification (HRS) Results

Based on your quantified system performance, comment on how you achieved or fell short of your expected hardware requirements. You should be quantifying this, using measurement tools to collect data.

Our device was capable of moving bilaterally through an H bridge controlled by the atmega328pb. Although movement along a zipline was not repeatable due to differences between motorshaft and wheels. We utilized tape and glue to attach our motors to the wheels of our pulley but the tape would eventually strip, leaving the motors to spin inside the wheels along the zipline. The bilateral movement was controlled through GPIO pins that would communicate direction to the H bridge depending on ultrasonic info. 

Our device was capable of using slowing down the motors before a post depending on distance through a PWM signal whose duty cycle was controlled by the microcontroller. Additionally, for a stop, the microcontroller would slow down the duty cycle to 0 before setting the H bridge on break mode. 

Our device was capable of dropping off packages through 20 kg servos with bars attached to them, such that at objects could be hooked onto those bars and be held up when the servo was in one position and objects would fall depending if the servo was in a different position. 

There was no user interface other than two physical switches. 

### 4. Conclusion

Reflect on your project. Some questions to consider: What did you learn from it? What went well? What accomplishments are you proud of? What did you learn/gain from this experience? Did you have to change your approach? What could have been done differently? Did you encounter obstacles that you didn’t anticipate? What could be a next step for this project?

William:
From the project, I learned about motors, i2c, using batteries, and how to test systems before implementing them into a project. Before this project, I had no experience with controlling motors other than through cheap servos on an arduino. Through this project, I learned not only how to control a DC motor but I also got experience with how motors function in the real world. For example, in the initial steps of the project, our initial research on what we should look for in motor involved how motors generate torque versus angular velocity. In that initial research, I learned that more current equaled more torque but I didn't understand how that happened.
It was only until getting our motors and hooking them up to an ammeter that the motor, in order to keep spinning against some resistance, would try to draw more current. Similarly, I relearned about H bridges and got experience actually using one. Initially, we tried to make one ourselves, but we burnt an atmega as a result. Otherwise, I'm proud that we were able to move past quite a few step backs. Through that process we went through both of our atmegas, got our parts fairly late, and ended up shearing multiple 3d printed adapters. I'm particularly proud of our i2c capibility since a lot of that was my work but, we couldn't really use it in the project since, with everything else, our atmega would run very slow. In the future, I think we could get this project properly working, by using a more powerful microcontroller and by getting proper motor shaft adaptors for our wheels. By the end of it, I think both Zwe and I are a lot more comfortable around physical circuits than before. 

Zwe:
This project taught me many new things and there were a lot of surprising outcomes. In the beginning our plan mainly focused on the software and electronics to get up and running as soon as possible. The electronics, and software challenges were aticipated to a degree such as as how we were going to wire, and how the code should integrate. Here I want to focus on a specific part that we did not see coming. During our first test on the rope, after we got the sensors and actuators running, we realized that we had not paid enough attention to the hardware limitation of our device. Mainly the weight, and the torque our motors were able to output. In theory our electronics and software should have allow the device to glide effortlessly across the rope, but we soon found out our initital motors we ordered did not output enoough torque. On the rope our motors were screeching trying to turn, but the weight of the device was too much to bare. We realized adding to this challenge was the fact our motor shaft did not mesh with the pulley exactly. The tape we were using as a buffer to counteract this problem also tore easily. At this moment we learned that the powerful motors we had bought in the first place were not geared. This meant that although it spun super fast it was not strong enough to move our device. The demo deadline was fast approaching so we had to pivot quickly. Luckily the Detkin lab staffs were able to help us find two geared motor. These motors were much slower, but when we put the device on the rope it was able to move it tempoarily until our tape wore out. Overall I am super happy we got our electronics and code working since that was the focus of the class. In the future I would like to place a greater emphasis on the hardware aspect especially when dealing with a moving device. This could be for example doing center of mass calculations, and researching the best fit motors for the project.

## References

Fill in your references here as you work on your proposal and final submission. Describe any libraries used here.

## Github Repo Submission Resources

You can remove this section if you don't need these references.

* [ESE5160 Example Repo Submission](https://github.com/ese5160/example-repository-submission)
* [Markdown Guide: Basic Syntax](https://www.markdownguide.org/basic-syntax/)
* [Adobe free video to gif converter](https://www.adobe.com/express/feature/video/convert/video-to-gif)
* [Curated list of example READMEs](https://github.com/matiassingers/awesome-readme)
* [VS Code](https://code.visualstudio.com/) is heavily recommended to develop code and handle Git commits
  * Code formatting and extension recommendation files come with this repository.
  * Ctrl+Shift+V will render the README.md (maybe not the images though)
