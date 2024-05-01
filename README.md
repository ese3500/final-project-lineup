[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/2TmiRqwI)
# final-project-skeleton

    * Team Name: Line Up
    * Team Members: William Tun & Zwe Tun
    * Github Repository URL: https://github.com/ese3500/final-project-lineup
    * Github Pages Website URL: (https://ese3500.github.io/final-project-lineup/)
    * Description of hardware: laptops

## Final Project Proposal

### 1. Abstract

   A zipline pulley that can move both ways along a zipline no matter the angle. The zipline pulley will mainly function as a easy to use delivery device that can carry light weight items and either drop them off at the end of the zipline or along the way. 

### 2. Motivation
In more rural and mountainous areas, typical transportation infrastructure can be both expensive and difficult to develop. In comparison, ziplines are much simpler as all they need are some posts and correct sized ropes. However, traditional ziplines face the issue of only working in one direction and as such lose much of their use. To solve that problem, our zipline would be motorized such that it could move back and forth along the line. Using the atmega328pb, we will also add additional functionality such as the ability to slow down before a post, and ability to drop packages off midway. 

### 3. Goals
At the end we should have a working zipline package delivery device. This device will run on a cable. We will be able to control the position the device drops its packages. It will have 2 attachments/hooks for packages, and servos will be used to drop them autonomously. Two motors along with their electronic speed controllers(ESC) will control the device. A gyroscope will also be installed so the microcontroller can increase power to the motor when needed such as on an incline. In addition there are two lidar sensors on each side so it can detect distances to stop safely. Two rechargeable batteries will be used. One for the motors and one for the microcontroller. Finally the finish will be mostly out of 3D printed parts.
These are to help guide and direct your progress.

### 4. Software Requirements Specification (SRS)

Our device will utilize PWM, ADC, and interrupts to control the motors of the device along the zipline.

Our users would be people looking for delivery systems in mountainous region 

Our signal duty cycle will update every 20 mS

Our accelerometer will calculate an approximate velocity through adding the current acceleration times a small time step added onto a total velocity

Our time of flight signals will recognize a stationary object in front of the pulley and slow down the pulley before impact

### 5. Hardware Requirements Specification (HRS)

Our device shall move bilaterally across a zipline
Our device shall be able to drop packages 
Our device shall slow down before hitting an end of the zipline
Our device should have a user interface

### 6. MVP Demo

We should be able to move across the zipline no matter the direction, stop before hitting an end and be able to move to a distance along the zipline.  

### 7. Final Demo

The pulley should have a proper chasis and be able to move across the zipline, accurately drop off packages along the zipline and stop before hitting either end of the zipline. 

### 8. Methodology
We will first focus on a prototype that can move back and forth along a rope. Afterwards, we will incorporate distance sensors that will detect posts and slow down the device before hitting a post. Then we will incorporate dropping functionality as well as the ability to drop off midway at a specific distance along the rope. 


### 9. Components
We will need two motors to move the zipline along the rope that can at least carry ten pounds of weight, a gyroscope that measures angle will be necessary to calculate the angle of the rope and determine the effects of gravity along the zipline, two distance sensors to detect posts before hitting them, an ESC to control our motors and two batteries to power the motors and the atmega328pb + peripherals.   

### 10. Evaluation
We will be making sure the location the package is dropped off is correct using a meter stick, and will also see if the device comes to a complete stop at the end of the line. We will try different drop off locations to test precision and accuracy. 

### 11. Timeline

This section is to help guide your progress over the next few weeks. Feel free to adjust and edit the table below to something that would be useful to you. Really think about what you want to accomplish by the first milestone.

| **Week**            | **Task** | **Assigned To**    |
|----------           |--------- |------------------- |
| Week 1: 3/24 - 3/31 |  Pulley Kinematics                            | William  |
||  Researching motor controls + distance sensor | Zwe      |
| Week 2: 4/1 - 4/7   |  Motor code                                   | William  |
||  Distance sensor code                         | Zwe      |
| Week 3: 4/8 - 4/14  |  Distance Calculations                        | William  |	  
||  Servo Controls                               | Zwe      |
| Week 4: 4/15 - 4/21 |  Cadding and Assembling                       | William  |	 
||  Cadding and Assembling                       | Zwe      |
| Week 5: 4/22 - 4/26 |  Assembling and final touches        	      | William  |
||  Assembling and final touches                 | Zwe      |

### 12. Proposal Presentation

Add your slides to the Final Project Proposal slide deck in the Google Drive.

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a Github pages website before, you can follow this webpage (though, substitute your final project repository for the Github username one in the quickstart guide):  <https://docs.github.com/en/pages/quickstart>

### 1. Video

[Insert final project video here]

### 2. Images

[Insert final project images here]

### 3. Results

What were your results? Namely, what was the final solution/design to your problem?

Our final design was a motorized pulley that utilized an atmega328pb and three sensors to control the motors. Our pulley used two DC geared motors to move up and down a zipline. These two geared motors were controlled through an H bridge that the atmega communicated to via PWM signal for speed and setting certain pins high or low on the H bridge to set direction. A twelve volt battery was connected to the H bridge to power the motors. Two ultrasonic sensors were attached on either side of the pulley to read distance. Those two sensors were used to detect the ends of the zipline, and would either send the pulley back or stop the pulley before hitting a pole. When the pulley first hits a pole, the pulley stops and then turns both servos attached to the bottom of the pulley to drop off whatever package it's carrying. The last sensor was an accelerometer that sent acceleration data to the atmega328pb to control speed. The atmega would take that data and calculate the pulleys current velocity by adding up acceleration values over small periods of time.  

#### 3.1 Software Requirements Specification (SRS) Results

We didn't need ADC, and utilized timers, PWM, and input capture interrupts to control the motors. 

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
