
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "security.h"
//^reads C privacy settings and returns them in JSON format for the web interface

#define CHARACTER_COUNT 15
#define MAX_REASON 100

// Keep the exact same character order as bio.c.
string names[CHARACTER_COUNT] =
{
    "Peeta Mellark",
    "Hermione Granger",
    "Elizabeth Bennet",
    "Mr. Darcy",
    "Romeo Montague",
    "Sherlock Holmes",
    "Tony Stark",
    "Jack Sparrow",
    "Shrek",
    "Harry Potter",
    "Katniss Everdeen",
    "Mario",
    "Darth Vader",
    "Pikachu",
    "Homer Simpson"
};

int main(int argc, string argv[])
{
    privacy settings;

    defaultPrivacy(&settings);
    loadPrivacy(&settings, CHARACTER_COUNT);

    // LOAD: Return all privacy settings.
    if (argc == 2 && strcmp(argv[1], "load") == 0)
    {
        printf("{\"showBio\":%d,", settings.showBio);
        printf("\"showTraits\":%d,", settings.showTraits);
        printf("\"showInterests\":%d,", settings.showInterests);
        printf("\"showCompatibility\":%d,", settings.showCompatibility);

        printf("\"blocked\":[");
        for (int i = 0; i < CHARACTER_COUNT; i++)
        {
            printf("%d", settings.blocked[i]);

            if (i < CHARACTER_COUNT - 1)
            {
                printf(",");
            }
        }

        printf("]}\n");
        return 0;
    }

    // SET: Change one visibility setting.
    if (argc == 4 && strcmp(argv[1], "set") == 0)
    {
        int value = atoi(argv[3]);

        if (value != 0 && value != 1)
        {
            printf("{\"error\":\"Invalid value\"}\n");
            return 1;
        }

        if (strcmp(argv[2], "showBio") == 0)
        {
            settings.showBio = value;
        }
        else if (strcmp(argv[2], "showTraits") == 0)
        {
            settings.showTraits = value;
        }
        else if (strcmp(argv[2], "showInterests") == 0)
        {
            settings.showInterests = value;
        }
        else if (strcmp(argv[2], "showCompatibility") == 0)
        {
            settings.showCompatibility = value;
        }
        else
        {
            printf("{\"error\":\"Unknown setting\"}\n");
            return 1;
        }

        if (savePrivacy(&settings, CHARACTER_COUNT))
        {
            printf("{\"success\":true}\n");
            return 0;
        }

        printf("{\"error\":\"Could not save settings\"}\n");
        return 1;
    }

    // BLOCK or UNBLOCK: Change a character's blocked status.
    if (argc == 3 &&
        (strcmp(argv[1], "block") == 0 ||
         strcmp(argv[1], "unblock") == 0))
    {
        int index = atoi(argv[2]);

        if (index < 0 || index >= CHARACTER_COUNT)
        {
            printf("{\"error\":\"Invalid character index\"}\n");
            return 1;
        }

        if (strcmp(argv[1], "block") == 0)
        {
            settings.blocked[index] = 1;
        }
        else
        {
            settings.blocked[index] = 0;
        }

        if (savePrivacy(&settings, CHARACTER_COUNT))
        {
            printf("{\"success\":true}\n");
            return 0;
        }

        printf("{\"error\":\"Could not save block list\"}\n");
        return 1;
    }

    // REPORT: Save a report and block the character.
    if (argc == 4 && strcmp(argv[1], "report") == 0)
    {
        int index = atoi(argv[2]);

        if (index < 0 || index >= CHARACTER_COUNT)
        {
            printf("{\"error\":\"Invalid character index\"}\n");
            return 1;
        }

        char reason[MAX_REASON + 1];
        int j = 0;

        for (int i = 0; argv[3][i] != '\0' && j < MAX_REASON; i++)
        {
            unsigned char ch = (unsigned char) argv[3][i];

            if (ch >= 32 && ch <= 126)
            {
                reason[j++] = ch;
            }
        }

        reason[j] = '\0';

        if (j == 0)
        {
            strcpy(reason, "No reason given");
        }

        FILE *file = fopen("reports.txt", "a");

        if (file == NULL)
        {
            printf("{\"error\":\"Could not open reports file\"}\n");
            return 1;
        }

        fprintf(file, "Character: %s | Reason: %s\n",
                names[index], reason);

        fclose(file);

        settings.blocked[index] = 1;

        if (savePrivacy(&settings, CHARACTER_COUNT))
        {
            printf("{\"success\":true}\n");
            return 0;
        }

        printf("{\"error\":\"Could not save privacy settings\"}\n");
        return 1;
    }

    printf("{\"error\":\"Invalid command\"}\n");
    return 1;
}
