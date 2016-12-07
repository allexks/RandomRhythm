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
int buttonPin = 12;
int buttonState = 0; //1 means pressed; 0 means not pressed


// Time Signature:
const int beats = 4;
const int beatnote = 4;

// Tempo
const int bpm = 60;
float beatTime = 60000 / bpm; // time of 1 beat in milliseconds

// Chance multipliers (in %)
const int pauseChance = 20;

const int pauseInSequenceChance = 25; //Chance of having a pause in a triplet sequence
const int tripletChance = 15;

const int singledotChance = 20;
const int doubledotChance = 5;


// A list with the length (in beats x 1000)
// of all types of notes
// generated in the setup()
int noteLen[6];


int ledState = HIGH;
int ledPin = 13;

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

  pinMode (buttonPin, INPUT_PULLUP);      // Sets pin 12 as a input for the button
  //digitalWrite (buttonPin, HIGH);  // Activates the pullup resistor of pin 12

  pinMode(ledPin, OUTPUT);
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
      // NEW NOTE

      //What type of note?
      int lengthn = noteLen[random(0, 5)]; //holds length of current note
      if (timeLeft < lengthn) continue;

      //Pause or tone?
      boolean isPause = randChance(pauseChance);

      //Triplet sequence?
      int tripletNotesLeft = 0;
      boolean TripletMode = false;

      if ((2 * lengthn) % 1000 == 0 && timeLeft >= 2 * lengthn) {
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
        timeLeft -= 2 * lengthn; // clears out the time of all 3 triplets at once
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
      Serial.println(Measure[noteNum]);
      noteNum += 1;
    }

    Measure[noteNum] = 0; // will mark the end of the measure
    buttonState = 1; // in order to play the rhythm once intitally
    rhythmReady = true; // now we are ready to move on to the second part
  }


  // Second part of the process (playing the rhythm):

  while (rhythmReady) {
    Serial.println(' ');
    int sum = 0; //for debug purposes only

    int currentNote = 0;
    bool firstNoteNotPlayed = true;
    unsigned long previousMillis = 1; //updates each note
    unsigned long previousBeatMillis = millis(); //updates each beat

    while (buttonState == 1) {

      unsigned long int duration = abs(Measure[currentNote]) * beatTime / 1000; //ms
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= duration || firstNoteNotPlayed) {
        // PLAYS NEXT NOTE OF THE MEASURE
        noTone(speaker);

        if (!firstNoteNotPlayed) {
          currentNote += 1;
        } else {
          firstNoteNotPlayed = false;
          // marks the first beat
          digitalWrite(ledPin, HIGH);
          Serial.println("BEAT");
          previousBeatMillis = currentMillis;
        }

        if (Measure[currentNote] == 0) {
          buttonState = 0;
          digitalWrite(ledPin, LOW);
          previousBeatMillis = currentMillis;
          break;
        }

        Serial.println(Measure[currentNote]);
        Serial.println(currentNote);

        //digitalWrite(ledPin, ledState%2); // change with each note
        //ledState += 1;

        //Serial.print(int(Measure[currentNote]));
        sum += abs(Measure[currentNote]);
        //Serial.print(' ');

        int note = 0;
        if (Measure[currentNote] > 0) {
          //if note is not a pause
          if (currentNote == 0) {
            note = 440; //A4
          } else {
            note = 394; //G4
          }
        }

        tone(speaker, note, abs(Measure[currentNote]) * beatTime / 1000);
        
        previousMillis = currentMillis;
      }


      if (currentMillis - previousBeatMillis >= (long)beatTime) {
        // CHANGES THE STATE OF THE LED ON EACH BEAT LIKE A METRONOME
        
        digitalWrite(ledPin, !digitalRead(ledPin));
        Serial.println("BEAT");
        previousBeatMillis = currentMillis;
      }
    }

    Serial.print(" = ");
    Serial.println(sum);


    while (buttonState == 0) {
      //Sets the playback in a paused state until the button is pressed
      if (digitalRead(buttonPin) == LOW) {
        buttonState = 1;
      }
    }
  }
}
