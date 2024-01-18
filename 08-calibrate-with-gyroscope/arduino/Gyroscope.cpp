#include "Gyroscope.h"

MPU6050 gyroscope;

bool Gyroscope::init(void) {
  Wire.begin();
  uint8_t chip_id = 0x00;
  uint8_t cout;

  // make sure the device is online
  do {
    chip_id = gyroscope.getDeviceID();
    Serial.print("MPU6050_chip_id: ");
    Serial.println(chip_id);
    delay(10);
    cout += 1;
    if (cout > 10)
    {
      return true;
    }
  } while (chip_id == 0X00 || chip_id == 0XFF);

  gyroscope.initialize();
  return false;
}

// calculate the average offset after 100 times
// basically it tells us what is the original rotation
bool Gyroscope::calibrate(void) {
    int16_t gz;

    unsigned short times = 100; // sampling times
    for (int i = 0; i < times; i++)
    {
      gz = gyroscope.getRotationZ();
      gzo += gz;
    }
    gzo /= times; // calculate gyroscope offset

    return false;
}

bool Gyroscope::getAngle(float *yaw) {
    int16_t gz;
    unsigned long now = millis();   // current time (ms)
    dt = (now - lastTime) / 1000.0; // differential time (s)
    lastTime = now;                 // last sampling time (ms)
    gz = gyroscope.getRotationZ();
    float gyroz = -(gz - gzo) / 131.0 * dt; // angular velocity in z axis
    if (fabs(gyroz) < 0.05)
    {
      gyroz = 0.00;
    }
    agz += gyroz; // integral of z-axis angular velocity
    *yaw = agz;
    return false;
}