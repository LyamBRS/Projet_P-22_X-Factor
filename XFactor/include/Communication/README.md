# Communications
----------------
## Content:
The content of the Communication folder aims to be header files that contains the definitions of all the communication related functions from high to low level. You can see the relations between the different functions present in this file through the following DrawIO https://drive.google.com/file/d/1UHP3Oy37UZu8lzrTNo4j-eE18wlPrqtI/view?usp=sharing
### Files:
- **Bluetooth.hpp**
- - Low level functions that directly interfaces UART protocol with the Bluetooth module of XFactor.
- **SafeBox.hpp**
- - High level functions that allows XFactor to quickly exchange or get data from SafeBox as well as perform commands and checkups on it. This file is divided in 2 regions, one for commands and one for getter functions. Getter functions should be used in command functions. All the protocol used is defined in DEFINES.