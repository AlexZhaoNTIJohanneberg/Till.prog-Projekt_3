#include "Charliplexing.h"

int Speed = 0;

int peaks[10] = {0};

void setup(){

  Serial.begin(9600);
  LedSign::Init(GRAYSCALE);
  Serial.println();
}

void loop(){
  
}
