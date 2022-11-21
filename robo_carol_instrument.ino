// TEST mode
// Als deze regel niet is uitgecomment zal het Instrument in test mode staan.
// Dit betekent dat hij onafhankelijk van de dirigent de `event()` functie
// aanroept. Hiermee kun je je instrument testen.
#define TEST

// Aantal milliseconde tussen twee test-events
// Hoe kleiner dit getal, des te sneller loopt de test
#define TEST_SPEED 250

#define TRACK0BITS 11

//#include "track.h"
#include "WeWishYouAMerryChristmas.h"
#include "robo_carol.h"

// Snelheid van communicatie over de USB kabel. Zorg ervoor dat de seriële
// monitor van de Arduino IDE op dezelfde snelheid is ingesteld.
#define BAUD_RATE 115200

/*
 * De `setup()` functie wordt één keer uitgevoerd na opstarten of een reset
 */
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Instrument is bezig met opstarten");

  // Configureer de pins. de LED_BUILTIN is pin 13: de ingebouwde LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialiseer het instrument. Pas dit zo aan dat het overeenkomt met je
  // gekozen track. Dus track0, TRACK0SIZE, TRACK0BITS of track1, TRACK1SIZE, 
  // TRACK1BITS, etc.
  initInstrument(&event, track0, TRACK0SIZE, TRACK0BITS);
  Serial.println("Instrument is opgestart");
}

/*
 * De `loop()` functie wordt iedere keer herhaald totdat de Arduino wordt 
 * uitgezet of gereset.
 */

void loop() {
  #ifdef TEST
    testInstrument(500);
  #endif
}

/*
 * De `event()` functie wordt iedere keer aangeroepen als dit instrument een
 * teken krijgt van de dirigent. De meegegeven parameter `data` bevat de informatie
 * over wat dit instrument tijdens dit event moet doen.
 */
void event(unsigned long data) {
  // De LED_BUILTIN omwisselen. Dit is alleen maar even ter demonstratie
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
