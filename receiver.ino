#include <esp_now.h>
#include <WiFi.h>
#include <Preferences.h>
#include <Stepper.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LEDPIN 2
#define NUMPIXELS 12
#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
Preferences preferences;

// Touch sensor
unsigned long previousMillis = 0;
unsigned long elapsedMillis = 0;
int debounceTime = 1000;
int pinTouchSensor = 15;

const int stepsPerRevolution = 2048;

Stepper stepper1(stepsPerRevolution, 13, 14, 12, 27);
Stepper stepper2(stepsPerRevolution, 26, 33, 25, 32);
Stepper stepper3(stepsPerRevolution, 23, 21, 22, 19);
Stepper stepper4(stepsPerRevolution, 18, 4, 5, 0);

Stepper steppers[4] = {stepper1, stepper2, stepper3, stepper4};
int stepCount[4] = {0, 0, 0, 0};

int position1[4] = {500, 300, 200, 400};
int position2[4] = {600, 111, 555, 800};
int position3[4] = {110, 520, 840, 1190};

int startPosition[4] = {0, 0, 0, 0};

boolean blocked = false;
int delayPerStep = 5; // Time (in milliseconds) between each step

void onDataReceive(const uint8_t *mac, const uint8_t *data, int len)
{
  printf("** Data Received **\n\n");
  printf("Received from MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  printf("Length: %d byte(s)\n", len);
  printf("Data: %d\n\n", data[0]);
  if (!blocked) {
    blocked = true;
    boolean positionReached[4] = {false, false, false, false};
    if (data[0] == 1) {
      for (int i = 0; i <= stepsPerRevolution; i++) {
        for (int x = 0; x < 4; x++) {
          if (stepCount[x] > position1[x] && !positionReached[x]) {
            steppers[x].step(-1);
            stepCount[x] = stepCount[x] - 1;
          } else if (stepCount[0] < position1[x] && !positionReached[x]) {
            steppers[x].step(1);
            stepCount[x] = stepCount[x] + 1;
          } else {
            positionReached[x] = true;
          }
        }

        if (positionReached[0] && positionReached[1] && positionReached[2] && positionReached[3]) {
          //preferences.putUInt();
          //writeIntIntoEEPROM(20, stepCount[0]);
          //writeIntIntoEEPROM(40, stepCount[1]);
          //writeIntIntoEEPROM(60, stepCount[2]);
          //writeIntIntoEEPROM(80, stepCount[3]);
          break;
        }
        delay(delayPerStep);
      }

    } else if (data[0] == 2) {

      for (int i = 0; i <= stepsPerRevolution; i++) {
        for (int x = 0; x < 4; x++) {
          if (stepCount[x] > position2[x] && !positionReached[x]) {
            steppers[x].step(-1);
            stepCount[x] = stepCount[x] - 1;
          } else if (stepCount[0] < position2[x] && !positionReached[x]) {
            steppers[x].step(1);
            stepCount[x] = stepCount[x] + 1;
          } else {
            positionReached[x] = true;
          }
        }

        if (positionReached[0] && positionReached[1] && positionReached[2] && positionReached[3]) {
        //  writeIntIntoEEPROM(20, stepCount[0]);
        //  writeIntIntoEEPROM(40, stepCount[1]);
         // writeIntIntoEEPROM(60, stepCount[2]);
         // writeIntIntoEEPROM(80, stepCount[3]);
          break;
        }
        delay(delayPerStep);
      }

    } else if (data[0] == 3) {

      for (int i = 0; i <= stepsPerRevolution; i++) {
        for (int x = 0; x < 4; x++) {
          if (stepCount[x] > position3[x] && !positionReached[x]) {
            steppers[x].step(-1);
            stepCount[x] = stepCount[x] - 1;
          } else if (stepCount[0] < position3[x] && !positionReached[x]) {
            steppers[x].step(1);
            stepCount[x] = stepCount[x] + 1;
          } else {
            positionReached[x] = true;
          }
        }

        if (positionReached[0] && positionReached[1] && positionReached[2] && positionReached[3]) {
        //  writeIntIntoEEPROM(20, stepCount[0]);
  //        writeIntIntoEEPROM(40, stepCount[1]);
//          writeIntIntoEEPROM(60, stepCount[2]);
    //      writeIntIntoEEPROM(80, stepCount[3]);
          break;
        }
        delay(delayPerStep);
      }

    } else {
      Serial.println("Received invalid message.");
    }
    blocked = false;
    Serial.println("Stepper 1: " + stepCount[0]);
    Serial.println("Stepper 2: " + stepCount[1]);
    Serial.println("Stepper 3: " + stepCount[2]);
    Serial.println("Stepper 4: " + stepCount[3]);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Coding IxD");

  pinMode(pinTouchSensor, INPUT);

  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Board MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error init ESP-NOW");
    return;
  }
 // EEPROM.begin(1000);
  preferences.begin("steppers", false);

  esp_now_register_recv_cb(onDataReceive);
  stepCount[0] = preferences.getUInt("0", 0);
  stepCount[1] = preferences.getUInt("1", 0);
  stepCount[2] = preferences.getUInt("2", 0);
  stepCount[3] = preferences.getUInt("3", 0);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  boolean positionReached[4] = {false, false, false, false};
  for (int i = 0; i <= stepsPerRevolution; i++) {
    for (int x = 0; x < 4; x++) {
      if (stepCount[x] > startPosition[x] && !positionReached[x]) {
        steppers[x].step(-1);
        stepCount[x] = stepCount[x] - 1;
      } else if (stepCount[0] < startPosition[x] && !positionReached[x]) {
        steppers[x].step(1);
        stepCount[x] = stepCount[x] + 1;
      } else {
        positionReached[x] = true;
      }
    }

    if (positionReached[0] && positionReached[1] && positionReached[2] && positionReached[3]) {
     // writeIntIntoEEPROM(20, stepCount[0]);
      //writeIntIntoEEPROM(40, stepCount[1]);
      //writeIntIntoEEPROM(60, stepCount[2]);
      //writeIntIntoEEPROM(80, stepCount[3]);
      break;
    }
    delay(delayPerStep);
  }

stepCount[0] = 0;
stepCount[1] = 0;
stepCount[2] = 0;
stepCount[3] = 0;
  Serial.println("Stepper 1: " + stepCount[0]);
  Serial.println("Stepper 2: " + stepCount[1]);
  Serial.println("Stepper 3: " + stepCount[2]);
  Serial.println("Stepper 4: " + stepCount[3]);
}

void loop() {
  handleSerial();
  elapsedMillis = millis() - previousMillis;
  pixels.clear(); // Set all pixel colors to 'off'

  if (digitalRead(pinTouchSensor) == HIGH && elapsedMillis > debounceTime) {
    //digitalWrite(LED_BUILTIN, ledState);

    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(220, 255, 255));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }
    // pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }

    Serial.println("Touch Button");
    previousMillis = millis();
  };
}

void handleSerial() {
  if(Serial.available() > 0){
        String input = Serial.readString();
        if (input.startsWith("eeprom", 0)) {
           // int setpper1pos = EEPROM.readInt(20);
           // int setpper2pos = EEPROM.readInt(40);
           // int setpper3pos = EEPROM.readInt(60);
           // int setpper4pos = EEPROM.readInt(80);
           // Serial.println("Stepper 1 Position: " + setpper1pos);
           // Serial.println("Stepper 2 Position: " + setpper2pos);
           // Serial.println("Stepper 3 Position: " + setpper3pos);
           // Serial.println("Stepper 4 Position: " + setpper4pos);
        } else if (input.startsWith("reset", 0)) {
            int setpper1pos = 0;
            int setpper2pos = 0;
            int setpper3pos = 0;
            int setpper4pos = 0;

           // EEPROM.put(0, setpper1pos);
           // EEPROM.put(40, setpper1pos);
           // EEPROM.put(60, setpper1pos);
           // EEPROM.put(80, setpper1pos);

           // int setpper1poss = preferences.getUInt("one", 0);
           // int setpper2poss = preferences.getUInt("two", 0);
          //  int setpper3poss = preferences.getUInt("four", 0);
          //  int setpper4poss = preferences.getUInt("three", 0);
            
         //   EEPROM.get(0, setpper1poss);
           // EEPROM.get(40, setpper1poss);
          //  EEPROM.get(60, setpper1poss);
          //  EEPROM.get(80, setpper1poss);
            
         //   Serial.println("Stepper 1 Position: " + setpper1poss);
         //   Serial.println(setpper1poss);
         //   Serial.println("Stepper 2 Position: " + setpper2poss);
         //   Serial.println("Stepper 3 Position: " + setpper3poss);
         //   Serial.println("Stepper 4 Position: " + setpper4poss);
           // EEPROM.writeInt(0, 1000);
        } else {
          Serial.println(input);
        }
  }
}