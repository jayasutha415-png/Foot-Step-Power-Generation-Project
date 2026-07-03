#include <iostream>
#include <cstdlib>
#include <ctime>

// --- ARDUINO EMULATION LAYER (Allows standard C++ compilers to run Arduino code) ---
#define A0 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

void pinMode(int pin, int mode) {}
void digitalWrite(int pin, int val) {}
void delay(int ms) {}

class SerialSimulator {
public:
    void begin(int speed) {
        std::cout << "[Serial Monitor Initialized at " << speed << " baud]\n";
    }
    void println(const char* text) { std::cout << text << "\n"; }
    void println(int num) { std::cout << num << "\n"; }
    void print(const char* text) { std::cout << text; }
    void print(int num) { std::cout << num; }
};
SerialSimulator Serial;

// Simulated analogRead that randomly mimics footstep spikes
int analogRead(int pin) {
    if ((rand() % 10) > 7) { 
        return rand() % 400 + 100; // Simulates a real step impact (voltage spike)
    }
    return rand() % 20; // Simulates background noise/resting state
}
// ---------------------------------------------------------------------------------


// ==================== YOUR ACTUAL ARDUINO CODE STARTS HERE ====================

const int piezoPin = A0;    // Piezoelectric disc connected to Analog Pin A0
const int ledPin = 13;      // Built-in LED to flash when a footstep is detected
const int threshold = 50;   // Minimum reading to qualify as a valid footstep

int piezoValue = 0;         // Variable to store the value read from the piezo
int stepCount = 0;          // Variable to keep track of total steps

void setup() {
  pinMode(ledPin, OUTPUT);  // Set LED pin as an output
  Serial.begin(9600);       // Initialize Serial Communication
  Serial.println("System Initialized. Start walking!");
}

void loop() {
  // Read the analog value from the piezo disc (returns 0 to 1023)
  piezoValue = analogRead(piezoPin);

  // If the impact is stronger than our threshold, count it as a step
  if (piezoValue > threshold) {
    stepCount++;
    
    // Flash the onboard LED for visual feedback
    digitalWrite(ledPin, HIGH);
    
    // Print the data to the Screen
    Serial.print("Step Detected! | Power Intensity: ");
    Serial.print(piezoValue);
    Serial.print(" | Total Steps: ");
    Serial.println(stepCount);
    
    delay(200); // Simple debounce delay to avoid double-counting
    digitalWrite(ledPin, LOW); 
  }
  
  delay(10); // Small delay for stability
}

// Standard C++ main function to run the Arduino loop 20 times for testing
int main() {
    srand(time(0)); // Seed random generator
    setup();
    for(int i = 0; i < 20; i++) {
        loop();
    }
    std::cout << "\n=== Simulation Finished Successfully ===" << std::endl;
    return 0;
}