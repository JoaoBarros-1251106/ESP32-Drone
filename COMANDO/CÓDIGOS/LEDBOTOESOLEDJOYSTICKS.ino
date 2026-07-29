#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED
#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 22
#define OLED_CS 5
#define OLED_RST 21

// Botões
#define BTN_ARM     25
#define BTN_MODE    26
#define BTN_DISARM  27

// LED RGB
#define LED_R 32
#define LED_G 33
#define LED_B 4

// Joystick Esquerdo
#define JOY_LX   34
#define JOY_LY   35
#define JOY_LSW  16

// Joystick Direito
#define JOY_RX   36   // VP
#define JOY_RY   39   // VN
#define JOY_RSW  17

bool armado = false;
int modo = 0;   // 0=SAFE 1=NORMAL 2=SPORT

bool lastMode = HIGH;
bool lastArm = HIGH;
bool lastDisarm = HIGH;

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  OLED_MOSI,
  OLED_CLK,
  OLED_DC,
  OLED_RST,
  OLED_CS
);

void setup() {

  Serial.begin(115200);

  pinMode(BTN_ARM, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_DISARM, INPUT_PULLUP);

  pinMode(JOY_LSW, INPUT_PULLUP);
  pinMode(JOY_RSW, INPUT_PULLUP);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  display.display();
}

void loop() {

  // =========================
  // Leitura dos botões
  // =========================

  bool armBtn = digitalRead(BTN_ARM);
  bool modeBtn = digitalRead(BTN_MODE);
  bool disarmBtn = digitalRead(BTN_DISARM);

  if (lastArm == HIGH && armBtn == LOW) {
    armado = true;
  }

  if (lastDisarm == HIGH && disarmBtn == LOW) {
    armado = false;
  }

  if (lastMode == HIGH && modeBtn == LOW) {

    modo++;

    if (modo > 2) {
      modo = 0;
    }
  }

  lastArm = armBtn;
  lastMode = modeBtn;
  lastDisarm = disarmBtn;

  // =========================
  // LED RGB
  // =========================

  if (!armado) {

    // Vermelho
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);

  } else {

    switch (modo) {

      case 0: // SAFE
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_B, LOW);
        break;

      case 1: // NORMAL
        digitalWrite(LED_R, HIGH);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_B, LOW);
        break;

      case 2: // SPORT
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, HIGH);
        break;
    }
  }

  // =========================
  // Joysticks
  // =========================

  int joyLX = analogRead(JOY_LX);
  int joyLY = analogRead(JOY_LY);

  int joyRX = analogRead(JOY_RX);
  int joyRY = analogRead(JOY_RY);

  bool joyLSW = !digitalRead(JOY_LSW);
  bool joyRSW = !digitalRead(JOY_RSW);

  // =========================
  // Serial Monitor
  // =========================

  Serial.print("LX:");
  Serial.print(joyLX);

  Serial.print("  LY:");
  Serial.print(joyLY);

  Serial.print("  RX:");
  Serial.print(joyRX);

  Serial.print("  RY:");
  Serial.print(joyRY);

  Serial.print("  LSW:");
  Serial.print(joyLSW ? "ON" : "OFF");

  Serial.print("  RSW:");
  Serial.print(joyRSW ? "ON" : "OFF");

  Serial.print("  ARM:");
  Serial.print(armado ? "ON" : "OFF");

  Serial.print("  MODE:");

  switch (modo) {

    case 0:
      Serial.println("SAFE");
      break;

    case 1:
      Serial.println("NORMAL");
      break;

    case 2:
      Serial.println("SPORT");
      break;
  }

  // =========================
  // OLED
  // =========================

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("CONTROLO DRONE");

  display.setCursor(0, 20);
  display.print("ARM: ");
  display.println(armado ? "ON" : "OFF");

  display.setCursor(0, 35);
  display.print("MODE: ");

  switch (modo) {

    case 0:
      display.println("SAFE");
      break;

    case 1:
      display.println("NORMAL");
      break;

    case 2:
      display.println("SPORT");
      break;
  }

  display.display();

  delay(50);
}