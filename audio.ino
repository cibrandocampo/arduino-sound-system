/*
 Second life for old audio systems
 ---------------------------------

 Compatible with Sony HCD-H150/H500
 Coded by Cibran Docampo
*/

// Sound imputs (analog)
int ccSoundInputPin = 4; // Chromecast in Phono input
int tvSoundInputPin = 5; // TV in CD input

// Action relays output (digital)
int onRelayPin = 8; // Turn on the sound system
int tvRelayPin = 9; // Switch to TV input (CD)
int ccRelayPin = 10; //

// Info LED output (digital)
int ledPin = 13;

// Configuration variables
int ccThreshold = 2;
int tvThreshold = 4;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(onRelayPin, OUTPUT);
  pinMode(tvRelayPin, OUTPUT);
  pinMode(ccRelayPin, OUTPUT);

  Serial.begin(9600);
  // Flash the LED twice to show the program has started
  Serial.print("Starting... \n");
  digitalWrite(ledPin, HIGH); delay(150); digitalWrite(ledPin, LOW); delay(150);
  digitalWrite(ledPin, HIGH); delay(150); digitalWrite(ledPin, LOW);
}

void loop() {

  if (CheckTvSignalLevel()){
    Serial.print("Turning on for TV \n");
    TurnOnSystem();
    Switch2Tv();
    TurnOffWhenTvNotLevel();
  }else{
    if (CheckCcSignalLevel()){
      Serial.print("Turning on for Chromecast \n");
      TurnOnSystem();
      Switch2Cc();
      TurnOffWhenCcNotLevel();
    }
  }
}

bool CheckCcSignalLevel(){
  if (analogRead(ccSoundInputPin) >= ccThreshold){
    return ConfirmCcSignalLevel();
  }
  return false;
}

bool CheckTvSignalLevel(){
  if (analogRead(tvSoundInputPin) >= tvThreshold){
    return ConfirmTvSignalLevel();
  }
  return false;
}

bool ConfirmCcSignalLevel(){
  int count = 3000;
  while (count > 0 ) {
    delay(10);
    if (analogRead(ccSoundInputPin) >= ccThreshold){
      return true;
    }
    count = count - 1;
  }
  Serial.print("Not level detected for Chromecast \n");
  return false;
}

bool ConfirmTvSignalLevel(){
  int count = 2000;
  while (count > 0 ) {
    delay(10);
    if (analogRead(tvSoundInputPin) >= tvThreshold){
      return true;
    }
    count = count - 1;
  }

  Serial.print("Not level detected for TV \n");
  return false;
}

void TurnOnSystem(){
  digitalWrite(onRelayPin, HIGH); delay(500); digitalWrite(onRelayPin, LOW);
};

void TurnOffSystem(){
  Serial.print("Turning Off system... \n");
  digitalWrite(onRelayPin, HIGH); delay(500); digitalWrite(onRelayPin, LOW);
};

void Switch2Cc(){
  digitalWrite(ccRelayPin, HIGH); delay(500); digitalWrite(ccRelayPin, LOW);
}

void Switch2Tv(){
  digitalWrite(tvRelayPin, HIGH); delay(500); digitalWrite(tvRelayPin, LOW);
}

void TurnOffWhenCcNotLevel(){
  while (true){if (ConfirmCcSignalLevel() == false){break;}}
  TurnOffSystem();
}

void TurnOffWhenTvNotLevel(){
  while (true){if (ConfirmTvSignalLevel() == false){break;}}
  TurnOffSystem();
}
