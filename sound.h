/*
Akshara Bharath
Sound effects for the terminal version of Love at First Write
*/
//^information (name, project)

#ifndef SOUND_H
#define SOUND_H
//^header guard so this file is only included once

typedef enum
{
  SOUND_START,
  SOUND_RESULTS,
  SOUND_ACCEPT,
  SOUND_REJECT,
  SOUND_BLOCK,
  SOUND_REPORT,
  SOUND_COUNT
}
sound;
//^the list of sound effects (SOUND_COUNT is just the total number of sounds)

void soundInit(void);
//^creates the sound files and finds a way to play them; call once when the program starts

void playSound(sound which);
//^plays one sound effect (does nothing if sound is turned off)

int soundToggle(void);
//^switches sound on or off and returns the new state (1 = on, 0 = off)

int soundIsOn(void);
//^returns 1 if sound is on, 0 if it is off

#endif
