#include <MaxMatrix.h>
#include "ledScroll.h"

ledScroll fs;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  fs.loop();
}
