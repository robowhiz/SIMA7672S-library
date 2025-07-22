#ifndef SIMA7672S_H
#define SIMA7672S_H

#include <Arduino.h>

#define _GPS_EARTH_MEAN_RADIUS 6371009 // old: 6372795

struct GNSSLocation
{
  friend class SIMA7672S;

private:
  float Latitude;
  float Longitude;
  float Course;

public:
  float lat() { return Latitude; };
  float log() { return Longitude; };
  float course() { return Course; };
};

struct GNSSSatellite
{
  friend class SIMA7672S;

private:
  uint8_t GPS_SVs;
  uint8_t GLONASS_SVs;
  uint8_t GALILEO_SVs;
  uint8_t BEIDOU_SVs;
  uint8_t Mode;

public:
  uint8_t NumOfSats() { return GPS_SVs + GLONASS_SVs + GALILEO_SVs + BEIDOU_SVs; };
  uint8_t Fix() { return Mode; };
};

struct GNSSInfo
{
  friend class SIMA7672S;

private:
  float Altitude;
  float Speed;
  float PDOP;
  float HDOP;
  float VDOP;
  uint8_t SignalStength;

public:
  float altitude() { return Altitude; };
  float speed() { return Speed; };
  float pdop() { return PDOP; };
  float hdop() { return HDOP; };
  float vdop() { return VDOP; };
  uint8_t signalStrength() { return SignalStength; };
};

struct GNSSDateTime
{
  friend class SIMA7672S;

private:
  uint8_t Year;
  uint8_t Month;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;

public:
  uint8_t year() { return Year; };
  uint8_t month() { return Month; };
  uint8_t day() { return Day; };
  uint8_t hour() { return Hour; };
  uint8_t minute() { return Minute; };
  uint8_t second() { return Second; };
};

class SIMA7672S
{
public:
  SIMA7672S(HardwareSerial *serial);

  GNSSLocation location;
  GNSSSatellite satellite;
  GNSSInfo info;
  GNSSDateTime dateTime;

  void begin(uint32_t baudrate);
  void sendAT(String command);
  String sendAT(String command, String response, uint32_t timeout = 1000);
  String ReadSerial(String response, uint32_t timeout);
  void parseGNSSInfo(String response);
  void getGNSSInfo(bool debug = false);

  // Note: while using encode function make sure there is no delay in that function,
  // if there is a dely serial buffer will overload and programm crashes
  // use stopReading() and derialFlush() and startReading() to avoid crashing
  void encode(bool debug = false);
  
  void setGNSSRefreshRate(uint8_t rate = 1);
  void serialFlush() { _serial->flush(); }
  void stopReading() { _serial->end(); }
  void startReading() { _serial->begin(_baudrate); }

  // static double distanceBetween(double lat1, double long1, double lat2, double long2);
  // static double courseTo(double lat1, double long1, double lat2, double long2);

private:
  HardwareSerial *_serial;
  uint32_t _baudrate;
  String _response;
  uint32_t _timeout = 1000;
};

#endif
