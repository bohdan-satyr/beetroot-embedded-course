#include <Arduino.h>

const int PIN_TRIG_TRANSISTOR = 14;
const int PIN_TRIG_RELAY = 21;

const int MEASURE_TRANSISTOR_PIN = 5;
const int MEASURE_RELAY_PIN = 13;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_TRIG_TRANSISTOR, OUTPUT);
  pinMode(PIN_TRIG_RELAY, OUTPUT);
  
  pinMode(MEASURE_TRANSISTOR_PIN, INPUT);
  pinMode(MEASURE_RELAY_PIN, INPUT_PULLDOWN);

  digitalWrite(PIN_TRIG_TRANSISTOR, LOW);
  digitalWrite(PIN_TRIG_RELAY, LOW);
  
  Serial.println("--- Тест запущено ---");
  delay(2000);
}

void loop() {
  unsigned long start_time;
  unsigned long t_transistor = 0;
  unsigned long t_relay = 0;

  Serial.println("\n========== Початок замірів ==========");


  digitalWrite(PIN_TRIG_TRANSISTOR, LOW);
  delay(200);
  
  start_time = micros();
  digitalWrite(PIN_TRIG_TRANSISTOR, HIGH);
  while (digitalRead(MEASURE_TRANSISTOR_PIN) == LOW) {

  }
  t_transistor = micros() - start_time;
  digitalWrite(PIN_TRIG_TRANSISTOR, LOW);
  
  Serial.print("1. Час спрацювання транзистора: ");
  Serial.print(t_transistor);
  Serial.println(" мкс");

  delay(500);

  if (digitalRead(MEASURE_RELAY_PIN) == HIGH) {
    Serial.println("2. Помилка: На контакті реле 3.3V у спокої! Будь ласка, переставте дріт 3.3V на інший боковий гвинт реле.");
    delay(5000);
    return;
  }

  start_time = micros();
  digitalWrite(PIN_TRIG_RELAY, HIGH);
  
  unsigned long timeout = micros();
  bool relay_switched = false;
  
  while ((micros() - timeout) < 40000) {
    if (digitalRead(MEASURE_RELAY_PIN) == HIGH) {
      t_relay = micros() - start_time;
      relay_switched = true;
      break;
    }
  }
  digitalWrite(PIN_TRIG_RELAY, LOW);

  if (relay_switched) {
    Serial.print("2. Час спрацювання ланцюга реле: ");
    Serial.print(t_relay);
    Serial.println(" мкс");
    
    Serial.printf("\nВисновок для звіту: Транзистор (~%lu мкс) працює швидше за механічне реле (~%lu мкс) у %.1f разів!\n", 
                  t_transistor, t_relay, (float)t_relay / (t_transistor == 0 ? 1 : t_transistor));
  } else {
    Serial.println("2. Помилка: Реле клацнуло, але контакт не замкнувся за 40 мс.");
  }

  Serial.println("=====================================");
  delay(7000);
}
