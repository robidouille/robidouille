/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#include <UltraSound1.h>

UltraSound1 gSonar;

void setup() {
  gSonar.Initialize(2);  
  Serial.begin(115200);
}

void loop() {
  int distance = gSonar.MesaureCM();
  Serial.println(distance);
  delay(200);
}
