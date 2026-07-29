const int X_PIN = 34;
const int Y_PIN = 35;
const int BUTTON_PIN = 16;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  bool pressed = !digitalRead(BUTTON_PIN);

  Serial.print("X: ");
  Serial.print(x);

  Serial.print(" | Y: ");
  Serial.print(y);

  Serial.print(" | Botão: ");
  Serial.println(pressed ? "Pressionado" : "Solto");

  delay(100);
}