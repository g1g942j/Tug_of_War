#define SPEED_1       5
#define DIR_1         4

void setup() {
  // put your setup code here, to run once:
pinMode(5, OUTPUT);
pinMode(4, OUTPUT);
Serial.begin(9600);
}

float total = 0;
void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(DIR_1, LOW);
analogWrite(SPEED_1, 46);
total += 45;
Serial.println(total);
delay(10);
}
