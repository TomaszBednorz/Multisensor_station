# Multisensor_station

## 1. Overview
### Description
Small home project of the multisensor station. Communication between the user and interface is by serial port. All drivers are written from scratch (without CMSIS and FreeRTOS).

## 2. Information about project
### Functionalities
User can:
 - set the LEDs lighting sequence on board,
 - read data from sensors like: pressure [hPa], temperature [*C], lighting [%],
 - read/set time and date,
 - set alarms,
 - use the potentiometer to change threshold to light up the LED
 - click the button to get log with dat

### Project structure


### Vizualization
![Bez tytułu](https://user-images.githubusercontent.com/62250785/161803830-d4a58c7e-f882-44aa-8177-c391bdedd4d5.png)

Above we can see screens connected in one image. On the image are points described below.
1) Main menu panel. User can choose one of seven options from 0 to 6. User chose option 0 (LED effect).
2) Here we have menu with LEDs effects. User chose invalid option. Error occurred.
3) User chose option 1, then 2, then 3. Data from sensors was measured and printed. After it user chose option 4 (Time&Date).
4) User chose option 0 (Set time). Time has been configured correctly. 
5) Here user chose option 5 and configured alarm.
6) Alarm occurred. User chose option 6.
7) Help information printed.
8) User clicked button and get data log.

## 2. Bill of materials
- STM32F429ZI Nucleo board
- LPS25H - pressure & temperature sensor
- Photoresistor
- Resistors
- Potentiometer
- LED diode
- Tact switch
- USB cable
- Breadboard
- Jumper wires
  
## 3. Software, middleware and extensions
- Visual Studio Code with below extensions
  - Arm assembly
  - C/C++
  - CMake
  - CMake Tools
  - Cortex-Debug
  - LinkerScript
- CMake
- Ninja build system
- CMSIS
- FreeRTOS
- Custom drivers
  
## 4. TODO list
### Peripherals drivers
- [x] GPIO
- [x] SPI
- [x] UART
- [x] ADC
- [x] RTC
### Hardware drivers
- [x] Pressure/temperature sensor LPS25HB 
### API
- [x] Handling the received data
- [x] LEDs effects
- [x] Temperature / Pressure / Lighting
- [x] RTC (data, time, alarm)
- [x] Button intterrupt
