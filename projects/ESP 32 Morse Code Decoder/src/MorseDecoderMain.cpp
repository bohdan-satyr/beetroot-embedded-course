#include <Arduino.h>

#define LED_PIN 18
#define BUTTON_PIN 5 
#define POTENTIOMETER_PIN 6
#define BUZZER_PIN 4
#define DEBOUNCE_TIME 50
#define FREQ 800

unsigned int T; // base time unit in milliseconds
unsigned int L; // pause between letters
unsigned int W; // pause between words
unsigned int lastT = 0;

unsigned long stateStartTime = 0;
unsigned long pressDuration = 0;

String morseCodeBuffer = "";

enum SystemState {
  STATE_IDLE,
  STATE_TYPING_SYMBOL,
  STATE_WAITING_LETTER,
  STATE_WAITING_WORD
};

SystemState currentState = STATE_IDLE;

struct MorseMapping {
  const char* code;
  char asciiChar;
};

char parseClickTime(unsigned long time, unsigned int currentT);
char decodeMorse(const String& morseCode);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  int potValue = analogRead(POTENTIOMETER_PIN);

  T = map(potValue, 0, 4095, 100, 700);
  L = 3 * T;
  W = 7 * T; 
  if (abs((int)T - (int)lastT) > 30) {
    Serial.print("\n[Speed T changed: ");
    Serial.print(T);
    Serial.println(" ms]");
    lastT = T;
  }

  unsigned long currentMillis = millis();

  switch (currentState) { 
    case STATE_IDLE: {
      if (buttonState == LOW) {
        stateStartTime = currentMillis;
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, FREQ);
        currentState = STATE_TYPING_SYMBOL;
      }
    } break;
    case STATE_TYPING_SYMBOL: {
      if (buttonState == HIGH) {
        pressDuration = currentMillis - stateStartTime;
        digitalWrite(LED_PIN, LOW);
        noTone(BUZZER_PIN);
        char morseCode = parseClickTime(pressDuration, T);
        if (morseCode != ' ') {
          morseCodeBuffer += morseCode;
          Serial.print(morseCode);
        }
        
        stateStartTime = currentMillis;
        currentState = STATE_WAITING_LETTER;
      }
    } break;
    case STATE_WAITING_LETTER: {      
      if (buttonState == LOW) {
        stateStartTime = currentMillis;
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, FREQ);
        currentState = STATE_TYPING_SYMBOL;
      } else if (currentMillis - stateStartTime > L) {
        if (morseCodeBuffer.length() > 0) {
          Serial.print(" -> ");
          Serial.print(decodeMorse(morseCodeBuffer));
          Serial.print(" | ");
          morseCodeBuffer = "";
        }
        stateStartTime = currentMillis;
        currentState = STATE_WAITING_WORD;
      }
    } break;
    case STATE_WAITING_WORD: {
      if (buttonState == LOW) {
        stateStartTime = currentMillis;
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, FREQ);
        currentState = STATE_TYPING_SYMBOL;
      } else if (currentMillis - stateStartTime > (W - L)) {
        Serial.println();
        Serial.println("--- End of Word ---");
        currentState = STATE_IDLE;
      }
    } break;
  }
}

char parseClickTime(unsigned long time, unsigned int currentT) {
  if (time < DEBOUNCE_TIME) return ' ';
  if (time < (2 * currentT)) {
    return '.';
  } else {
    return '-';
  }
}

char decodeMorse(const String& morseCode) {

  static const MorseMapping morseDictionary[] = {
      {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
      {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
      {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
      {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
      {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
      {"--..", 'Z'},

      {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, {".....", '5'},
      {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'}, {"-----", '0'}
  };

  int dictionarySize = sizeof(morseDictionary) / sizeof(morseDictionary[0]);

  for (int i = 0; i < dictionarySize; i++) {
    if (morseCode == morseDictionary[i].code) {
        return morseDictionary[i].asciiChar; 
    }
  }

  return '?';
}