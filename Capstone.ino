//IR Sensors
#define L_SENSOR 19
#define F_SENSOR 17
#define R_SENSOR 18

//H-Bridge Pins
#define L_1 9
#define L_2 10
#define R_1 7
#define R_2 8

//Wheels
#define R_WHEEL 5
#define L_WHEEL 6

double K_P = 1.5;  //for PD
double K_P_R = 5;
double K_D = 0.60;
int L_prevError = 0;
int R_prevError = 0;

double L_zero = 0;  //calibration
double F_zero = 0;
double R_zero = 0;

int L_BASE = 90; //base speeds
int R_BASE = 90;

const bool debug = false;

void setup() 
{
  Serial.begin(9600);

  //allow for user to place the car before calibration
  delay(3000);

  //average of the values
  for (int i = 0; i < 100; i++) 
  {
    L_zero += analogRead(L_SENSOR);
    R_zero += analogRead(R_SENSOR);
  }
  
  L_zero /= 100;
  R_zero /= 100;

  //Set H-Bridge pins to go straight
  digitalWrite(L_1, LOW);  
  digitalWrite(L_2, HIGH);
  digitalWrite(R_1, HIGH);
  digitalWrite(R_2, LOW);
}

void loop() 
{  
  //gottaGoFast();  //hack into the matrix
  int L_READ = analogRead(L_SENSOR);
  int R_READ = analogRead(R_SENSOR);
  
  //Measure error
  int L_error = L_READ - L_zero;
  int R_error = R_READ - R_zero;

  //PID control
  int L_output = L_BASE + K_P * L_error - K_D * (L_error - L_prevError);
  int R_output = R_BASE + K_P_R * R_error - K_D * (R_error - R_prevError); 

  L_output = constrain(L_output, 100, 255);
  R_output = constrain(R_output, 100, 255);

  L_prevError = L_error;
  R_prevError = R_error;

  if(debug)
  {
    Serial.println("Left proportional: " + String(K_P * L_error));
    Serial.println("Right proportional: " + String(K_P * R_error));
    Serial.println("=========================");
    Serial.println("Left derivative: " + String(K_D * (L_error - L_prevError)));
    Serial.println("Right derivative: " + String(K_D * (R_error - R_prevError)));
    Serial.println("=========================");
    Serial.println("Left output: " + String(L_output));
    Serial.println("Right output: " + String(R_output)); 
    Serial.println("Left read: " + String(L_READ));
    Serial.println("Right read: " + String(R_READ));
    Serial.println("*************************");
  }
  
  analogWrite(L_WHEEL, L_output);
  analogWrite(R_WHEEL, R_output);

  if(analogRead(F_SENSOR) > 900)  //only turns if the front is blocked
  { 
    analogWrite(L_WHEEL, 0);
    analogWrite(R_WHEEL, 0);

    delay(300);
    
    if(analogRead(R_SENSOR) > 200)
      left();
    else if(analogRead(L_SENSOR) > 200)
      right();

    delay(300);
  }
}

void turn()
{
  while (true)
  {
    right();
    delay(1000);
    left();
    delay(1000);
  }
}

void gottaGoFast()  //tests the max speed of the car 
{
  while (true)
  {
    analogWrite(L_WHEEL, 250);
    analogWrite(R_WHEEL, 250);
  }
}

void left() //90 degree turn
{
  digitalWrite(R_1, LOW);
  digitalWrite(R_2, HIGH);
  
  analogWrite(L_WHEEL, 150);
  analogWrite(R_WHEEL, 150);

  delay(295);

  digitalWrite(R_1, HIGH);
  digitalWrite(R_2, LOW);

  analogWrite(L_WHEEL, 0);
  analogWrite(R_WHEEL, 0);
}

void right()  //90 degree turn
{
  digitalWrite(L_1, HIGH);
  digitalWrite(L_2, LOW);
  
  analogWrite(L_WHEEL, 150);
  analogWrite(R_WHEEL, 150);

  delay(295);
  
  digitalWrite(L_1, LOW);
  digitalWrite(L_2, HIGH);

  analogWrite(L_WHEEL, 0);
  analogWrite(R_WHEEL, 0);
}
