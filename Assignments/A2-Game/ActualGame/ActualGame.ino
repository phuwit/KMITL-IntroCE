#include "ST7735_TEE.h"
#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1
#define PIN_BUTTON_A 2
#define PIN_BUTTON_B 3
#define PIN_BUTTON_C 4
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
  int speed;
  uint16_t color;
private:
};
int basePlayerSpeed = 10;
int dashPlayerSpeed = 2 * basePlayerSpeed;
Character player;
const int enemySize = 20;
Character enemy[enemySize];
uint16_t colorArray[] = { GREEN, RED, BLUE, BLACK, CYAN, MAGENTA, YELLOW };
int screenOffset = 30;
bool isGameEnd = false;
void setup() {
  //lcd.init(lcd.HORIZONTAL);
  Serial.begin(9600);
  pinMode(PIN_BUTTON_A, INPUT);
  lcd.init(lcd.VERTICAL);

  //setup player
  player.width = lcd.width / 4;
  player.height = lcd.height / 24;
  player.position.x = lcd.width / 2 - player.width / 2;
  player.position.y = lcd.height - player.height;
  player.color = colorArray[0];
  player.speed = basePlayerSpeed;

  //setup enemy
  for (int i = 0; i < enemySize; i++) {
    enemy[i].width = lcd.width / ((rand() % 4) + 6);
    enemy[i].height = enemy[i].width;
    enemy[i].position.x = rand() % (lcd.width - enemy[i].width);
    enemy[i].position.y = -enemy[i].height;
    enemy[i].color = colorArray[1];
    enemy[i].speed = (rand() % 11) + 10;
  }
}

void DrawPlayer() {
  if (analogRead(PIN_ANALOG_X) <= 341) {
    player.position.x -= player.speed;
    if (player.position.x <= 0)
      player.position.x = 0;
  } else if (analogRead(PIN_ANALOG_X) >= 682) {
    player.position.x += player.speed;
    if (player.position.x >= lcd.width - player.width)
      player.position.x = lcd.width - player.width;
  }
  lcd.fillRect(player.position.x, player.position.y, player.width, player.height, player.color);
}

void clearPlayer() {
  lcd.fillRect(player.position.x, player.position.y, player.width, player.height, WHITE);
}

void RandomPlayerColor() {
  if (digitalRead(PIN_BUTTON_A) == LOW) {
    int randomColor = rand() % 7;
    player.color = colorArray[randomColor];
  }
}
void SetPlayerSpeed() {
  if (digitalRead(PIN_BUTTON_B) == LOW)
    player.speed = dashPlayerSpeed;
  else
    player.speed = basePlayerSpeed;
}
void PlayerMechanic() {
  RandomPlayerColor();
  SetPlayerSpeed();
  DrawPlayer();
}

//enemy
void EnemyMechanic() {
  for (int i = 0; i < enemySize; i++) {
    enemy[i].position.y += enemy[i].speed;
    if (enemy[i].position.y >= lcd.height) {
      enemy[i].position.x = rand() % (lcd.width - enemy[i].width);
      enemy[i].position.y = -enemy[i].height;
    }
  }
}

void CheckCollision() {
  for (int i = 0; i < enemySize; i++) {
    bool isCollision = 
    (player.position.y + player.height >=  enemy[i].position.y) &&
    (player.position.y <= enemy[i].position.y + enemy[i].height) &&
    (player.position.x + player.width >= enemy[i].position.x) &&
    (player.position.x <= enemy[i].position.x + enemy[i].width);
    if (isCollision)
      isGameEnd = true;
  }
}

void ResetGame() {
  // Reset all game variables and setup
  isGameEnd = false;
  player.position.x = lcd.width / 2 - player.width / 2;
  player.position.y = lcd.height - player.height;
  player.color = colorArray[0];
  player.speed = basePlayerSpeed;

  for (int i = 0; i < enemySize; i++) {
    enemy[i].width = lcd.width / ((rand() % 4) + 6);
    enemy[i].height = enemy[i].width;
    enemy[i].position.x = rand() % (lcd.width - enemy[i].width);
    enemy[i].position.y = -enemy[i].height;
    enemy[i].color = colorArray[1];
    enemy[i].speed = (rand() % 11) + 10;
  }
}

void loop() {
  if (digitalRead(PIN_BUTTON_C) == LOW) {
    ResetGame();
  }

  // Rest of your game logic
  if (isGameEnd)
    lcd.drawString(lcd.width / 3, 60, "Press C to try again", BLACK, 3);
  else {
    PlayerMechanic();
    EnemyMechanic();
    CheckCollision();
  }

  // Clear buffer
  lcd.fillScreen(WHITE);
}

