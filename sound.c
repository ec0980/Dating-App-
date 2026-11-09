/*
Akshara Bharath
Sound effects for the terminal version of Love at First Write

How it works:
1. When the program starts, it builds a few short sound files (.wav) in the
   computer's temporary folder. The sounds are generated with math (sine waves),
   so no audio files need to be added to the project.
2. To play a sound, it asks the computer's built-in audio player to play one of
   those files (afplay on Mac, paplay or aplay on Linux, PowerShell on Windows).
3. If no player is found (for example in a cloud editor), it falls back to the
   terminal beep so the program still works.
4. The sound files stay in the temporary folder (about 200 KB in total) and are
   reused the next time the program runs. They are not deleted when the program
   ends, because the last sound may still be playing at that moment.
*/
//^information and overview

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"
//^libraries needed for code

#define SAMPLE_RATE 22050
//^samples per second (enough quality for short chimes, and keeps files small)
#define MAX_NOTES 4
//^most notes any one effect can have
#define PATH_LENGTH 512
//^longest file path that can be stored
#define PI 3.14159265358979
//^the value of pi, used to make sine waves (not every compiler provides it, so it is defined here)

typedef struct
{
  double frequency;
  int startMs;
  int lengthMs;
}
note;
//^one note: how high it is (hertz), when it starts, and how long it lasts

typedef struct
{
  int count;
  note notes[MAX_NOTES];
}
effect;
//^one sound effect is a short list of notes

static const effect effects[SOUND_COUNT] =
{
  {2, {{523.25, 0, 200}, {783.99, 120, 320}}},
  //^START: two quick rising notes when the quiz begins
  {3, {{523.25, 0, 250}, {659.25, 120, 250}, {783.99, 240, 400}}},
  //^RESULTS: rising C, E, G when the matches appear
  {4, {{523.25, 0, 300}, {659.25, 100, 300}, {783.99, 200, 300}, {1046.50, 300, 500}}},
  //^ACCEPT: a happy rising chime
  {2, {{330.00, 0, 180}, {261.63, 130, 260}}},
  //^REJECT: two soft falling notes
  {1, {{196.00, 0, 350}}},
  //^BLOCK: one low tone
  {2, {{440.00, 0, 130}, {440.00, 190, 130}}}
  //^REPORT: two short beeps
};

typedef enum
{
  PLAYER_NONE,
  PLAYER_BEEP,
  PLAYER_MAC,
  PLAYER_PULSE,
  PLAYER_ALSA,
  PLAYER_WINDOWS
}
player;
//^the different ways the program can play a sound

static int enabled = 1;
//^1 = sound on, 0 = sound off

static int ready = 0;
//^1 once soundInit has finished

static player method = PLAYER_NONE;
//^which way of playing sound this computer will use

static char paths[SOUND_COUNT][PATH_LENGTH];
//^where each sound file is saved

static void writeBytes(FILE *file, unsigned long value, int count)
//^writes a number to the file one byte at a time, smallest byte first (the order WAV files need)
{
    for (int i = 0; i < count; i++)
    {
        fputc((int) ((value >> (8 * i)) & 0xFF), file);
    }
}

static int writeWav(const char *path, const effect *e)
//^builds one sound effect and saves it as a .wav file (returns 1 on success, 0 on failure)
{
    int totalMs = 0;
    for (int i = 0; i < e->count; i++)
    {
        int end = e->notes[i].startMs + e->notes[i].lengthMs;
        if (end > totalMs)
        {
            totalMs = end;
        }
    }
    totalMs += 100;
    //^a little silence at the end so the last note is not cut off

    int total = SAMPLE_RATE * totalMs / 1000;
    short *samples = calloc(total, sizeof(short));
    if (samples == NULL)
    {
        return 0;
    }

    for (int n = 0; n < e->count; n++)
    {
        int start = SAMPLE_RATE * e->notes[n].startMs / 1000;
        int length = SAMPLE_RATE * e->notes[n].lengthMs / 1000;
        int attack = SAMPLE_RATE / 100;
        //^a 10 millisecond fade in so the note does not click

        for (int i = 0; i < length && start + i < total; i++)
        {
            double t = (double) i / SAMPLE_RATE;
            double envelope = 1.0;

            if (i < attack)
            {
                envelope = (double) i / attack;
            }
            envelope *= exp(-3.0 * i / length);
            //^the note fades away smoothly, like a music box

            double wave = sin(2.0 * PI * e->notes[n].frequency * t)
                        + 0.3 * sin(4.0 * PI * e->notes[n].frequency * t);
            //^a main tone plus a quieter higher tone to make it sound warmer

            double value = samples[start + i] + 9000.0 * envelope * wave;
            if (value > 32767.0)
            {
                value = 32767.0;
            }
            if (value < -32767.0)
            {
                value = -32767.0;
            }
            samples[start + i] = (short) value;
        }
    }

    FILE *file = fopen(path, "wb");
    if (file == NULL)
    {
        free(samples);
        return 0;
    }

    unsigned long dataBytes = (unsigned long) total * 2;
    fwrite("RIFF", 1, 4, file);
    writeBytes(file, 36 + dataBytes, 4);
    fwrite("WAVEfmt ", 1, 8, file);
    writeBytes(file, 16, 4);
    writeBytes(file, 1, 2);
    writeBytes(file, 1, 2);
    writeBytes(file, SAMPLE_RATE, 4);
    writeBytes(file, SAMPLE_RATE * 2, 4);
    writeBytes(file, 2, 2);
    writeBytes(file, 16, 2);
    fwrite("data", 1, 4, file);
    writeBytes(file, dataBytes, 4);
    //^the standard 44 byte WAV header: mono, 16-bit, at the sample rate above

    for (int i = 0; i < total; i++)
    {
        writeBytes(file, (unsigned short) samples[i], 2);
    }

    int result = (fclose(file) == 0);
    free(samples);
    return result;
}

static int commandExists(const char *name)
//^checks whether a program (like paplay) is installed on a Mac or Linux computer
{
    char command[128];
    snprintf(command, sizeof(command), "command -v %s >/dev/null 2>&1", name);
    return system(command) == 0;
}

void soundInit(void)
{
    if (ready)
    {
        return;
    }
    ready = 1;

#ifdef _WIN32
    method = PLAYER_WINDOWS;
    const char *folder = getenv("TEMP");
#elif defined(__APPLE__)
    method = PLAYER_MAC;
    const char *folder = getenv("TMPDIR");
#else
    const char *folder = getenv("TMPDIR");
    if (commandExists("paplay"))
    {
        method = PLAYER_PULSE;
    }
    else if (commandExists("aplay"))
    {
        method = PLAYER_ALSA;
    }
    else
    {
        method = PLAYER_BEEP;
        //^no audio player found (common in cloud editors), so use the terminal beep
    }
#endif

    if (folder == NULL || folder[0] == '\0')
    {
        folder = "/tmp";
    }

    if (method != PLAYER_BEEP)
    {
        for (int i = 0; i < SOUND_COUNT; i++)
        {
            size_t length = strlen(folder);
            const char *slash = (length > 0 && (folder[length - 1] == '/' || folder[length - 1] == '\\')) ? "" : "/";
            snprintf(paths[i], PATH_LENGTH, "%s%slafw_sound_%i.wav", folder, slash, i);

            if (!writeWav(paths[i], &effects[i]))
            {
                paths[i][0] = '\0';
                method = PLAYER_BEEP;
                //^if a file cannot be saved, fall back to the beep instead of failing
            }
        }
    }
}

void playSound(sound which)
{
    if (!enabled || which < 0 || which >= SOUND_COUNT)
    {
        return;
    }

    if (!ready)
    {
        soundInit();
    }

    char command[PATH_LENGTH + 160];
    command[0] = '\0';

    if (method == PLAYER_BEEP || paths[which][0] == '\0')
    {
        printf("\a");
        fflush(stdout);
        return;
    }

    if (method == PLAYER_MAC)
    {
        snprintf(command, sizeof(command), "afplay \"%s\" >/dev/null 2>&1 &", paths[which]);
    }
    else if (method == PLAYER_PULSE)
    {
        snprintf(command, sizeof(command), "paplay \"%s\" >/dev/null 2>&1 &", paths[which]);
    }
    else if (method == PLAYER_ALSA)
    {
        snprintf(command, sizeof(command), "aplay -q \"%s\" >/dev/null 2>&1 &", paths[which]);
    }
    else if (method == PLAYER_WINDOWS)
    {
        snprintf(command, sizeof(command),
                 "start /b \"\" powershell -NoProfile -Command \"(New-Object Media.SoundPlayer '%s').PlaySync()\" >nul 2>&1",
                 paths[which]);
    }

    if (command[0] != '\0')
    {
        system(command);
        //^the & (or start /b) lets the program keep going while the sound plays
    }
}

int soundToggle(void)
{
    enabled = !enabled;
    return enabled;
}

int soundIsOn(void)
{
    return enabled;
}
