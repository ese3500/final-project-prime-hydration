[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/2TmiRqwI)
# final-project-skeleton

    * Team Name: PRIME Hydration
    * Team Members: Karen Li, Quinn Liu
    * Github Repository URL: https://github.com/ese3500/final-project-prime-hydration
    * Github Pages Website URL: [for final submission]
    * Description of hardware: (embedded hardware, laptop, etc) 

## Final Project Proposal

### 1. Abstract

In a few sentences, describe your final project. This abstract will be used as the description in the evaluation survey forms.

Our final project is an automated alcoholic and non-alcoholic drink mixer meant to take the hassle out of creating fun drinks for you and your friends. This machine will feature an LCD screen to display a custom user interface where users can select which drinks they want to be made, a joystick/button to navigate the user interface, motors to handle the dispensing and mixing of liquids, and lastly an ID scanner (or breathalyzer) to ensure legal consumption. All of these features are meant to take the hassle out of preparing beverages for themselves or guests, and let them "set and forget" their drink menu.  

### 2. Motivation

What is the problem that you are trying to solve? Why is this project interesting? What is the intended purpose?

When hosting social events, preparing to serve guests beverages can be a deceptively daunting task. What do people want to drink? What ingredients do each of the drinks need? How much of each ingredient do I need to buy? How can I make sure that I'm following the law and only serving those over 21? And lastly, how does one make those drinks? Who will make drinks for our guests and when? Our project will hope to answer all these questions by distilling all these questions down to two. What ingredients do I need to buy, and when do I need to refill? This project is intersting because not only does it solve a real-world problem with a tangible solution, but it will feature modularity that will allow users to customize the solution to their own needs.

"If Red Bull gives you wings, Prime Hydration is giving you rockets."

Welcome to your PRIME.

### 3. Goals

These are to help guide and direct your progress.

Core Goals:

1. Have a consistent, controlled, and accurate fluid-dispensing system.

2. Create a user interface that is simple, clean, and intuitive.

3. Design an exterior that allows for easy user entry and replacement of liquids.

4. Find a good balance between speed and precision when making drinks.

Reach Goals:

5. Implement variable mixing speeds depending on the beverage.

6. If the ID scanner is implemented, then implement a breathalyzer, or vice versa.

7. Program different presets, where users can select which set of drinks the machine can make.

8. Allow the user to create custom drink recipes with preset ingredients to be dispensed.

9. Use a distance sensor to sense whether a cup has been placed in the correct position to allow liquid to flow.

10. Monitor the levels of each ingredient supply and alert the user when it is necessary to replace a certain liquid.

11. Create a mobile interface to wirelessly control the device.

### 4. Software Requirements Specification (SRS)

Formulate key software requirements here.

- Intuitive and simple user interface to select which drink to make. This portion of the software will store a "database" of drink recipes and a current selected recipe to make. It will also use the LCD graphics library from Lab 4 to render this interface onto the LCD screen. Evaluated based on responsiveness of interface and ease of use.

- Program that will accurately control the dispensing of different liquids. This portion of the code will keep track of time in order to ensure accurate liquid measurements given known flow rate. 

- Program that can handle ID scanning (or breathalyzer input). This portion of the code will read in an input. The ID scanner may require configuration with an external API while the breathalyzer will require the use of ADC.

### 5. Hardware Requirements Specification (HRS)

Formulate key hardware requirements here.

- MCU: The ATmega328pb will control and coordinate the various components involved in our design.

- Liquid Dispensing Mechanism: An accurate and consistent system for dispensing the ingredients. This will consist of a solenoid valve and tubing system that uses gravity to extract liquids from upright bottles. This mechanism requires precise measurement and controlled flow rates to ensure accuracy of drink preparation. The liquid storage system should also be easy for users to change/refill the contents. This will be evaluated based on the accuracy and consistency of the liquid dispensing.

- Mixing System: After ingredients are dispensed into the cup, they will be mixed using a motor. The mixing will be evaluated on whether or not the ingredients are still distinguishable from each other.

- User Interface: The LCD screen is the main component of the user interface. There will be a joystick or other input component for users to interact with the device to select and make drinks. This will be evaluated on whether 

- Sensors: Sensors and other inputs will detect various parameters such as drink weight/liquid level and (potentially) user identification. 

- Exterior Design: The physical enclosure and materials for the design will house the wiring and components in a compact and aesthetic manner. This will be evaluated based on ease of use and size.

### 6. MVP Demo

What do you expect to accomplish by the first milestone?

We expect to have a machine that depending on user input, can dispense a subset of available liquids at variable and accurate amounts. The LCD screen will display a basic interface and be controlled by an input device such as a joystick.

### 7. Final Demo

What do you expect to achieve by the final demonstration or after milestone 1?

By the final demonstration, we expect our project to be able to create a drink with no manual input from the user other than their drink of choice. This means that the device will have a UI that the user can traverse using some form of input hardware, such as a joystick. Once the user has specified which drink they want, the device will then accurately dispense the ingredients for the specific drink into a cup and then mix it. 

### 8. Methodology

What is your approach to the problem?

We plan to use a systematic and iterative approach to simultaneously develop both software and hardware components.

1. Plan: We will begin by outlining the architecture of the system, including sourcing the appropriate hardware components, designing the user interface layout, and planning the software structure.

2. Hardware Development: Beginning with selecting and sourcing the appropriate components, we will assemble and wire these components. Specific modules will include the interface (LCD screen, joystick), drink dispenser (motor), drink mixer (motor), etc.

3. Software Development: Concurrently, we will develop the corresponding software required for controlling each aspect of the device. Specifically, we will write functions to manage user inputs, sensor readings, drink selection, liquid dispensing, mixign operations, etc.

4. Iteration and Testing: Throughout the process of hardware and software development, we will test isolated modules to ensure correctness before integration.


### 9. Components

What major components do you need and why?

LCD Screen - The screen serves as the main user interface, displaying drink options and status updates. It provides visual feedback to users, allowing them to navigate through the drink selection process. 

Joystick (or other input device) - The joystick enables users to interact directly with the interface, selecting drinks and navigating menus.

Motor - The motor will aid in dispensing and mixing liquids in order to maintain accurate control over the flow rate of ingredients. 

Photoresistor - The light sensor detects the presence of a cup in the designated position to prevent spills and inaccuracies.

Ultrasonic Sensor - The distance sensor verifies the amount of liquid in the drink cup and ingredient source containers.

### 10. Evaluation

What is your metric for evaluating how well your product/solution solves the problem? Think critically on this section. Having a boolean metric such as “it works” is not very useful. This is akin to making a speaker and if it emits sound, albeit however terrible and ear wrenching, declare this a success.
It is recommended that your project be something that you can take pride in. Oftentimes in interviews, you will be asked to talk about projects you have worked on.

We believe the success of our device will not just be whether it works, but rather how consistent and easy it is to use. Here is how we would evaluate the different aspects of the project.

Physical Design:

- How compact is it?

- How difficult will it be to refill the different liquids?

- Are the different components placed in logical places?

- Is it visually appealing?

User Interface/Interaction:

- How intuitive is the user interface?

- Is the interface quick and responsive to use (not frustrating)?

- How many steps and different menus will users have to use?

Drink Ingredient Dispensing:

- How accurate is the liquid dispensing system?

- How consistent is the liquid dispensing system?

Drink Mixing:

- How well does the mixing system mix the ingredients together?

General:

- How does it taste?

### 11. Timeline

This section is to help guide your progress over the next few weeks. Feel free to adjust and edit the table below to something that would be useful to you. Really think about what you want to accomplish by the first milestone.

| **Week**            | **Task** | **Assigned To**    |
|----------           |--------- |------------------- |
| Week 1: 3/24 - 3/31 | finalize + order components         |  K + Q                  |
| | create more detailed plan of attack         | K + Q                   |
| | design interface| Karen|
| Week 2: 4/1 - 4/7   | liquid dispension method         |  Karen                  |
| | LCD screen + joystick setup         | Quinn  
| Week 3: 4/8 - 4/14  | mix a basic drink         |   Quinn                 |
| | interactive LCD interface | Karen|
| Week 4: 4/15 - 4/21 | begin 3D printing external parts         |  Quinn |
|  | ID scanner or breathalyzer         |  Karen |
| Week 5: 4/22 - 4/26 | finish up + reach goals         |  K + Q                  |

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
