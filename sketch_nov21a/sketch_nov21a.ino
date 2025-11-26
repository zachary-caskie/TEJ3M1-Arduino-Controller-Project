int xPin = A1;
int yPin = A0;
int buttonPin = A2;
int y2Pin = A3;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // assuming you used pull-up
}

void loop() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int y2 = analogRead(y2Pin);
  int button = 0;
  if (digitalRead(buttonPin) == 0)
  {
    button = 1;
  }


  // Format however SFML expects it
  Serial.print(x);
  Serial.print("/");
  Serial.print(y);
  Serial.print("/");
  Serial.print(button);
  Serial.print("/");
  Serial.print(y2);
  Serial.println();
  
  delay(100/6);
}
