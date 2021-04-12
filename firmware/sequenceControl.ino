/*
 * Required protoypes / declarations

const int numTargets = 4;
Servo targetServos[numTargets];
int targetServoPins[] = {24, 26, 28, 30};
int targetLEDPins[] = {46, 44, 42, 40};
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


 */

// initialise all the servo instances and assigns pins
void initServos() {
  for (int i = 0; i < numTargets; i++) {
    targetServos[i].attach(targetServoPins[i]);
    pinMode(targetLEDPins[i], OUTPUT);
    digitalWrite(targetLEDPins[i], LOW);
  }
}

// fill the sequence array with 0s
void blankSequence() {
  
  for (int i = 0; i < maxSeqLength; i++) {
    for (int j = 0; j < numTargets+1; j++) {
      sequence[i][j] = 0;
    }
  }
}

// turn all the targets in a particular direction
void faceAllTargets(int face) {
  for (int i = 0; i < numTargets; i++) {
    writeToTarget(i, face);
  }
}

// run next step of sequence, including delay, increment index
void runStep() {
  
  for (int i = 0; i < numTargets; i++){
    writeToTarget(i, sequence[sequenceIndex][i]);
  }

  delay(sequence[sequenceIndex][numTargets] * 1000);

  sequenceIndex++;

  if (sequenceIndex >= sequenceLength) {
    sequenceIndex = 0;
    runningSeq = false;
  }
}

void resetSequence() {
  runningSeq = false;
  sequenceIndex = 0;
  faceAllTargets(0);
}

void parseSequence(String input) {
  blankSequence();
  Serial.println("Parsing...");
  String currentNumber = "";
  int i = 0;
  while (input.charAt(i) != ',') {
    currentNumber.concat(input.charAt(i));
    i++;
  }
  i++;
  
  sequenceLength = currentNumber.toInt();
  
  int lineIndex = 0;
  int itemIndex = 0;

  while (i < input.length()) {
    currentNumber = "";
    
    while (input.charAt(i) != ',') {
      currentNumber.concat(input.charAt(i));
      i++;
    }
    i++;

    sequence[lineIndex][itemIndex] = currentNumber.toFloat();
    itemIndex++;
    
    if (itemIndex > numTargets) {
      itemIndex = 0;
      lineIndex++;
    }
  }

  serialDumpSequence();
}

void writeToTarget(int targetIndex, int face) {
  bool facing = (face == 1);
  Serial.println("Facing target " + (String)targetIndex + (facing ? " to face" : " away"));
  digitalWrite(targetLEDPins[targetIndex], face);
  targetServos[targetIndex].write(20 + (1-face) * 140);
}

 // for debugging
void serialDumpSequence() {
  for (int i = 0; i < maxSeqLength; i++) {
    String output = "";
    for (int j = 0; j < numTargets+1; j++) {
      output += String(sequence[i][j]);
      output += ", ";
    }
    Serial.println(output);
  }
}
