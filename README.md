# RandomRhythm

The aim of this Arduino project is to help musicians - aspiring or professional - come up with ideas for their original work or improvisations. For now the functionality allows only rhythm to be generated randomly, although in the future development melody will be generated as well.

# Setup
A speaker attached between pin 9 (though pin number can be changed easily) of the Arduino and GND.
A pushbutton attached between pin 12 (again can be changed easily) of the Arduino and the +5V pin.

# Workflow
The button functions as a kind of a play/pause button - it plays the measure <b>while</b> it is being pressed. If it is being pressed continuously, the playback loops, else - it plays the measure exactly 1 time.

Playing the measure means playing the randomly-genarated notes of the measure, for now the first one having a pitch of A4 and all others being G4. As I hinted in the first paragraph, future functionality plans include generating and playing different melodies in different scales.

If the user wishes to create a new rhythm, he has to press the <b>reset</b> button on the Arduino.

# Random Notes
The principle behind the generating of the notes is simple.
First, the type of note is selected randomly. (possible types: whole, half, quarter, eight, sixteenth)
Then, via hardcoded parameters (future functionality plans user input for the params, possibly) it determines whether the note shall be a <i>pause</i>, <i>singledotted</i> or <i>doubledotted</i>, or whether a <i>triplet sequence</i> of this note type shall be initiated. The paramters are integers with their values being the chances of the described note feature (pause, pause in a triplet, singedot, doubledot, triplet) in percentanges.
This process is repeated until <b>one</b> whole measure of the given time signature is created (the time signature is hardcoded for now, equal to 4/4; in theory the program should work well even with odd time signatures)
The note lengths are stored in an array and when the playback requires a certain note to play, it calculates the length of it using the given tempo in bpm (again hardcoded for now).
<i><b>NB:</b> Although hardcoded, the values of all parameters described above can easily be changed.</i>

# The noDelay branch
The <i>noDelay</i> branch is an attempt to playback the notes without having to use the <b>delay()</b> function. This improves the functionality, by for example allowing a metronome to indicate each beat while playing the notes. The metronome can be a LED attached to pin 13 (or any other).
<i>Currently there is a massive bug which I seem to be unable to solve out, any help woulb be appreciated!</i>
