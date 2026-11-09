/*
Jo Yi Tham
Akshara Bharath
Emma Cai
Madison Tran
Fiona Mullins
Commander Schenk
4 November 2026
Period 3
Compatibility Quiz
*/
//^information (name, teacher, date, period, project name)

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "security.h"
#include "sound.h"
//^libraries needed for code

#define PRIVACY_FILE "privacy.dat"
//^file where privacy settings and the block list are saved so they persist between sessions
#define REPORT_FILE "reports.txt"
//^file where safety reports are logged
#define MAX_REASON 100
//^longest report reason that will be saved

void defaultPrivacy(privacy *p)
//^resets visibility settings to shown and clears the block list
{
    p->showBio = 1;
    p->showTraits = 1;
    p->showInterests = 1;
    p->showCompatibility = 1;

    for (int i = 0; i < MAX_PROFILES; i++)
    {
        p->blocked[i] = 0;
    }
}

int savePrivacy(privacy *p, int count)
//^writes settings to a temporary file first, then swaps it in so a crash can't corrupt the saved file
{
    char tempName[] = PRIVACY_FILE ".tmp";

    FILE *file = fopen(tempName, "w");
    if (file == NULL)
    {
        printf("Warning: could not save your privacy settings.\n");
        return 0;
    }

    fprintf(file, "%d %d %d %d\n", p->showBio, p->showTraits, p->showInterests, p->showCompatibility);

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d ", p->blocked[i]);
    }
    fprintf(file, "\n");

    if (fclose(file) != 0)
    {
        printf("Warning: could not save your privacy settings.\n");
        remove(tempName);
        return 0;
    }

    if (rename(tempName, PRIVACY_FILE) != 0)
    {
        printf("Warning: could not save your privacy settings.\n");
        remove(tempName);
        return 0;
    }

    return 1;
}

int loadPrivacy(privacy *p, int count)
//^reads settings from disk and checks every value before using it
{
    FILE *file = fopen(PRIVACY_FILE, "r");
    if (file == NULL)
    {
        return 0;
        //^no saved file yet (first run)
    }

    privacy loaded;
    defaultPrivacy(&loaded);
    //^starts from clean defaults so unused block-list slots are never left uninitialized
    int values[4];

    if (fscanf(file, "%d %d %d %d", &values[0], &values[1], &values[2], &values[3]) != 4)
    {
        fclose(file);
        printf("Saved privacy settings were damaged, so defaults were restored.\n");
        return 0;
    }

    for (int i = 0; i < 4; i++)
    {
        if (values[i] != 0 && values[i] != 1)
        {
            fclose(file);
            printf("Saved privacy settings were damaged, so defaults were restored.\n");
            return 0;
        }
    }

    loaded.showBio = values[0];
    loaded.showTraits = values[1];
    loaded.showInterests = values[2];
    loaded.showCompatibility = values[3];

    for (int i = 0; i < count; i++)
    {
        int flag;
        if (fscanf(file, "%d", &flag) != 1 || (flag != 0 && flag != 1))
        {
            fclose(file);
            printf("Saved privacy settings were damaged, so defaults were restored.\n");
            return 0;
        }
        loaded.blocked[i] = flag;
    }

    fclose(file);
    *p = loaded;
    //^only copies the data over after everything has been checked
    return 1;
}

void blockCharacter(privacy *p, string names[], int count, int index)
//^blocks one character and saves the change right away
{
    if (index < 0 || index >= count)
    {
        return;
        //^ignores an invalid position
    }

    p->blocked[index] = 1;
    savePrivacy(p, count);
    printf("\n%s has been blocked. They will not appear in your matches again.\n", names[index]);
}

void reportCharacter(privacy *p, string names[], int count, int index)
//^logs a safety report about a character, then blocks them
{
    if (index < 0 || index >= count)
    {
        return;
    }

    string reason = get_string("Briefly describe why you are reporting this profile: ");

    char cleaned[MAX_REASON + 1];
    int length = 0;

    if (reason != NULL)
    {
        for (int i = 0; reason[i] != '\0' && length < MAX_REASON; i++)
        {
            if (reason[i] >= 32 && reason[i] != 127)
            {
                cleaned[length] = reason[i];
                length++;
                //^keeps only printable characters and caps the length
            }
        }
    }
    cleaned[length] = '\0';

    if (length == 0)
    {
        strcpy(cleaned, "No reason given");
    }

    FILE *file = fopen(REPORT_FILE, "a");
    if (file == NULL)
    {
        printf("Warning: your report could not be saved.\n");
    }
    else
    {
        fprintf(file, "Character: %s | Reason: %s\n", names[index], cleaned);
        fclose(file);
        printf("\nThank you. Your report about %s has been recorded.\n", names[index]);
    }

    if (reason != NULL)
    {
        free(reason);
    }

    blockCharacter(p, names, count, index);
    //^reported characters are blocked automatically for the user's comfort
}

int startMenu(privacy *p, string names[], int count)
//^start menu: stays here until the user chooses to begin the quiz (returns 1) or quit (returns 0)
{
    int choice = 0;

    while (choice != 1)
    {
        printf("\n===== LOVE AT FIRST WRITE =====\n");
        printf("1: Take the quiz\n");
        printf("2: Privacy and safety settings\n");
        printf("3: Privacy notice\n");
        printf("4: Sound: %s\n", soundIsOn() ? "ON" : "OFF");
        printf("5: Quit\n");

        choice = get_int("Choose an option (1-5): ");

        if (choice == 2)
        {
            privacyMenu(p, names, count);
        }
        else if (choice == 3)
        {
            showPrivacyNotice();
        }
        else if (choice == 4)
        {
            soundToggle();
            //^switches sound effects on or off
        }
        else if (choice == 5)
        {
            printf("\nGoodbye!\n");
            return 0;
        }
        else if (choice != 1)
        {
            printf("Invalid choice. Please enter 1, 2, 3, 4, or 5.\n");
        }
    }

    return 1;
}

char getDecision(void)
//^asks until the user enters a valid letter, then returns it in capital form
{
    char decision;

    do
    {
        decision = get_char("Enter A to accept, R to reject, B to block, or F to report: ");

        if (decision >= 'a' && decision <= 'z')
        {
            decision = decision - 'a' + 'A';
            //^turns a lowercase letter into a capital
        }

        if (decision != 'A' && decision != 'R' && decision != 'B' && decision != 'F')
        {
            printf("Invalid response. Please enter A, R, B, or F.\n");
        }
    }
    while (decision != 'A' && decision != 'R' && decision != 'B' && decision != 'F');

    return decision;
}

void showPrivacyNotice(void)
//^short, plain-language explanation of what the app does with information
{
    printf("\n===== PRIVACY NOTICE =====\n");
    printf("- Your quiz answers are used only to calculate matches during this session.\n");
    printf("- Your answers are not saved to disk.\n");
    printf("- Only your visibility settings and block list are saved, in %s.\n", PRIVACY_FILE);
    printf("- Reports are saved in %s so they can be reviewed.\n", REPORT_FILE);
    printf("- You can block or report any profile, and change settings at any time.\n");
    printf("==========================\n");
}

void privacyMenu(privacy *p, string names[], int count)
//^lets the user change visibility settings and manage the block list
{
    int choice = 0;

    while (choice != 8)
    {
        printf("\n===== PRIVACY AND SAFETY =====\n");
        printf("1: Biography on profiles:       %s\n", p->showBio ? "SHOWN" : "HIDDEN");
        printf("2: Personality traits:          %s\n", p->showTraits ? "SHOWN" : "HIDDEN");
        printf("3: Interests:                   %s\n", p->showInterests ? "SHOWN" : "HIDDEN");
        printf("4: Compatibility percentages:   %s\n", p->showCompatibility ? "SHOWN" : "HIDDEN");
        printf("5: View blocked characters\n");
        printf("6: Unblock a character\n");
        printf("7: Reset visibility settings\n");
        printf("8: Back\n");
        printf("(Choose 1-4 to switch a setting between shown and hidden.)\n");

        choice = get_int("Choose an option (1-8): ");

        if (choice == 1)
        {
            p->showBio = !p->showBio;
            savePrivacy(p, count);
        }
        else if (choice == 2)
        {
            p->showTraits = !p->showTraits;
            savePrivacy(p, count);
        }
        else if (choice == 3)
        {
            p->showInterests = !p->showInterests;
            savePrivacy(p, count);
        }
        else if (choice == 4)
        {
            p->showCompatibility = !p->showCompatibility;
            savePrivacy(p, count);
        }
        else if (choice == 5 || choice == 6)
        {
            int blockedCount = 0;

            printf("\nBlocked characters:\n");
            for (int i = 0; i < count; i++)
            {
                if (p->blocked[i])
                {
                    printf("%2i. %s\n", i + 1, names[i]);
                    blockedCount++;
                }
            }

            if (blockedCount == 0)
            {
                printf("You have not blocked anyone.\n");
            }
            else if (choice == 6)
            {
                int number = get_int("Enter the number to unblock (0 to cancel): ");

                if (number >= 1 && number <= count && p->blocked[number - 1])
                {
                    p->blocked[number - 1] = 0;
                    savePrivacy(p, count);
                    printf("%s has been unblocked.\n", names[number - 1]);
                }
                else if (number != 0)
                {
                    printf("That number is not on your blocked list.\n");
                }
            }
        }
        else if (choice == 7)
        {
            p->showBio = 1;
            p->showTraits = 1;
            p->showInterests = 1;
            p->showCompatibility = 1;
            //^resets visibility only; the block list is kept
            savePrivacy(p, count);
            printf("Visibility settings were reset.\n");
        }
        else if (choice != 8)
        {
            printf("Invalid choice. Please enter a number from 1 to 8.\n");
        }
    }
}
