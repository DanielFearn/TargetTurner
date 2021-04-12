#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Servo.h>
#include <string.h>

// server prototypes / declarations
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
const int maxRequestWords = 100;
String requestData[maxRequestWords];
void initServer();
void checkForRequest();
void receiveRequest(EthernetClient client);
void sendDefaultHeaders(EthernetClient client);
void handleRequest(EthernetClient client);
void serveFile(EthernetClient client, String path);

// sequenceControl prototypes / declarations
const int numTargets = 10;
Servo targetServos[numTargets];
int targetServoPins[] = {31, 33, 35, 37, 39, 41, 43, 45, 47, 49};
int targetLEDPins[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40};
const int maxSeqLength = 30;
float sequence[maxSeqLength][numTargets + 1];
int sequenceIndex = 0;
int sequenceLength = 0;
bool runningSeq = false;
void initServos();
void blankSequence();
void faceAllTargets(int face);
void runStep();
void resetSequence();
void parseSequence(String input);
void serialDumpSequence();


void setup() {
  
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("Target Controller by Daniel Fearn");

  initServer();
  initServos();
  faceAllTargets(0);

  Serial.println("Setup Complete.");
}

void loop() {
  checkForRequest();

  if (runningSeq) {
    runStep();
  }
  
}
