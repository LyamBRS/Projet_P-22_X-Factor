# Movements
-----------
## Content:
The content of the Movements folder aims to be header files that contains the definitions of all the movement related functions from high to low level. You can see the relations between the different functions present in this file through the following DrawIO https://drive.google.com/file/d/1UHP3Oy37UZu8lzrTNo4j-eE18wlPrqtI/view?usp=sharing
### Files:
- **Motors.hpp**
- - Low level functions to directly interface with ROBUS library and convertions.
- **Movements.hpp**
- - High level functions that allows the program to tell the robot it needs to start moving to a wanted position.
- **PID.hpp**
- - File containing the definition of PID specific functions and nothing else.
- **Positions.hpp**
- - File containing functions that are used to update he robot's relative positions for vectors
- **Vectors.hpp**
- - File containing all the definitons of functions that manipulates and saves the robot's positions using vectors.