#define L_SENSOR 4
#define F_SENSOR 3
#define R_SENSOR 5
#define L_1 9
#define L_2 10
#define R_1 7
#define R_2 8
#define L_WHEEL 5
#define R_WHEEL 6

double K_P = 0.4;  //for PID
double K_D = 0.25;
int L_prevError = 0;
int R_prevError = 0;

double L_zero = 0;  //calibration
double F_zero = 0;
double R_zero = 0;

int L_BASE = 90; //base speeds
int R_BASE = 90;

void setup() {
  Serial.begin(9600);
  //delay(3000);
  for (int i = 0; i < 100; i++)
  {
    L_zero += analogRead(L_SENSOR);
    R_zero += analogRead(R_SENSOR);
  }
  L_zero /= 100;
  R_zero /= 100;
  digitalWrite(L_1, LOW);  //these make it go straight
  digitalWrite(L_2, HIGH);
  digitalWrite(R_1, HIGH);
  digitalWrite(R_2, LOW);
}

void loop() {

  while(true)
  {
    right();
    delay(1000);
    left();
    delay(1000);
  }
    
//  analogWrite(L_WHEEL, L_BASE);
//  analogWrite(R_WHEEL, R_BASE);
  int L_READ = analogRead(L_SENSOR);
  int R_READ = analogRead(R_SENSOR);
  //PID control
  int L_error = L_READ - L_zero;
  int R_error = R_READ - R_zero;
  int L_output = L_BASE + K_P * L_error - K_D * (L_error - L_prevError);
  int R_output = R_BASE + K_P * R_error - K_D * (R_error - R_prevError); 

  L_output = constrain(L_output, 65, 255);
  R_output = constrain(R_output, 65, 255);
//
//  Serial.println("Left proportional: " + String(K_P * L_error));
//  Serial.println("Right proportional: " + String(K_P * R_error));
//  Serial.println("=========================");
//  Serial.println("Left derivative: " + String(K_D * (L_error - L_prevError)));
//  Serial.println("Right derivative: " + String(K_D * (R_error - R_prevError)));
//  Serial.println("=========================");
//  Serial.println("Left output: " + String(L_output));
//  Serial.println("Right output: " + String(R_output)); 
//  Serial.println("*************************");
//  
  analogWrite(L_WHEEL, L_output);
  analogWrite(R_WHEEL, R_output);

  if(analogRead(F_SENSOR) > 500)
  { 
    analogWrite(L_WHEEL, 0);
    analogWrite(R_WHEEL, 0);
  }
}

void right()
{
  digitalWrite(R_1, LOW);
  digitalWrite(R_2, HIGH);
  
  analogWrite(L_WHEEL, 150);
  analogWrite(R_WHEEL, 150);

  delay(395);

  digitalWrite(R_1, HIGH);
  digitalWrite(R_2, LOW);

  analogWrite(L_WHEEL, 0);
  analogWrite(R_WHEEL, 0);
}

void left()
{
  digitalWrite(L_1, HIGH);
  digitalWrite(L_2, LOW);
  
  analogWrite(L_WHEEL, 150);
  analogWrite(R_WHEEL, 150);

  delay(405);
  
  digitalWrite(L_1, LOW);
  digitalWrite(L_2, HIGH);

  analogWrite(L_WHEEL, 0);
  analogWrite(R_WHEEL, 0);
}

