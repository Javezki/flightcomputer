#include <Arduino.h>

void setup() {
  Serial5.begin(9600);
}

void loop() {
  Serial5.print("Hello, World!\n");
  delay(1000);
  Serial5.print("Goodbye, World!\n");
  delay(1000);
}
