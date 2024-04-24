#define BUTTON_PIN 4

int LDRvalue = 0;          // initiate photoresistor value
int motorValue = 0;
const int readings = 20; 
int ambientLight = 0;
int total = 0;
int average = 0;
static float value = 0;
bool state = false; // true = on, false = off
bool runEnd = false; // check to see if code already ran
int time = 0;
int threshold = 0;
int timeDelay = 100;

void setup() {
	Serial.begin(9600);  // data transmission rate
 	pinMode(A0, INPUT);  // reads input from photoresistor
  pinMode(11, OUTPUT); // controls the LED light
  pinMode(7, OUTPUT);  // output for positive rotation of motor
  pinMode(BUTTON_PIN, INPUT_PULLUP); // reads input from button
  digitalWrite(11, HIGH); // turns on LED
}

void loop() {
  // button stuff
  int buttonMode = digitalRead(BUTTON_PIN);
  if (buttonMode == 0) {
    buttonPress();
  }

  // read value from LDR
  float raw = analogRead(A0);
  //value = ((value*15)+raw)/16; // gets rid of noise (maybe)
  average = raw;
  //digitalWrite(11, HIGH);

  // print reading
  Serial.println(average);
  delay(timeDelay);

  if (!state) { // if off state
    if (average < ambientLight - 50) {
      Serial.println("START");
      time = 0;
      threshold = average + 100; // sets threshold to stop motor
      state = true;
      timeDelay = 100;
      digitalWrite(7,HIGH); // turns on motor

      Serial.print("THRESHOLD: ");
      Serial.println(threshold);
    }
  } else { // if on state
    if (average >= threshold) {       
    	//digitalWrite(11, HIGH);  
    	digitalWrite(7,LOW);       // stop motor
      Serial.println("STOP");
      // Serial.println(time);
      state = false;
      timeDelay = 1000;
    }
    else {
      time = time + 100;
      //digitalWrite(11, HIGH);
    	digitalWrite(7,HIGH);  
    }
  }
  
}

void buttonPress() {
  // procedure to rest ambient light readings
  // code executes when button is pressed
  // takes resetTime seconds

  int resetTime = 5;
  Serial.println("RESETTING...");
  int lightReadings = 0;
  for (int i = 1; i <= resetTime; i++) {
    float raw = analogRead(A0);
    lightReadings = lightReadings + raw;
    delay(1000);
    Serial.println(i);
  }

  // calculates average ambient light reading
  ambientLight = lightReadings / resetTime;

  // resets code
  state = false;
  runEnd = false;
  timeDelay = 500;

  Serial.println("RESET!");
  Serial.print("NEW AMBIENT: ");
  Serial.println(ambientLight);
  delay(1000);
}