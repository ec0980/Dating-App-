/*
Akshara Bharath
Privacy and Safety Features (Gate 5)
Love at First Write
*/
//^information (name, project, gate)

#ifndef SECURITY_H
#define SECURITY_H
//^header guard so this file is only included once

#include <cs50.h>
//^needed for the string type

#define MAX_PROFILES 50
//^the most characters the block list can hold (must be at least CHARACTERS in bio.c)

typedef struct
{
  int showBio;
  int showTraits;
  int showInterests;
  int showCompatibility;
  //^visibility settings: 1 = shown on profile pages, 0 = hidden

  int blocked[MAX_PROFILES];
  //^1 = this character is blocked (hidden from matches and decisions), 0 = not blocked
}
privacy;
//^defines a privacy type that holds the user's visibility settings and block list

void defaultPrivacy(privacy *p);
//^sets every setting back to its default and clears the block list

int loadPrivacy(privacy *p, int count);
//^loads saved settings from disk (returns 1 on success, 0 if the file is missing or invalid)

int savePrivacy(privacy *p, int count);
//^saves settings to disk (returns 1 on success, 0 on failure)

int startMenu(privacy *p, string names[], int count);
//^shows the start menu; returns 1 to begin the quiz or 0 if the user quits

char getDecision(void);
//^asks the user to Accept, Reject, Block, or Report and returns 'A', 'R', 'B', or 'F'

void blockCharacter(privacy *p, string names[], int count, int index);
//^blocks a character and saves the change

void reportCharacter(privacy *p, string names[], int count, int index);
//^logs a report about a character and blocks them

void privacyMenu(privacy *p, string names[], int count);
//^menu for visibility settings and the block list

void showPrivacyNotice(void);
//^prints a short plain-language privacy explanation

#endif
