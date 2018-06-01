//Battery2
  //---
  //7.2 V (Much later)

//Battery3
  //9.6 V
  //8.3 V (1.5 hrs)

//Battery4
  //9.68 V
  //8.6 (30 min) (ideal!) (as in, we gucci)

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

//Best: K_P = 0.4, K_P_R = 10.5, K_D = 2, K_D_R = 2
//New Best: K_P = 0.4, K_P_R = 12.5, K_D = 9, K_D_R = 17
double K_P = 0.40;  //for PD
double K_P_R = 12.5;
double K_D = 9;
double K_D_R = 17;
int L_prevError = 0;
int R_prevError = 0;

long L_zero = 0;  //calibration
long F_zero = 0;
long R_zero = 0;

int L_BASE = 215; //base speeds
int R_BASE = 215;

//previously moving output
//int R_prev = R_BASE;
//int L_prev = L_BASE;

//turning corrections
double L_correction = 1.5;
double R_correction = 2.5;

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
  //turn();
  //gottaGoFast();  //hack into the matrix
  int L_READ = analogRead(L_SENSOR);
  int R_READ = analogRead(R_SENSOR);
  
  //Measure error
  int L_error = L_READ - L_zero;
  int R_error = R_READ - R_zero;
  
  //PID control
  int L_output = L_BASE + K_P * L_error + K_D * (L_error - L_prevError);
  int R_output = R_BASE + K_P_R * R_error + K_D_R * (R_error - R_prevError); 

  L_output = constrain(L_output, 100, 255);
  R_output = constrain(R_output, 100, 255);
  
  //checking for emptiness lol
  if (L_READ < 600)
  {
    analogWrite(L_WHEEL, L_output + L_correction * K_D * (L_error - L_prevError));
    analogWrite(R_WHEEL, R_output);
  }
  else if (R_READ < 600)
  {
    analogWrite(L_WHEEL, L_output);
    analogWrite(R_WHEEL, R_output + R_correction * K_D_R * (R_error - R_prevError));
  }
  else
  {
    analogWrite(L_WHEEL, L_output);
    analogWrite(R_WHEEL, R_output);
  }

  L_prevError = L_error;
  R_prevError = R_error;

  //L_prev = L_output;
  //R_prev = R_output;

//  Serial.println(String(L_READ) + " - " + String(L_zero) + " = " + String(L_error));
//  Serial.println(String(K_P * L_error) + " - " + String(K_D * (L_error - L_prevError)) + " = " + String(L_output));
//  Serial.println(String(R_READ) + " - " + String(R_zero) + " = " + String(R_error));
//  Serial.println(String(K_P_R * R_error) + " - " + String(K_D_R * (R_error - R_prevError)) + " = " + String(R_output) + '\n');
  
  if(debug)
  {
    Serial.println("Left proportional: " + String(K_P * L_error));
    Serial.println("Right proportional: " + String(K_P_R * R_error));
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

  if(analogRead(F_SENSOR) > 830)  //only turns if the front is blocked
  { 
    //fast stop
    digitalWrite(L_1, HIGH);  
    digitalWrite(L_2, HIGH);
    digitalWrite(R_1, HIGH);
    digitalWrite(R_2, HIGH);
    
    analogWrite(L_WHEEL, 255);
    analogWrite(R_WHEEL, 255);

    delay(300);

    analogWrite(L_WHEEL, 0);
    analogWrite(R_WHEEL, 0);

    digitalWrite(L_1, LOW);  
    digitalWrite(L_2, HIGH);
    digitalWrite(R_1, HIGH);
    digitalWrite(R_2, LOW);
    
    if(L_READ < 350)
      left();
    else
      right();

    digitalWrite(L_1, LOW);  
    digitalWrite(L_2, HIGH);
    digitalWrite(R_1, HIGH);
    digitalWrite(R_2, LOW);

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

  //delay(295);
  delay(225);

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

  //delay(295);
  delay(225);
  
  digitalWrite(L_1, LOW);
  digitalWrite(L_2, HIGH);

  analogWrite(L_WHEEL, 0);
  analogWrite(R_WHEEL, 0);
}
