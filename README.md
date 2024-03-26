[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/2TmiRqwI)
# final-project-skeleton

    * Team Name: Line Up
    * Team Members: William Tun & Zwe Tun
    * Github Repository URL: https://github.com/ese3500/final-project-lineup
    * Github Pages Website URL: [for final submission]
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
We will be using: 
PWM and timers to manage the motors
ADC and interrupts for the sensors


### 5. Hardware Requirements Specification (HRS)
Our actuators will include four motors that will be used to drop off items, and move the zipline across rope. 

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
		      |  Researching motor controls + distance sensor | Zwe      |
| Week 2: 4/1 - 4/7   |  Motor code                                   | William  |
		      |  Distance sensor code                         | Zwe      |
| Week 3: 4/8 - 4/14  |  Distance Calculations                        | William  |
		      |  Servo Controls                               | Zwe      |
| Week 4: 4/15 - 4/21 |  Cadding and Assembling                       | William  |
		      |  Cadding and Assembling                       | Zwe      |
| Week 5: 4/22 - 4/26 |  Assembling and final touches        	      | William  |
		      |  Assembling and final touches                 | Zwe      |

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

#### 3.1 Software Requirements Specification (SRS) Results

Based on your quantified system performance, comment on how you achieved or fell short of your expected software requirements. You should be quantifying this, using measurement tools to collect data.

#### 3.2 Hardware Requirements Specification (HRS) Results

Based on your quantified system performance, comment on how you achieved or fell short of your expected hardware requirements. You should be quantifying this, using measurement tools to collect data.

### 4. Conclusion

Reflect on your project. Some questions to consider: What did you learn from it? What went well? What accomplishments are you proud of? What did you learn/gain from this experience? Did you have to change your approach? What could have been done differently? Did you encounter obstacles that you didn’t anticipate? What could be a next step for this project?

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
