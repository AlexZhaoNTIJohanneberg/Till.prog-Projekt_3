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
  LedSign::Init(GRAYSCALE);      // startar shield-LOL(LED skärmen)

  for (int i = 0; ; i++) {
    if (text[i] == 0) {
      leng = i;
      break;
    }
  }

}

void setupgame() {   //Uppstartning för spelet(genererar values)

  gamespeed = 100;  //Hur snabbt spelet går(bl.a ormens hastighet)

  alive = true;
  snakeLength = 4;                 //Längden på ormen vid start
  snakeX[0] = 0;                   //Startpunkten för ormen hos x-axeln
  snakeY[0] = random(0, 9);        //Startpunkten för ormen hos y-axeln
  dirX = 1;                        //Startriktning för ormen hos x-axeln
  dirY = 0;                        //Startriktning för ormen hos y-axeln

  foodX = random(0, 14);           //Genererar en slumpmässig x-koordinat för maten
  foodY = random(0, 9);            //Genererar en slumpmässig y-koordinat för maten
}

void joystickinput() {  //Funktion som behandlar joysticken

  joystickX = analogRead(VRx);   //Läser av pinout 'VRx' på joysticken som är den "vågräta" axeln och anger ett värde till joystickX
  joystickY = analogRead(VRy);   //Läser av pinout 'VRy' på joysticken som är den "lodräta" axeln och anger ett värde till joystickY


  if (dirX == 1) {                
    joystickY = analogRead(VRy);  //Läser av pinout 'VRy' på joysticken som är den "lodräta" axeln och anger ett värde till joystickY
    if (joystickY < 300) {        //If-sats som bestämmer riktning på y-axeln och sätter dirX till noll
      dirY = 1;
      dirX = 0;
    }
    else if (joystickY > 700) {
      dirY = -1;
      dirX = 0;
    }
  }

  if (dirX == -1) {               
    joystickY = analogRead(VRy);  //Läser av pinout 'VRy' på joysticken som är den "lodräta" axeln och anger ett värde till joystickY
    if (joystickY < 300) {        //If-sats som bestämmer riktning på y-axeln och sätter dirX till noll
      dirY = 1;
      dirX = 0;
    }
    else if (joystickY > 700) {
      dirY = -1;
      dirX = 0;
    }
  }

  if (dirY == 1) {                
    joystickX = analogRead(VRx);  //Läser av pinout 'VRx' på joysticken som är den "vågräta" axeln och anger ett värde till joystickX
    if (joystickX < 300) {        //If-sats som bestämmer riktning på x-axeln och sätter dirY till noll
      dirX = 1;
      dirY = 0;
    }
    else if (joystickX > 700) {
      dirX = -1;
      dirY = 0;
    }
  }

  if (dirY == -1) {               
    joystickX = analogRead(VRx);  //Läser av pinout 'VRx' på joysticken som är den "vågräta" axeln och anger ett värde till joystickX
    if (joystickX < 300) {        //If-sats som bestämmer riktning på x-axeln och sätter dirY till noll
      dirX = 1;
      dirY = 0;
    }
    else if (joystickX > 700) {
      dirX = -1;
      dirY = 0;
    }
  }
}

bool checkcollisionsnake() {      //Funktion som returnerar en boolean. Funktionen kollar om ormen har krockat in i en vägg eller sig själv.
  if (snakeX[0] < 0 || snakeX[0] >= GRIDSIZEX || snakeY[0] < 0 || snakeY[0] >= GRIDSIZEY) {    //Krocka in i en vägg
    return true;
  }

  for (int i = 1; i < snakeLength; i++) {         //Krocka in i sig själv
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  return false;
}

bool checkcollisionfood() {      //Funktion som returnerar en boolean. Funktionen kollar om ormen har krockat in i maten.
  return (snakeX[0] == foodX && snakeY[0] == foodY);
}

void drawfood() {     //Funktion som lyser upp LED-dioden där maten befinner sig
  LedSign::Set(foodX, foodY, 4);
}

void drawsnake() {     //Funktion som lyser upp ormen.
  for (int i = 0; i < snakeLength; i++) {    //En for-loop med längden av ormenslängd som går igenom varje x- och y-värde ormen har.
    LedSign::Set(snakeX[i], snakeY[i], 2);   //Anger hur starkt LED-dioden ska lysa på denna koordinat på ormen.
  }
}

void movesnake() {     //Funktionen gör så att ormen rör på sig

  for (int i = snakeLength - 1; i > 0; i--) {
    Serial.print("X before: ");
    Serial.print(snakeX[i]);
    Serial.print("\t");
    snakeX[i] = snakeX[i - 1];
    //snakeY[i] = snakeY[i - 1];
    Serial.print("X after: ");
    Serial.println(snakeX[i]);

    
  }

  snakeX[0] += dirX;
  snakeY[0] += dirY;

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

  //Myfont::Banner(leng, text);

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
