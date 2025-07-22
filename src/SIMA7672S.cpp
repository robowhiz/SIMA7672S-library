#include <SIMA7672S.h>

SIMA7672S::SIMA7672S(HardwareSerial *serial)
{
  _serial = serial;
}

void SIMA7672S::begin(uint32_t baudrate)
{
  _baudrate = baudrate;
  _serial->begin(baudrate);
  _serial->flush();
  sendAT("AT+CGNSSINFO=0", "OK");
  _serial->flush();
  sendAT("AT+CGNSSPWR=1", "READY!", 10000);
}

void SIMA7672S::sendAT(String command)
{
  _serial->print(command);
  _serial->print("\r");
}

String SIMA7672S::sendAT(String command, String response, uint32_t timeout)
{
  _serial->print(command);
  _serial->print("\r");

  return ReadSerial(response, timeout);
}

String SIMA7672S::ReadSerial(String response, uint32_t timeout)
{
  String temp = "";
  uint32_t time = millis();
  bool found = false;

  while ((time + timeout) > millis() && !found)
  {
    while ((_serial->available()) && !found)
    {
      temp += (char)_serial->read();

      if (temp.indexOf(response) != -1)
      {
        found = true;
      }
    }
  }
  return temp;
}

void SIMA7672S::parseGNSSInfo(String response)
{
  int startIndex = response.indexOf(":") + 1;
  response = response.substring(startIndex);
  response.trim();
  String data[18];
  int index = 0;
  if (response[0] == '2' || response[0] == '3')
  {
    while (response.length() > 0)
    {
      int commaIndex = response.indexOf(",");
      if (commaIndex == -1)
      {
        data[index++] = response;
        break;
      }
      else
      {
        data[index++] = response.substring(0, commaIndex);
        response = response.substring(commaIndex + 1);
      }
    }
  }

  satellite.Mode = data[0].toInt();
  satellite.GPS_SVs = data[1].toInt();
  satellite.GLONASS_SVs = data[2].toInt();
  satellite.BEIDOU_SVs = data[3].toInt();
  satellite.GALILEO_SVs = data[4].toInt();

  location.Latitude = (data[6] == "N") ? data[5].toFloat() : -data[5].toFloat();
  location.Longitude = (data[8] == "E") ? data[7].toFloat() : -data[7].toFloat();
  location.Course = data[13].toFloat();

  dateTime.Day = data[9].substring(0, 2).toInt();
  dateTime.Month = data[9].substring(2, 4).toInt();
  dateTime.Year = data[9].substring(4, 6).toInt();
  dateTime.Hour = data[10].substring(0, 2).toInt();
  dateTime.Minute = data[10].substring(2, 4).toInt();
  dateTime.Second = data[10].substring(4, 6).toInt();

  info.Altitude = data[11].toFloat();
  info.Speed = data[12].toFloat();
  info.PDOP = data[14].toFloat();
  info.HDOP = data[15].toFloat();
  info.VDOP = data[16].toFloat();
  info.SignalStength = data[17].toInt();
}

void SIMA7672S::getGNSSInfo(bool debug)
{
  String response = sendAT("AT+CGNSSINFO", "OK");
  if (debug)
    Serial1.println(response);
  parseGNSSInfo(response);
}

void SIMA7672S::setGNSSRefreshRate(uint8_t rate)
{
  _serial->flush();
  sendAT("AT+CGNSSINFO=" + String((int)rate), "OK");
  _serial->flush();
  sendAT("AT+CGNSSPORTSWITCH=1,1", "OK");
  _serial->flush();
}

void SIMA7672S::encode(bool debug)
{
  char c;
  while (_serial->available())
  {
    c = (char)_serial->read();
    _response += c;
    if (debug)
      Serial1.print(c);
    if (_response.indexOf('\n') != -1)
    {
      while (_response.indexOf('O') != -1)
      {
        _response = _response.substring(_response.indexOf('O') + 1, _response.indexOf('O') + _response.length());
      }
      parseGNSSInfo(_response);
      _response = "";
      _serial->flush();
      break;
    }
  }
}

// double SIMA7672S::distanceBetween(double lat1, double long1, double lat2, double long2)
// {
//   // returns distance in meters between two positions, both specified
//   // as signed decimal-degrees latitude and longitude. Uses great-circle
//   // distance computation for hypothetical sphere of radius 6371009 meters.
//   // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
//   // Courtesy of Maarten Lamers
//   double delta = radians(long1-long2);
//   double sdlong = sin(delta);
//   double cdlong = cos(delta);
//   lat1 = radians(lat1);
//   lat2 = radians(lat2);
//   double slat1 = sin(lat1);
//   double clat1 = cos(lat1);
//   double slat2 = sin(lat2);
//   double clat2 = cos(lat2);
//   delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
//   delta = sq(delta);
//   delta += sq(clat2 * sdlong);
//   delta = sqrt(delta);
//   double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
//   delta = atan2(delta, denom);
//   return delta * _GPS_EARTH_MEAN_RADIUS;
// }

// double SIMA7672S::courseTo(double lat1, double long1, double lat2, double long2)
// {
//   // returns course in degrees (North=0, West=270) from position 1 to position 2,
//   // both specified as signed decimal-degrees latitude and longitude.
//   // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
//   // Courtesy of Maarten Lamers
//   double dlon = radians(long2-long1);
//   lat1 = radians(lat1);
//   lat2 = radians(lat2);
//   double a1 = sin(dlon) * cos(lat2);
//   double a2 = sin(lat1) * cos(lat2) * cos(dlon);
//   a2 = cos(lat1) * sin(lat2) - a2;
//   a2 = atan2(a1, a2);
//   if (a2 < 0.0)
//   {
//     a2 += TWO_PI;
//   }
//   return degrees(a2);
// }