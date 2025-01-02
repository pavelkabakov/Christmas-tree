#define LED_1 5
#define LED_2 4
#define LED_3 0
#define LED_4 2
#define LED_5 14
#define LED_6 12
#define LED_7 13
#define LED_8 15
#define LED_9 3
#define LED_10 1
#define BUTTON_PIN 16 // Button on pin 16

const int leds[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8, LED_9, LED_10};
const int numLeds = sizeof(leds) / sizeof(leds[0]);
unsigned long previousMillis = 0;
const unsigned long interval = 500; // Interval for LED changes
int currentLed = 0;
int currentMode = 5; // Default mode: 5 (All LEDs off)
bool lastButtonState = HIGH;

int brightness = 0;  // PWM brightness for LEDs
bool fadingUp = true; // Flag to track whether we are increasing or decreasing the brightness

int pairBrightness[5] = {0}; // Array to store brightness for each LED pair
bool pairFadingUp[5] = {true}; // Array to store the fading direction for each pair

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Program initialized.");

  // Setup LEDs
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    analogWrite(leds[i], 0); // Set initial PWM value to 0 (off)
  }

  // Setup button
  pinMode(BUTTON_PIN, INPUT); // Button connected to ground (active LOW)
  Serial.println("All LEDs are off. Ready.");
}

void loop() {
  // Read button state
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      currentMode = (currentMode + 1) % 6; // Switch modes (now up to 6)
      Serial.print("Button pressed. Current mode: ");
      switch (currentMode) {
        case 0:
          Serial.println("Blinking LEDs one by one.");
          break;
        case 1:
          Serial.println("All LEDs on.");
          break;
        case 2:
          Serial.println("Running light.");
          break;
        case 3:
          Serial.println("Groups of LEDs.");
          break;
        case 4:
          Serial.println("PWM fade.");
          break;
        case 5:
          Serial.println("All LEDs off.");
          break;
      }
    }
  }
  lastButtonState = buttonState;

  if (currentMode == 0) {
    // Mode 0: Blinking LEDs one by one
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Turn off the current LED
      digitalWrite(leds[currentLed], LOW);

      // Move to the next LED
      currentLed++;
      if (currentLed >= numLeds) {
        currentLed = 0; // Wrap back to the first LED
      }

      // Turn on the next LED
      digitalWrite(leds[currentLed], HIGH);
    }
  } else if (currentMode == 1) {
    // Mode 1: Turn all LEDs on
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(leds[i], HIGH); // Turn all LEDs on
    }
  } else if (currentMode == 2) {
    // Mode 2: Running light
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Turn off all LEDs
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(leds[i], LOW);
      }

      // Turn on only the current LED
      digitalWrite(leds[currentLed], HIGH);

      // Move to the next LED
      currentLed++;
      if (currentLed >= numLeds) {
        currentLed = 0; // Wrap back to the first LED
      }
    }
  } else if (currentMode == 3) {
    // Mode 3: Groups of LEDs (4 LEDs at a time)
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Turn off all LEDs
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(leds[i], LOW);
      }

      // Turn on the current group of LEDs (4 LEDs at a time)
      for (int i = currentLed; i < currentLed + 4 && i < numLeds; i++) {
        digitalWrite(leds[i], HIGH);
      }

      // Turn off the previous group of LEDs
      for (int i = currentLed - 4; i < currentLed && i >= 0; i++) {
        digitalWrite(leds[i], LOW);
      }

      // Move to the next group of LEDs
      currentLed++;
      if (currentLed >= numLeds) {
        currentLed = 0; // Wrap back to the first group
      }
    }
  } else if (currentMode == 4) {
    // Mode 4: PWM fade
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 30) { // Adjust fade speed
      previousMillis = currentMillis;

      // Fade in or out for all LEDs
      if (fadingUp) {
        brightness += 5;  // Increase brightness
        if (brightness >= 255) {
          brightness = 255;  // Max brightness
          fadingUp = false;  // Start fading down
        }
      } else {
        brightness -= 5;  // Decrease brightness
        if (brightness <= 0) {
          brightness = 0;  // Min brightness
          fadingUp = true;  // Start fading up
        }
      }

      // Set the same brightness for all LEDs
      for (int i = 0; i < numLeds; i++) {
        analogWrite(leds[i], brightness);
      }
    }
  } else if (currentMode == 5) {
    // Mode 5: All LEDs off
    for (int i = 0; i < numLeds; i++) {
      analogWrite(leds[i], 0);  // Turn off all LEDs
    }
  }
}
