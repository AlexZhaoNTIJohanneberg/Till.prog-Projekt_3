//Arduino bibliotek som behövs

#include "Charliplexing.h"
#include "Myfont.h"
#include "Arduino.h"

// skapar variabler

#define VRx A14 // Arduino pin connected to the VRx Pin
#define VRy A15 // Arduino pin connected to the VRy Pin
#define VRb A13 // Arduino pin connected to the SW Pin

int GRIDSIZEX = 14;
int GRIDSIZEY = 9;

int joystickX;
int joystickY;
int joystickB;

int foodX;
int foodY;

int snakeLength;
int snakeX[126];
int snakeY[126];
int dirX;
int dirY;
bool collision;

int gamespeed;

bool alive = true;
bool start = false;

char text[] = "Hold to play\0";
int leng = 0;

void setup() {

  Serial.begin(9600);
  LedSign::Init(GRAYSCALE);

  for (int i = 0; ; i++) { //get the length of the text
    if (text[i] == 0) {
      leng = i;
      break;
    }
  }

}

void setupgame() {

  gamespeed = 100;

  snakeLength = 4;
  snakeX[0] = 0;
  snakeY[0] = random(0, 9);
  dirX = 1;
  dirY = 0;

  foodX = random(0, 14);
  foodY = random(0, 9);
}

void joystickinput() {

  joystickX = analogRead(VRx);
  joystickY = analogRead(VRy);


  if (dirX == 1) {
    joystickY = analogRead(VRy);
    if (joystickY < 300) {
      dirY = 1;
      dirX = 0;
    }
    else if (joystickY > 700) {
      dirY = -1;
      dirX = 0;
    }
  }

  if (dirX == -1) {
    joystickY = analogRead(VRy);
    if (joystickY < 300) {
      dirY = 1;
      dirX = 0;
    }
    else if (joystickY > 700) {
      dirY = -1;
      dirX = 0;
    }
  }

  if (dirY == 1) {
    joystickX = analogRead(VRx);
    if (joystickX < 300) {
      dirX = 1;
      dirY = 0;
    }
    else if (joystickX > 700) {
      dirX = -1;
      dirY = 0;
    }
  }

  if (dirY == -1) {
    joystickX = analogRead(VRx);
    if (joystickX < 300) {
      dirX = 1;
      dirY = 0;
    }
    else if (joystickX > 700) {
      dirX = -1;
      dirY = 0;
    }
  }
}

bool checkcollisionsnake() {
  if (snakeX[0] < 0 || snakeX[0] >= GRIDSIZEX || snakeY[0] < 0 || snakeY[0] >= GRIDSIZEY) {
    return true;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  return false;
}

bool checkcollisionfood() {
  return (snakeX[0] == foodX && snakeY[0] == foodY);
}

void drawfood() {
  LedSign::Set(foodX, foodY, 4);
}

void drawsnake() {
  for (int i = 0; i < snakeLength; i++) {
    LedSign::Set(snakeX[i], snakeY[i], 2);
  }
}

void movesnake() {

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] += dirX;
  snakeY[0] += dirY;


  // Kan användas så att man inte dör om man kör in i 'väggen'. I detta fall
  // används denna del kod för att "snygga" till när man kör in i väggen.
  if (snakeX[0] < 0) {
    snakeX[0] = GRIDSIZEX + 1;
  }
  else if (snakeX[0] >= GRIDSIZEX) {
    snakeX[0] = -1;
  }

  if (snakeY[0] < -1) {
    snakeY[0] = GRIDSIZEY + 1;
  }
  else if (snakeY[0] > GRIDSIZEY) {
    snakeY[0] = -1;
  }

}


void snakegame() {
  joystickinput();
  LedSign::Clear();
  movesnake();

  checkcollisionsnake();
  if (checkcollisionsnake()) {
    alive = false;
    delay(2000);
  }
  if (checkcollisionfood()) {
    snakeLength++;
    foodX = random(0, 14);
    foodY = random(0, 9);
  }

  drawsnake();
  drawfood();

  delay(gamespeed);
}



void loop() {
  setupgame();
  alive = true;

  Myfont::Banner(leng, text);

  if (analogRead(VRb) == 0) {
    start = true;
  }
  else {
    start = false;
  }

  while (alive == true && start == true) {
    snakegame();
  }
  LedSign::Clear();
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = -1;
    snakeY[i] = -1;
  }

}
