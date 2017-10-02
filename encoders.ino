// This code connects four rotary encoders to interrupt service routines
// to calculate their angular velocities.

// The motor + encoder being used:
// https://www.servocity.com/118-rpm-hd-premium-planetary-gear-motor-w-encoder

// Encoder pin definitions
const byte encPins[4] = {1, 0, 2, 3};

// Encoder counters to keep track of number of pulses
volatile int encCnts[4] = {0, 0, 0, 0};

#define LOOP_HERTZ 10                 // Loop rate
float dt = 1.0/((float) LOOP_HERTZ);  // Time step

int gear_ratio = 71;         // Gear ratio of motor 
int ppr = gear_ratio*12;     // Pulses per revolution
float coeff = 2*PI/(dt*ppr); // Converts pulse count to ang vel

void setup() 
{
  Serial.begin(9600);
  
  // Set encoder pin modes
  for (int i=0; i<4; i++) {
    pinMode(encPins[i], INPUT_PULLUP);
  }
  // Attach encoder pins to ISRs
  attachInterrupt(digitalPinToInterrupt(encPins[0]), enc0, RISING);
  attachInterrupt(digitalPinToInterrupt(encPins[1]), enc1, RISING);
  attachInterrupt(digitalPinToInterrupt(encPins[2]), enc2, RISING);
  attachInterrupt(digitalPinToInterrupt(encPins[3]), enc3, RISING);
}

// Interrupt service routines for each encoder
void enc0() {
  encCnts[0]++;
}
void enc1() {
  encCnts[1]++;
}
void enc2() {
  encCnts[2]++;
}
void enc3() {
  encCnts[3]++;
}

void loop() 
{  
  // Initialise angular velocities
  float angVels[4] = {0, 0, 0, 0};

  // Calculate angular velocities and reset encoder counters
  for (int i=0; i<4; i++) {
    angVels[i] = coeff*encCnts[i];
    Serial.print("Encoder " + String(i) + " has RPM: " + String(60*angVels[i]/(2*PI)) + "\n"); 
    encCnts[i] = 0; 
  }

  Serial.print("\n");
   
  delay(1000*dt);
}
