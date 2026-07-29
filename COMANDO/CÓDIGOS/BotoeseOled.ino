#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 22
#define OLED_CS 5
#define OLED_RST 21

#define BTN_ARM     25
#define BTN_MODE    26
#define BTN_DISARM  27

bool armado = false;

int modo = 0; // 0=SAFE 1=NORMAL 2=SPORT

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

  pinMode(BTN_ARM, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_DISARM, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  display.display();
}

void loop() {

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

  delay(20);
}