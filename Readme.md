# XFactor & SafeBox
--------------
## General information
- ### Repository's Contents:
- - This repository contains 2 projects. SafeBox and XFactor. Both of which are Arduino based projects that uses PlatformIO on Visual Studio Code to program and upload code into official Arduino Megas and clones of it. This repository is divided in the 2 projects which folders contains their individual project. They should be openned individually to avoid problems.

## Contributors
- While you can fork and clone this repository freely, only our team is allowed to contribute to this project as it is graded by our university. The following list is the people who contributed to this project in no particular order.
- 1. GITHUB ACCOUNT
- 2. GITHUB ACCOUNT
- 3. GITHUB ACCOUNT
- 4. GITHUB ACCOUNT
- 5. GITHUB ACCOUNT
- 6. GITHUB ACCOUNT
- 7. GITHUB ACCOUNT
- 8. GITHUB ACCOUNT

## FAQ
### 1. What is this project?
- XFactor and SafeBox are final projects made for Sherbrooke's University's end of session 1. We were tasked with making  a robot that is useful to humans and is autonomous. The robot needed to use the ROBUS platform as well as being fully autonomous. During a team meeting, we decided we wanted to make a prototype for a robot that could hide delivered packages from porch theives in an attempt to bring a new silly solution to the delivery market. Countless cases of reckless deliveries and stealings is what brought us to this idea in particular.

### 2. What is XFactor?
- XFactor is the name given to the robot built on ROBUS platform. The name was chosen after multiple meetings which finally led to a vote taking place.

### 3. What is SafeBox?
- Safebox is the name given to the smart box where the robot deposits the packages he finds on your porch. It is also where the robot hides while waiting for a delivery to happen, hence the name "SafeBox".

### 4. How does it work?
- Firstly, the user activates SafeBox so its ready to handle packages and deliveries by using an RFID/NFC card and buttons on the box. The user inputs how many deliveries are expected then locks the box. Once that is done, SafeBox waits for your doorbell to activate. When the doorbell activates, a signal is sent to the robot to retreive a package. The robot executes a search pattern until the package is located. Then it secures the package, calculates and executes its returning path to the box. Once there, a communication between XFactor and SafeBox allows the box to be unlocked for just the right amount of time for the package to be transfered inside SafeBox after which it locks itself. XFactor then returns inside SafeBox.

### 5. What if something bad happens?
- Both SafeBox and XFactor are programmed to detect anomalies and execute anti-thief behaviour if something were to happen. If anything happens to both of them during their use, they will detect it and activate special measures to alert you of a problem.

------
# Disclaimer
SafeBox and XFactor are student projects and are not meant to be used seriously. This project is barely a functionning prototype for the idea of a finished product. The finished product would work completely differently and would also be built completely differently. In no ways shape or form should this project actually be used for its intended purpose in real conditions. 
Please note that ROBUS is the property of "Université de Sherbrooke". This includes, but is not limited to, the physical robot, this program, and anything built on the ROBUS platform.

