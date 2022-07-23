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
int last_1 = 0;
int last_2 = 0;
float total = 0;
float r;
float v;
float s;
float p1 = 0;
float p2 = 0;
float p1_1;
float p2_1;
float p1_2;
float p2_2;

void setup()
{
  Serial.begin(115200);
  const int channels[] = {cECG1};
  uniorBegin(channels, sizeof(channels) / sizeof(channels[0]));
  pinMode(A0, INPUT);
  uint32_t sec1 = millis() / 1000;
  uint32_t sec;
  while (sec < 60) {
    p1_1 = uniorRead(cECG1, v);
    p2_1 = analogRead(A0);
    delay(3);
    p1_2 = uniorRead(cECG1, v);
    p2_2 = analogRead(A0);
    if (p1_1 < p1_2) {
      last_1 = 1;
    }
    else {
      if (last_1 = 1) {
        p1 += 1;
        last_1 = 0;
      }
    }
    if (p2_1 < p2_2) {
      last_2 = 1;
    }
    else {
      if (last_2 = 1) {
        p2 += 1;
        last_2 = 0;
      }
    }
    sec = millis() / 1000 - sec1;
  }
}
const float p1_base = p1 / 120;
const float p2_base = p2 / 120;

uint32_t mil1 = millis();
uint32_t mil;

float res1;
float res2;
void loop()
{
  while (mil < 100) {
    p1_1 = uniorRead(cECG1, v);
    p2_1 = analogRead(A0);
    delay(3);
    p1_2 = uniorRead(cECG1, v);
    p2_2 = analogRead(A0);
    if (p1_1 < p1_2) {
      last_1 = 1;
    }
    else {
      if (last_1 = 1) {
        p1 += 1;
        last_1 = 0;
      }
    }
    if (p2_1 < p2_2) {
      last_2 = 1;
    }
    else {
      if (last_2 = 1) {
        p2 += 1;
        last_2 = 0;
      }
    }
    mil = millis() - mil1;
  }
  res1 = p1 / 120 - p1_base;
  res2 = p2 / 120 - p2_base;
  r = res1 - res2;
  if (r > -2 and r < 2) {
    analogWrite(SPEED_1, 0);
  }
  if (r > 2 and r < 250) {
    analogWrite(DIR_1, 0);
    analogWrite(SPEED_1, 85);
    total += 85;
  }
  if (r < -2 and r > -250) {
    analogWrite(DIR_1, 1);
    analogWrite(SPEED_1, 85);
    total += -85;
  }
  if (r > 250) {
    analogWrite(DIR_1, 0);
    analogWrite(SPEED_1, 175);
    total += 175;
  }
  if (r < -250) {
    analogWrite(DIR_1, 1);
    analogWrite(SPEED_1, 175);
    total += -175;
  }
  if (total > 5000 or total < -5000) {
    delay(5000);
  }
  mil1 = millis();
  mil = 0;
  p1 = 0;
  p2 = 0;
}
