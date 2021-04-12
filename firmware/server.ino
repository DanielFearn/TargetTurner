/*

 * Required prototypes / declarations

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

 */

void initServer() {
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet hardware not found / connected.");
    while (1);
  }
  
  server.begin();

  if (!SD.begin(4)) {
    Serial.println("MicroSD card error.");
    while (1);
  }
}

void checkForRequest() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Request receing...");
    receiveRequest(client); // handles requestData by ref
    Serial.println("Request received.");
    handleRequest(client);
  }
}

void receiveRequest(EthernetClient client) {
  for (int i = 0; i < maxRequestWords; i++) {
    requestData[i] = "";
  }

  bool currentLineBlank = true;
  int currentWordIndex = 0;
  
  while (client.connected()) {
    if (client.available()) {
      
      char c = client.read();

      if (c == '\n' && currentLineBlank) {
        return;
      }

      if (c == ' ' || c == '\n') {
        currentWordIndex++;
      } else {
        requestData[currentWordIndex].concat(c);
      }

      if (c == '\n') {
        currentLineBlank = true;
      } else if (c != '\r') {
        currentLineBlank = false;
      }
      
    }
  }
}

void sendDefaultHeaders(EthernetClient client) {
  client.println("Content-Type: text/html");
  client.println("Server: TargetTurner");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
}

void handleRequest(EthernetClient client) {
  
  String path = requestData[1];

  Serial.println("Handling request for " + path + "...");

  if (path == "/") {
    serveFile(client, "/gui.htm");
    
  } else if (path.startsWith("/upload")) {
    parseSequence(path.substring(8));
    serveFile(client, "/ok.htm");
    
  } else if (path.startsWith("/start")) {
    resetSequence();
    runningSeq = true;
    serveFile(client, "/ok.htm");
    
  } else if (path.startsWith("/stop")) {
    resetSequence();
    serveFile(client, "/ok.htm");
    
  } else if (path.startsWith("/faceall")) {
    faceAllTargets(1);
    serveFile(client, "/ok.htm");
    
  } else if (path.startsWith("/awayall")) {
    faceAllTargets(0);
    serveFile(client, "/ok.htm");
    
  } else {
    serveFile(client, path);
  }

  client.stop();
  Serial.println("Request complete");
}

void serveFile(EthernetClient client, String path) {
  File file = SD.open(path);

  if (file) {
    client.println("HTTP/1.1 200 OK");
    sendDefaultHeaders(client);
    while (file.available()) {
      client.print((char)file.read());
    }
    file.close();
  } else {
    client.println("HTTP/1.1 404 File not found");
    sendDefaultHeaders(client);
    client.println("404 page not found");
  }
}
