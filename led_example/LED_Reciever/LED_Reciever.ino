// Include the necessary libraries to initialize the radios
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24Network.h>
#include <RF24.h>

// Initialize the radio and the network
RF24 radio(7, 8);  // nRF24L01 radio
RF24Network network(radio);   // Network uses that radio

/ /Create the structure for the payload to be transmitted
struct payload_t{
  int16_t id;  // The device ID
  unsigned int signal;  // The signal that is sent, should the LED be on?
};

// Initialize the temporary payload to be added to the queue
payload_t received_payload;  // The payload that was received
RF24NetworkHeader header;

//Initialize global Constants  
const uint16_t this_node = 0;  // Unique address of this node
const uint16_t other_node = 1; // Unique address of the transmitter node

// Initialize global variables
int led_pin = 9;  // The pin to turn on the LED when a signal is received
unsigned int signal = 0;  // The initial signal value is 0 = LED off

void setup()
{
  // Initialize the serial port for debugging
  Serial.begin(9600);
  Serial.println("Beginning");
  
  // Initialize Network
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  
  // Initialize LED pin to output
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  // Pump the network regularly checking for new transmissions
  network.update();
 
  // Check to see if there is something on the network is being transmitted
  while ( network.available() )
  {
    // Grab what is on the network and store it in received_payload
    network.read(header, &received_payload, sizeof(received_payload));

    // Print out what was recieved
    Serial.println("testing read");
    Serial.println(received_payload.signal);
    signal = received_payload.signal;

    // Change the state of the LED based on the signal recieved
    if (signal == 1) {
       digitalWrite(led_pin, HIGH);
    } else {
       digitalWrite(led_pin, LOW); 
    }
    
  }
}
