# Multisensor_station - in progress

## 1. Description
Small home project of weather station. User can read data from sensors by serial port. All drivers are written from scratch (without CMSIS and FreeRTOS).

## 2. Information about project
### Functionalities
- User can check the following informations from sensors
  - temperature [°C]
  - humidity [%]
  - pressure [hPa]
  - height above sea level [m]
  - height above ground in current location [m]
  - lighting [%]
  - acceleration [m/s^2]
  - rotation speed [°/s]
  - date and time
- User can set the information nedded to some calculations
  - height in user location [m] - by serial port
  - pressure in user location [hPa] - by serial port
  - threshold lighting [%] - by potentiometer 
### Vizualization
- LCD display (switching data by 4x4 tact switch)
<TO DO: photography with 4x4 tact switch description>
- Serial port (by sending appropriate commands described below - 115200 boudrate)
  - help : shows all options
  - t : shows the temperature [°C]
  - hu : shows the humidity [%]
  - p : shows the pressure [hPa]
  - p_xxxx.xx : saves actual pressure in user location (in hPa) <TO DO: link to website>
  - he_asl : shows height above sea level [m]
  - he_ag : shows height above ground in current location [m]
  - he_xxx.xx : saves actual height in user location (in m) <TO DO: link to website>
  - l : shows the lighting [%]
  - a : shows the acceleration [m/s^2]
  - g : shows the rotation speed [°/s]
  - t : shows the time [date and time]
  - all : shows all data from sensors
  - i : shows information about actual threshold to turn on diode (in %) and actual pressure at sea level
  - s : saves the log to the EEPROM memory
  - e : reads the log from the EEPROM memory
  - cont1_en : enables continuous mode 1 Hz - shows measurements every 1 second by serial port
  - cont4_en : enables continuous mode 4 Hz - shows measurements every 250 miliseconds by serial port
  - cont10_en : enables continuous mode 10 Hz - shows measurements every 100 miliseconds by serial port
  - cont_dis : disables continuous mode
- Servo shows actual temperature
- The LED turns on when lighting is below set threshold
  
## 3. Bill of materials
- STM32F429ZI Nucleo board
- LPS25H - pressure & temperature sensor
- Photoresistor
- Resistor
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
- [x] SPI pooling mode
- [x] UART pooling mode
- [x] UART interrupt mode
- [x] ADC
- [x] RTC
### Hardware drivers
- [x] Pressure/temperature sensor LPS25HB 
### API
- [x] Handling the received data
- [x] LEDs effects
- [x] Temperature / Pressure
- [ ] RTC (data, time, alarm)
- [ ] Button intterrupt
### Others
- [ ] Electrical schematics
- [ ] Project description & visualization on GitHub
## 6. Electrical diagram
<TO DO>
