#include "Arduino.h"

// Stel de reset pin in
#define INTERRUPT_PIN_RESET 2
// Stel de timing pin in
#define INTERRUPT_PIN_TIMING 3

unsigned int track_size;
unsigned int track_bits;
unsigned int current_index;

// Pointer naar de callback functie
void (*callback_on_cue)(unsigned long);

unsigned long *currentTrack;

unsigned long currentEvent;

/*
 * Hulpje om een binair getal te printen met een vast aantal bits
 */ 
void print_binary(int number, byte Length){
  static int Bits;
  if (number) { //The remaining bits have a value greater than zero continue
    Bits++; // Count the number of bits so we know how many leading zeros to print first
    print_binary(number >> 1, Length); // Remove a bit and do a recursive call this function.
    if (Bits) for (byte x = (Length - Bits); x; x--)Serial.write('0'); // Add the leading zeros
    Bits = 0; // clear no need for this any more
    Serial.write((number & 1) ? '1' : '0'); // print the bits in reverse order as we depart the recursive function
  }
}

/*
 * Interrupt handler van de timing pin
 */
void timing_ISR() {
  static unsigned long last_timing_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 100ms, assume it's a bounce and ignore
  if (interrupt_time - last_timing_interrupt_time > 100 && current_index < track_size) {
    last_timing_interrupt_time = interrupt_time;
    memcpy_P(&currentEvent, &currentTrack[current_index++], sizeof(unsigned long));
    Serial.print(current_index);
      Serial.print("/");
      Serial.print(track_size);
    Serial.print(": ");
    print_binary(currentEvent, track_bits);
    Serial.println();
    callback_on_cue(currentEvent);
  }
  #ifdef TEST
    else {
      Serial.println("Liedje is klaar.");
      reset_ISR();
    }
  #endif      
}

/*
 * Interrupt handler van de reset pin
 */
void reset_ISR() {
  static unsigned long last_reset_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 50ms, assume it's a bounce and ignore
  if (interrupt_time - last_reset_interrupt_time > 50) {
    last_reset_interrupt_time = interrupt_time;
    current_index = 0;
    Serial.println("Reset");
  }
}

/*
 * Initialiseer het instrument
 *
 * @param callback pointer naar de functie die moet worden aangeroepen
 * @param track pointer naar de track array
 * @param array_size de grootte van de track array
 * @param used_bits aantal bits in de track 
 */
void initInstrument(void (*callback)(unsigned long), unsigned long track[], unsigned int array_size, unsigned int used_bits) {
  pinMode(INTERRUPT_PIN_RESET, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN_TIMING, INPUT_PULLUP);
  Serial.print("Deze track heeft ");
  track_size = array_size;
  Serial.print(track_size);
  Serial.println(" events");
  track_bits = used_bits;
  callback_on_cue = callback;
  currentTrack = track;
  #ifndef TEST
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_RESET), reset_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_TIMING), timing_ISR, RISING);
    Serial.println("Interrupt service routines zijn gekoppeld. Instrument luistert vanaf nu naar de dirigent");
  #else
    Serial.println("!TESTMODE! Instrument reageert NIET op de dirigent!");
  #endif
  reset_ISR();
}

/*
 * Hulpje om te testen
 *
 * @param delayTime aantal milliseconden wachttijd tot de volgende cue
 */ 
void testInstrument(int delayTime) {
  timing_ISR();
  delay(delayTime);
}
