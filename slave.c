#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h" 
#include <esp_now.h>
#include <WiFi.h>

USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl; 

const int ledPins[] = {4, 5, 3, 7}; 

// Track current profile (1 = General, 2 = Media, 3 = Automation)
int currentProfile = 1; 

typedef struct struct_message {
    int buttonNum;
} struct_message;

struct_message receivedData; 

// Update LEDs to show active profile
void displayProfile() {
    for(int i=0; i<4; i++) digitalWrite(ledPins[i], LOW);  
    digitalWrite(ledPins[currentProfile - 1], HIGH);       
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    int btn = receivedData.buttonNum;
    
     
    if(btn == 14) {
        currentProfile++;
        if(currentProfile > 3) currentProfile = 1;  
        
        // Blink all LEDs to confirm Mode Change
        for(int i=0; i<4; i++) digitalWrite(ledPins[i], HIGH);
        delay(150);
        displayProfile(); // Nayi active profile dikhayein
        return; // Yahan se wapas laut jayein, koi shortcut press na karein
    }

     
    if(btn >= 1 && btn <= 4) {
        
        // PROFILE 1: Windows Navigation
        if (currentProfile == 1) {
            switch(btn) {
                case 1: Keyboard.press(KEY_LEFT_GUI); Keyboard.press('l'); break; // Lock PC
                case 2: Keyboard.press(KEY_LEFT_GUI); Keyboard.press('d'); break; // Show Desktop
                case 3: Keyboard.press(KEY_LEFT_CTRL); Keyboard.press('c'); break; // Copy
                case 4: Keyboard.press(KEY_LEFT_CTRL); Keyboard.press('v'); break; // Paste
            }
        }
        
        // PROFILE 2: Media Controller
        else if (currentProfile == 2) {
            switch(btn) {
                case 1: ConsumerControl.press(CONSUMER_CONTROL_PLAY_PAUSE); break;
                case 2: ConsumerControl.press(CONSUMER_CONTROL_MUTE); break;
                case 3: ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT); break;
                case 4: ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT); break;
            }
        }

        // PROFILE 3:  THE AUTOMATION MODE 
        else if (currentProfile == 3) {
            switch(btn) {
                case 1: // Auto 1: Workspace Setup (Chrome + VS Code)
                    Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r'); Keyboard.releaseAll(); delay(300);
                    Keyboard.print("chrome"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    delay(800); // Browser open hone ka wait
                    Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r'); Keyboard.releaseAll(); delay(300);
                    Keyboard.print("code"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    break;
                
                case 2: // Auto 2: Launch Gemini AI
                    Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r'); Keyboard.releaseAll(); delay(300);
                    Keyboard.print("https://gemini.google.com"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    break;
                
                case 3: // Auto 3: Quick CMD Ping (Network check)
                    Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r'); Keyboard.releaseAll(); delay(300);
                    Keyboard.print("cmd"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    delay(500); 
                    Keyboard.print("ping 8.8.8.8"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    break;
                
                case 4: // Auto 4: Open 'Downloads' Folder Directly
                    Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r'); Keyboard.releaseAll(); delay(300);
                    Keyboard.print("shell:downloads"); Keyboard.press(KEY_RETURN); Keyboard.releaseAll();
                    break;
            }
        }

         
        delay(100);
        Keyboard.releaseAll(); 
        ConsumerControl.release();
    }
    
     
    else if(btn >= 11 && btn <= 13) {
        switch(btn) {
            case 11: // Long Press 1: Task Manager
                Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_LEFT_SHIFT); Keyboard.press(KEY_ESC); 
                break;
            case 12: // Long Press 2: File Explorer
                Keyboard.press(KEY_LEFT_GUI); Keyboard.press('e'); 
                break; 
            case 13: // Long Press 3: Close Current Window (Alt+F4)
                Keyboard.press(KEY_LEFT_ALT); Keyboard.press(KEY_F4); 
                break;
        }
        delay(100);
        Keyboard.releaseAll();
    }
}

void setup() {
    // LED Pins setup
    for(int i=0; i<4; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
    
    
    Keyboard.begin();
    ConsumerControl.begin(); 
    USB.begin();
    
     
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return;
    esp_now_register_recv_cb(OnDataRecv);
    
    // Boot hote hi Profile 1 ki LED on karein
    displayProfile(); 
}

void loop() {
     
}
