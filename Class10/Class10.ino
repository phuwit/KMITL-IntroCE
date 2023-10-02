int enA = 5;  //right
int in1 = 6;
int in2 = 7;
int enB = 10; //left
int in3 = 8;
int in4 = 9;

void setup()
{
  pinMode (enA, OUTPUT);
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (enB, OUTPUT);
  pinMode (in3, OUTPUT);
  pinMode (in4, OUTPUT);
}

void loop()
{
  stop(2000);

  forward(100, 1200);
  
  stop(500);

  turn_left(100, 800);

  stop(500);
}

void stop(int time)
{
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(time);
}

void forward(int speed, int time)
{
  analogWrite(enA, speed - 32);
  analogWrite(enB, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(time);
}

void turn_left(int speed, int time)
{
  analogWrite(enA, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(time);

}