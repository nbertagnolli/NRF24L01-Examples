
// Include the necessary libraries to initialize the radios
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24Network.h>
#include <RF24.h>

// Initialize the radio and the network
RF24 radio(7, 8);  // nRF24L01 radio
RF24Network network(radio);   // Network uses that radio

//Create the structure for the payload to be transmitted
struct payload_t{
  int16_t id;  // device ID
  unsigned int signal;  // The signal that is sent should the LED be on?
};

// Initialize the temporary payload to be added to the queue
payload_t transmit_payload;  // The payload to be transmitted next.

//Initialize global Constants  
const uint16_t this_node = 1;  // Unique address of our node
const uint16_t other_node = 0; // Unique address of the reciever node

// Initialize global variables
boolean transmit_ready;  // Has the button been pressed are we ready to transmit
int button_pin = 9;  //  Which pin has the button

void setup()
{
  // Setup the serial port for debugging and analyzing the transmitter
  Serial.begin(9600);
  Serial.println("Beginning");
  
  //Initialize Network
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  transmit_ready = true;

  // Setup the first payload, we want the LED to be on
  transmit_payload.id = this_node;
  transmit_payload.signal = 1;
  
  // Initialize Button Pin to input mode to tell when pressed
  pinMode(button_pin, INPUT);
}

void loop()
{
   // If there is a transmission to send, send it.
  if(transmit_ready){
      // Print tests for the signal transmission
      Serial.println("Testing Transmission Payload");
      Serial. println(transmit_payload.id);
      Serial.println(transmit_payload.signal);
      
      // Update the network to see if nodes are available
      network.update();
      RF24NetworkHeader header(/*to node*/ other_node);
      boolean ok = network.write(header,&transmit_payload,sizeof(transmit_payload));

        
      // Checks to see that this data was successfully transmitted if it was
      // we can stop trying to transmit
      if(ok){
        transmit_ready = false; 
        Serial.println("Transmission Successful");
        ok = false;
      }
  }
  
  delay(50);
  
  // Read from the button and set transmission to ready
  if (digitalRead(button_pin) == 1){
    
    // Toggle the value of the payload signal when button is pressed
    if (transmit_payload.signal == 1) {
      transmit_payload.signal = 0;
      transmit_ready = true;
    } else {
      transmit_payload.signal = 1;
      transmit_ready = true;
    }
    
    Serial.println("Pressed");
    delay(25);
  }
  
  
}
