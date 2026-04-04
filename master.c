#include <esp_now.h>
#include <WiFi.h>

// YOUR SLAVE'S MAC ADDRESS GOES HERE
uint8_t broadcastAddress[] = {0xB8, 0xF8, 0x62, 0xF7, 0xEF, 0xC4};       

const int buttonPins[] = {12, 13, 14, 27}; 
const int numButtons = 4;

typedef struct struct_message {
    int buttonNum;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  
  for(int i=0; i<numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Master Ready. Press buttons!");
}

void loop() {
  for(int i=0; i<numButtons; i++) {
    if(digitalRead(buttonPins[i]) == LOW) {
      unsigned long startTime = millis();
      
      // Wait until you let go of the button
      while(digitalRead(buttonPins[i]) == LOW); 
      unsigned long duration = millis() - startTime;

      // Check how long it was held
      if(duration > 800) { 
        // LONG PRESS
        myData.buttonNum = (i + 1) + 10; 
        Serial.printf("Long Press Sent: %d\n", myData.buttonNum);
      } else if (duration > 50) { 
        // SHORT PRESS
        myData.buttonNum = i + 1;
        Serial.printf("Short Press Sent: %d\n", myData.buttonNum);
      }

      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      delay(150); // Debounce
    }
  }
}
