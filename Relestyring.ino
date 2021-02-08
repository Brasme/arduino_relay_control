#include "Arduino.h"
#include "Relay.h"
#include "Button.h"

// Pin Definitions
#define PUSHBUTTONMOMENTARY_PIN  2
#define RELAYMODULE_PIN_SIGNAL  3

Relay relayModule(RELAYMODULE_PIN_SIGNAL);
Button pushButtonMomentary(PUSHBUTTONMOMENTARY_PIN);

int state=0;
void setup() {
  Serial.begin(9600);
  Serial.println("Relay control");
  pushButtonMomentary.init();
  relayModule.off(); 
  state=0;
}

void handle_serial()
{
  if (!Serial.available())
    return;
  int c=Serial.read();
  if (c>=32) {
    Serial.print(c,DEC);
    Serial.print(":'");
    Serial.write(c);
    Serial.println("'");    
  } 

  if (c=='h') { Serial.println("Help: 1:ON, 0:Off, s:State"); }
  if (c=='s') { Serial.print("State: "); Serial.println(state?"ON":"OFF"); }

  if (c=='1') { Serial.println("ON"); relayModule.on(); delay(500); state=1; }
  if (c=='0') { Serial.println("OFF"); relayModule.off(); delay(500); state=0; }
}

long time0;
long timeout=1000;

void handle_button()
{
  bool pushButtonMomentaryVal = pushButtonMomentary.read();
  if (pushButtonMomentaryVal && (millis() - time0 > timeout)) {
    if (state==0) { Serial.println("ON (b)"); relayModule.on(); state=1;}
    else { Serial.println("OFF (b)"); relayModule.off(); state=0;}
    time0=millis();
  }
}
void loop() {
  handle_serial();
  handle_button();
}
