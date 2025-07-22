#include <Arduino.h>
#include <SIMA7672S.h>

HardwareSerial Serial1(USART1);

SIMA7672S simA7672S(&Serial2);

void setup()
{
  Serial1.begin(115200);
  simA7672S.begin(115200);
  Serial1.println("GNSS Module Ready!");
}

void loop()
{
  simA7672S.getGNSSInfo();
  if (simA7672S.satellite.Fix() != 0)
  {
    Serial1.println("Lattitude: " + String(simA7672S.location.lat(), 7));
    Serial1.println("Longitude: " + String(simA7672S.location.log(), 7));
    Serial1.println("Course: " + String(simA7672S.location.course(), 2));
    Serial1.println("Number of SATS: " + String(simA7672S.satellite.NumOfSats()));
    Serial1.println("Fix: " + String(simA7672S.satellite.Fix()));
    Serial1.println("Altitude: " + String(simA7672S.info.altitude(), 2));
    Serial1.println("Speed: " + String(simA7672S.info.speed(), 2));
    Serial1.println("Signal Strength: " + String(simA7672S.info.signalStrength()));
    Serial1.println("Date: " + String(simA7672S.dateTime.year()) + "/" + String(simA7672S.dateTime.month()) + "/" + String(simA7672S.dateTime.day()));
    Serial1.println("Time: " + String(simA7672S.dateTime.hour() + 5) + ":" + String(simA7672S.dateTime.minute() + 30) + ":" + String(simA7672S.dateTime.second()));
    Serial1.println();
  }
  else
    Serial1.println("No Fix Available");

  delay(5000);
}