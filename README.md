# RandomRhythm

The aim of this Arduino project is to help musicians - aspiring or professional - come up with ideas for their original work or improvisations. For now the functionality allows only rhythm to be generated randomly, although in the future development melody will be generated as well.

# Setup
A speaker attached between pin 9 (though pin number can be changed easily) of the Arduino and GND. <br/>
A pushbutton attached between pin 12 (again can be changed easily) of the Arduino and the +5V pin. <br/>
<em>[optional]:</em> An LED to pin 13 (or change pin value to another if you wish) for the metronome.

# Workflow
The button functions as a kind of a play/pause button - it plays the measure <strong>while</strong> it is being pressed. If it is being pressed continuously, the playback loops, else - it plays the genenerated measure exactly 1 time. <br/> <br/>

Playing the measure means playing the randomly-genarated notes of the measure, for now the first one having a pitch of A4 and all others being G4. As I hinted in the first paragraph, future functionality plans include generating and playing different melodies in different scales. <br/> <br/>

While playing, either the builtin LED or the one attached by the user themselves changes to ON and OFF each beat, thus playing the role of a metronome. <em>(One shall note that the first beat always lights the LED up, so if one changes the time signature to an odd number of beats, say 3/4 or 9/8, the last and first beat will both make the LED light up, though a little blinking can be seen just at the end of the measure because then it is automatically set to be OFF.)</em> <br/> <br/>

If the user wishes to create a new rhythm, he has to press the <strong>reset</strong> button on the Arduino.

# Random Notes
The principle behind the generating of the notes is simple. <br/>
First, the type of note is selected randomly. (possible types: whole, half, quarter, eight, sixteenth) <br/>
Then, via hardcoded parameters (future functionality plans user input for the params, possibly) it determines whether the note shall be a <i>pause</i>, <i>singledotted</i> or <i>doubledotted</i>, or whether a <i>triplet sequence</i> of this note type shall be initiated. The paramters are integers with their values being the chances of the described note feature (pause, pause in a triplet, singedot, doubledot, triplet) in percentanges. <br/>
This process is repeated until <b>one</b> whole measure of the given time signature is created (the time signature is hardcoded for now, equal to 4/4; in theory the program should work well even with odd time signatures) <br/>
The note lengths are stored in an array and when the playback requires a certain note to play, it calculates the length of it using the given tempo in bpm (again hardcoded for now). <br/>
<i><strong>NB:</strong> Although hardcoded, the values of all parameters described above can easily be changed inside the code.</i>
