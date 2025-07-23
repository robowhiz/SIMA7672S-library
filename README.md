# SIMA7672S Arduino Library

An Arduino library for interfacing with the SIMA7672S series of GNSS/LTE modules.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [API Reference](#api-reference)
- [Notes](#notes)
- [License](#license)

## Introduction
This library provides support for the SIMA7672S GNSS module, enabling GPS/GLONASS/GALILEO/BEIDOU satellite navigation system tracking and data retrieval. The module supports NMEA and proprietary AT commands for configuration and data retrieval.

## Features
- Satellite positioning (GPS, GLONASS, GALILEO, BEIDOU)
- Latitude, longitude, altitude, speed, and course calculation
- Signal strength monitoring
- Date and time tracking
- Configurable update rates
- Error checking and handling

## Installation

1.  **Download**: Download this repository as a `.zip` file from GitHub.
2.  **Install in Arduino IDE**:
    - Open the Arduino IDE.
    - Go to `Sketch` -> `Include Library` -> `Add .ZIP Library...`.
    - Select the downloaded `.zip` file.
3.  **Include**: Add `#include <SIMA7672S.h>` to the top of your sketch.

## API Reference

### SIMA7672S Class
The main class for interacting with the GNSS module.

#### Constructor
```cpp
SIMA7672S(HardwareSerial *serial)
```
- Parameters:
  - `serial`: Pointer to hardware serial port (e.g., &Serial1)

#### Methods

##### Initialization
```cpp
void begin(uint32_t baudrate)
```
- Initializes the GNSS module with specified baud rate.

##### Data Retrieval
```cpp
void getGNSSInfo(bool debug = false)
```
- Retrieves and parses GNSS data.
- Optional `debug` parameter enables verbose output.

##### Configuration
```cpp
void setGNSSRefreshRate(uint8_t rate = 1)
```
- Sets the update interval for GNSS data.
- Default rate is 1 second (rate=1).

##### Helper Functions
```cpp
String sendAT(String command, String response, uint32_t timeout = 1000)
```
- Sends an AT command and waits for a specific response string.
- Returns the response string.

#### Properties
- `location`: GNSSLocation struct containing latitude, longitude, and course
- `satellite`: GNSSSatellite struct containing satellite status and count
- `info`: GNSSInfo struct containing altitude, speed, DOP values, and signal strength
- `dateTime`: GNSSDateTime struct containing date and time information

### Data Structures

#### GNSSLocation
```cpp
struct GNSSLocation {
  float Latitude;
  float Longitude;
  float Course;

  // Getters
  float lat();
  float log(); 
  float course();
};
```

#### GNSSSatellite
```cpp
struct GNSSSatellite {
  uint8_t GPS_SVs;
  uint8_t GLONASS_SVs;
  uint8_t GALILEO_SVs;
  uint8_t BEIDOU_SVs;
  uint8_t Mode;

  // Getters
  uint8_t NumOfSats();
  uint8_t Fix();
};
```

#### GNSSInfo
```cpp
struct GNSSInfo {
  float Altitude;
  float Speed;
  float PDOP;
  float HDOP;
  float VDOP;
  uint8_t SignalStength;

  // Getters
  float altitude();
  float speed(); 
  float pdop();
  float hdop();
  float vdop();
  uint8_t signalStrength();
};
```

#### GNSSDateTime
```cpp
struct GNSSDateTime {
  uint8_t Year;
  uint8_t Month;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;

  // Getters
  uint8_t year();
  uint8_t month(); 
  uint8_t day();
  uint8_t hour();
  uint8_t minute();
  uint8_t second();
};
```

## Notes
- Ensure proper hardware serial configuration (pins, baud rate).
- Module requires continuous power and communication to maintain fix.
- For best results, use external antennas in challenging environments.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
