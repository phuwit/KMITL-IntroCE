#include "ST7735_TEE.h"

#define PIN_ANALOG_X 0

#define PIN_ANALOG_Y 1
TEE_ST7735 lcd(9, 10, 11, 12, 13);  //Arduino  csk,sda,A0,rst,cs
struct Vector2 {
  int x;
  int y;
};

class Character {
public:
  Vector2 position;
  int width;
  int height;
private:
};
Character player;

int screenOffset = 30;
void setup() {
  //lcd.init(lcd.HORIZONTAL);
  lcd.init(lcd.VERTICAL);
  player.width = lcd.width / 4;
  player.height = lcd.height / 24;
  player.position.x = lcd.width / 2 - player.width / 2;
  player.position.y = lcd.height - player.height;
}

void drawPlayer() {
  if (analogRead(PIN_ANALOG_X) <= 341) {
    if (player.position.x >= player.width + sc) 
      player.position.x -= lcd.width / 10;
  } else if (analogRead(PIN_ANALOG_X) >= 682) {
    if(player.position.x <= lcd.width - player.width*2)
      player.position.x += lcd.width / 10;
  }
  lcd.fillRect(player.position.x, player.position.y, player.width, player.height, GREEN);
}

void clearPlayer() {
  lcd.fillRect(player.position.x, player.position.y, player.width, player.height, WHITE);
}

void loop() {
  //clear buffer
  drawPlayer();
  clearPlayer();
  //start game logic
}
