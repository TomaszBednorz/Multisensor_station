# Multisensor_station

## 1. Description
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
![image](https://user-images.githubusercontent.com/62250785/161798094-891327e0-31a5-4299-9a72-253b34463a5c.png)


## 3. Bill of materials
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
  
## 4. Software, middleware and extensions
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
  
## 5. TODO list
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
