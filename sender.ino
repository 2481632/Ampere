#include <ezButton.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xCD, 0xBE, 0x44};

#define LED1_PIN 4
#define LED2_PIN 14
#define LED3_PIN 5

#define DEBOUNCE_TIME 100 // the debounce time in millisecond, increase this time if it still chatters

ezButton button1(2); // create ezButton object that attach to pin GIOP21;
ezButton button2(13); // create ezButton object that attach to pin GIOP22;
ezButton button3(12); // create ezButton object that attach to pin GIOP23;

int timeBetweenClicks = 5000; // in milliseconds
int lastClick = 0;

void setup() {
  Serial.begin(115200);
  button1.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds
  button2.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds
  button3.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first

  //Serial.println(button1.getStateRaw());

  if (button1.isPressed()) {
    Serial.println("The button 1 is pressed");
    if (lastClick + timeBetweenClicks < millis()) {
      int buttonId = 1;
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);
      lastClick = millis();
      esp_now_send(broadcastAddress, (uint8_t *) &buttonId, sizeof(int));
    } else {
      Serial.println("wait");
    }

  }


  if (button1.isReleased())
    //digitalWrite(LED1_PIN, LOW);
    Serial.println("The button 1 is released");

  if (button2.isPressed()) {
    Serial.println("The button 2 is pressed");
    if (lastClick + timeBetweenClicks < millis()) {
      int buttonId = 2;
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, LOW);
      lastClick = millis();
      esp_now_send(broadcastAddress, (uint8_t *) &buttonId, sizeof(int));
    } else {
      Serial.println("wait");
    }

  }


  if (button2.isReleased())
    Serial.println("The button 2 is released");

  if (button3.isPressed()) {
    Serial.println("The button 3 is pressed");
    if (lastClick + timeBetweenClicks < millis()) {
      int buttonId = 3;
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, HIGH);
      lastClick = millis();
      esp_now_send(broadcastAddress, (uint8_t *) &buttonId, sizeof(int));
    } else {
      Serial.println("wait");
    }

  }


  if (button3.isReleased())
    Serial.println("The button 3 is released");
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
