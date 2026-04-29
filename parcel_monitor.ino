// Smart Doorstep Parcel Monitor
// Introduction to IoT - PBL (Adapted from Smart Pet Feeder)
// Original Author: C. Mendes
// Modified for: Parcel Detection System
// Date: April 29th, 2025

/* ───── Library ───── */
#include <NewPing.h> // Handles HC-SR04 timing and distance math

/* ───── PIN DEFINITIONS ───── */
const int triggerPin = 11;
const int echoPin    = 10;
const int ledPin     = 6;
const int buzzerPin  = 5;
const int buttonPin  = 4;    // push-button override (active-LOW)

/* ───── CONSTANTS ───── */
const int maxDistance = 200;  // maximum distance to measure (cm) - increased for doorstep range
const int threshold   = 30;   // parcel considered present if distance < threshold (cm)

/* ───── GLOBAL STATE ───── */
NewPing sonar(triggerPin, echoPin, maxDistance);

bool alertSilenced   = false;   // true → alert muted until parcel is removed
bool prevButtonState = HIGH;    // for edge detection (HIGH = not pressed)

/* ───── FUNCTION DECLARATIONS ───── */
long checkDistance();
void triggerAlert(bool alert);
void handleButton(bool parcelPresent);

/* ───── SETUP ───── */
void setup() {
  pinMode(ledPin,    OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);   // internal pull-up
  Serial.begin(9600);
  
  // Startup indication
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  Serial.println("Smart Doorstep Parcel Monitor Ready");
}

/* ───── MAIN LOOP ───── */
void loop() {
  /* 1. Measure distance */
  long distance = checkDistance();     // in cm

  /* 2. Determine if a parcel is present */
  bool parcelPresent;
  if (distance < threshold && distance != 0) {
    parcelPresent = true;              // parcel detected at doorstep
    Serial.print("PARCEL DETECTED at ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    parcelPresent = false;             // no parcel, doorstep clear
    Serial.println("No parcel detected");
  }

  /* 3. Handle push-button toggle (mutes alert while parcel is present) */
  handleButton(parcelPresent);

  /* 4. Alert logic */
  if (parcelPresent && !alertSilenced) {
    triggerAlert(true);                // buzzer + LED ON
  } else {
    triggerAlert(false);               // buzzer + LED OFF
  }

  /* 5. Auto-re-arm alert once parcel is removed */
  if (!parcelPresent) {
    alertSilenced = false;             // alerts enabled for next parcel delivery
    Serial.println("System re-armed for next parcel");
  }

  delay(200);  // loop delay
}

/* ─────────────────────────────────── */
/* Measure distance (cm) with NewPing  */
long checkDistance() {
  delay(50);                             // sensor settling time
  long dist = sonar.ping_cm();           // 0 if out of range
  if (dist == 0) {
    dist = maxDistance;                  // treat as "no object detected"
  }
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
  return dist;
}

/* Turn LED & buzzer ON or OFF */
void triggerAlert(bool alert) {
  if (alert) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);  // active buzzer sounds
    Serial.println("ALERT: LED + BUZZER ON");
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);   // silence buzzer
  }
}

/* Edge-detect push-button and set alertSilenced */
void handleButton(bool parcelPresent) {
  bool buttonPressed;

  /* Read buttonPin (LOW when pressed) */
  int rawState = digitalRead(buttonPin);
  if (rawState == LOW) {
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }

  /* rising-edge detection: pressed now, was not pressed last loop */
  if (buttonPressed && prevButtonState == HIGH && parcelPresent) {
    alertSilenced = true;               // mute alert until parcel removed
    digitalWrite(buzzerPin, LOW);       // immediate silence
    Serial.println("Alert silenced by button. System will re-arm when parcel is removed.");
  }

  prevButtonState = buttonPressed;      // update state for next loop
}
