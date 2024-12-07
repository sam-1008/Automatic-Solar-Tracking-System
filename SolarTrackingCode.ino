#include <Servo.h> 

#define SERVOPINH  5 //horizontal servo
#define SERVOPINV  6 //vertical servo

int tol=100;   // Response range of illuminance
int dtime=100; // Delay parameter for response speed

Servo horizontal;  
int servoh = 90;  
int servohLimitHigh = 180;  
int servohLimitLow = 0;

Servo vertical;  
int servov = 120;  
int servovLimitHigh = 180;  
int servovLimitLow = 90;

const int ldrlt = A0;  // Top left
const int ldrrt = A1;  // Top right
const int ldrld = A2;  // Bottom left
const int ldrrd = A3;  // Bottom right

void setup() {
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  Serial.begin(9600);
}

void loop() {
  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);

  int avt = (lt + rt) / 2; 
  int avd = (ld + rd) / 2; 
  int avl = (lt + ld) / 2; 
  int avr = (rt + rd) / 2;
  int veg=(avt + avd + avl + avr) / 4;

  if(0 < veg && veg < 300) { 
    tol = map(veg, 10, 300, 5, 100);
    dtime = map(veg, 10, 300, 100, 50);
  } else { 
    tol = 50;
    dtime = 50;
  }

  int dvert = avt - avd;
  int dhoriz = avl - avr;

  if (-1 * tol > dvert || dvert > tol) {
    if (avt > avd) {
      servov = ++servov;
      if (servov > servovLimitHigh) servov = servovLimitHigh;
    } else if (avt < avd) {
      servov = --servov;
      if (servov < servovLimitLow) servov = servovLimitLow;
    }
    vertical.write(servov);
  }

  if (-1 * tol > dhoriz || dhoriz > tol) {
    if (avl < avr) {
      servoh = --servoh;
      if (servoh < servohLimitLow) servoh = servohLimitLow;
    } else if (avl > avr) {
      servoh = ++servoh;
      if (servoh > servohLimitHigh) servoh = servohLimitHigh;
    }
    horizontal.write(servoh);
  }

  delay(dtime);
}
