#include <SoftwareSerial.h>

#define SPEED_1       5
#define DIR_1         4

SoftwareSerial piSerial(10, 11); // RX, TX

void uniorBegin(const int channels[], int number_of_channels = 1)
{
  piSerial.begin(57600);
  piSerial.write(0xff);
  while (true)
  {
    String s = piSerial.readString();

    if (s == "OK\n")
    {
      String s;
      for (int n = 0; n < number_of_channels; n++)
      {
        if (n != 0) s += ";";
        s += channels[n];
      }
      piSerial.println(s);
      break;
    }
  }
}

bool uniorRead(uint8_t channel_no, float& value)
{
  while (piSerial.available()) piSerial.read();
  piSerial.write(channel_no);
  int readCnt = piSerial.readBytes((byte*)&value, sizeof(float));
  return (readCnt == sizeof(float)) && (value == value);
}

const int cECG1 = 2;

void setup()
{
  Serial.begin(115200);
  const int channels[] = {cECG1};
  uniorBegin(channels, sizeof(channels) / sizeof(channels[0]));
  pinMode(A0, INPUT);
}

float res = 0;

void loop()
{
  float v;
  while (uniorRead(cECG1, v)) {
    Serial.println(v);
    float s = analogRead(A0);
    delay(10);
    float r = v - s / 2;
    if (r > -2 and r < 2) {
      analogWrite(SPEED_1, 0);
    }
    if (r > 2 and r < 250) {
      analogWrite(DIR_1, 0);
      analogWrite(SPEED_1, 85);
    }
    if (r < -2 and r > -250) {
      analogWrite(DIR_1, 1);
      analogWrite(SPEED_1, 85);
    }
    if (r > 250) {
      analogWrite(DIR_1, 0);
      analogWrite(SPEED_1, 175);
    }
    if (r < -250) {
      analogWrite(DIR_1, 1);
      analogWrite(SPEED_1, 175);
    }
    if (res > 15000 or res < -15000) {
      delay(5000);
    }
  }
}
