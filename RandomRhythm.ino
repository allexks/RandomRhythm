/*
   Random rhythm generator

   Required hardware and circuit setup:
   Speaker attached to pin 9
   Button attached to pin 12
*/

// Speaker
const int speaker = 9; //pin

// Button
// While the button is pressed, the rhythm plays through the speaker
int buttonPin = 12;          // Pin of button 1
int buttonState=0;           // State of button 1


// Time Signature:
const int beats = 4;
const int beatnote = 4;

// Tempo
const int bpm = 100;
float beatTime = 60000 / bpm;

// Chance multipliers (in %)
const int pauseChance = 20;

const int pauseInSequenceChance = 25; //Chance of having a pause in a triplet sequence
const int tripletChance = 15;

const int singledotChance = 20;
const int doubledotChance = 5;


// A list with the length (in beats x 1000) of notes
int noteLen[6];


//int ledState = 1;
//int ledPin = 13;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  float k = 1000.0;
  for (int i = 0; i < 5; i++) {
    noteLen[i] = (int)(k * beatnote);

    k /= 2;
    /* noteLen indexes:
    0 -> whole note
    1 -> half note
    2 -> quarter note
    3 -> 8th note
    4 -> 16th note
  */
  }

    pinMode (buttonPin, INPUT);      // Sets pin 12 as a input for the button 
    digitalWrite (buttonPin, HIGH);  // Activates the pullup resistor of pin 12

    //pinMode(ledPin, OUTPUT);
}


boolean randChance(int chance) {
  int r = random(1, 101);
  if (chance >= r) {
    return true;
  } else {
    return false;
  }
}


void loop() {

  //vars
  boolean rhythmReady = false; // shows whether we have successfully generated the rhythm
  int Measure[33]; // will hold the notes (their lengths in beatsx1000)
  
  //int counter = 0;
  //String readNote = "";
  while (rhythmReady == false) {
    /* UNCOMENT THIS  AND THE 2 LINES ABOVE IF THE NOTES ARE BEING RECEIVED THRU SERIAL
      if (Serial.available() > 0) {
      char data = Serial.read();
      if (data == ' ') {
        int readNoteInt = readNote.toInt();
        Measure[counter] = readNoteInt;
        counter += 1;
        readNote = "";
      } else if (data == '$') {
        Measure[counter] = 0;
        rhythmReady = true;
      } else {
        readNote += data;
      }

      }

      delay(10);
    AND THEN COMMENT EVERYTHING ELSE IN THIS LOOP*/
    int timeLeft = beats * 1000;
    int noteNum = 0;

    while (timeLeft > 0) {

      // Vars for note generation
      int lengthn = 0; //will hold current note length
      int tripletNotesLeft = 0; //helps with the generation of triplets


      // NEW NOTE
      //---------
      //Pause or tone?
      boolean isPause = randChance(pauseChance);

      //What type of note?
      lengthn = noteLen[random(0, 5)];
      if (timeLeft < lengthn) continue;

      //Triplet sequence?
      boolean TripletMode = false;
      if ((2*lengthn)%1000==0 && timeLeft >= 2*lengthn){
        TripletMode = randChance(tripletChance);
      }
      
      if (TripletMode) tripletNotesLeft = 3;

      while (tripletNotesLeft > 0) {
        isPause = randChance(pauseInSequenceChance);
        
        int newLength = (int)(lengthn * (2.0 / 3));

        if (isPause) {
          Measure[noteNum] = -newLength; // if it's a pause, we make it negative
        } else {
          Measure[noteNum] = newLength;
        }
        noteNum += 1;
        tripletNotesLeft -= 1;
      }
      if (TripletMode) {
        timeLeft -= 2*lengthn; // clears out the time of all 3 triplets at once
        continue;
      }
      TripletMode = false;

      //Dotted?
      boolean singleDot = randChance(singledotChance);
      boolean doubleDot = randChance(doubledotChance);
      if (singleDot && doubleDot) doubleDot = false; //?

      if (singleDot) {
        if (lengthn < noteLen[3]) continue;
        lengthn *= 1.5;
        if (timeLeft < lengthn) continue;
      } else if (doubleDot) {
        if (lengthn < noteLen[2]) continue;
        lengthn *= 1.75;
        if (timeLeft < lengthn) continue;
      }
      

      // Finally, adding the note!
      timeLeft -= lengthn;
      if (isPause) {
        Measure[noteNum] = -lengthn; 
        // if it's a pause, it is stored as a negative number
      } else {
        Measure[noteNum] = lengthn;
      }

      noteNum += 1;
    }

    Measure[noteNum] = 0; // will mark the end of the measure
    rhythmReady = true; // now we are ready to move on to the second part
  }


  // Second part of the process (playing the rhythm):
  
  while (rhythmReady) {
    Serial.println(' ');
    int sum = 0;
    //ledState = 1;
    
    for (int i = 0; i < 33; i++) {
      if (Measure[i] == 0) {
        break; } 
        
      else {

        //digitalWrite(ledPin, ledState%2); // change with each note
        //ledState += 1;

        Serial.print(int(Measure[i]));
        sum += abs(Measure[i]);
        Serial.print(' ');
     
        //speaker part
        int note = 0;
        if (i == 0 && Measure[i] > 0) {
          // only if it's the first note of the measure
          note = 440; //A4
          
        } else if (Measure[i] > 0) {
          // every other note of the measure
          note = 394; //G4 
        }

        unsigned long int duration = abs(Measure[i]) * beatTime / 1000; //ms


        tone(speaker, note, duration);
        delay(duration);
        noTone(speaker);
      }  
    }

    Serial.print(" = ");
    Serial.println(sum);


    while (buttonState==0) { 
      //Sets the playback in a paused state until the button is pressed    
      if (digitalRead(buttonPin)==LOW) { buttonState=1; }
    }
    
    buttonState=0;   
  }
}
