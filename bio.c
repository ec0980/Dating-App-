/*
Jo Yi Tham
Akshara Bharath
Emma Cai
Madison Tran
Fiona Mullins
Commander Schenk
21 October 2026
Period 3
Compatibility Quiz
*/
//^information (name, teacher, date, period, project name)

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "security.h"
//^libraries needed for code (security.h holds the privacy and safety features)

#define QUESTIONS 20
//^defines questions as 20 to be used throughout the program (easy to change)
#define CHARACTERS 15
//^defines the number of fictional characters the user can be matched with
#define MAX_DIFFERENCE 4
//^the biggest possible gap on one question (answering 1 when the character answered 5)

typedef struct
{
  string name;
  //^stores the character's name

  string source;
  //^stores where the character is from

  string bio;
  //^stores the character's biography

  string traits;
  //^stores the character's personality traits

  string interests;
  //^stores the character's hobbies and interests

  int answers[QUESTIONS];
  //^stores how the character would answer every question
}
character;
//^defines a character type that holds a name, where they are from, and how THEY would answer each question


float matchPercent(int userAnswers[], int characterAnswers[]);
//^defining functions early to be used later

int getAnswer(int questionNumber, string question);
//^defines a function used to ask questions and validate user input

void showProfile(character c, float compatibility, privacy *p);
//^defines a function used to display detailed character profile information


int main(void)
//^main method
{
  //^defines an integer used to store the number of questions
  int answers[QUESTIONS];
  //^defines an integer array to hold all the users answers (currently empty)
  string questions[QUESTIONS] =
  //^defines a string array that holds all the questions
  {
    "I feel loved when my partner expresses appreciation verbally.",
    "Hearing that my partner is proud of me makes me feel secure.",
    "I feel closest to my partner when we spend uninterrupted time together.",
    "I value meaningful conversations without distractions.",
    "Thoughtful gifts make me feel appreciated.",
    "Small surprises make me feel remembered and valued.",
    "I feel loved when my partner helps me with tasks.",
    "I appreciate actions that make my day easier.",
    "Hugs and hand-holding help me feel connected to my partner.",
    "Physical affection is important in my relationships.",
    "I feel comfortable telling my partner what I need.",
    "I try to understand my partner's perspective during disagreements.",
    "I feel comfortable trusting my partner when we are apart.",
    "I believe healthy relationships require honesty and consistency.",
    "I value maintaining my own hobbies and friendships.",
    "Both partners should have personal space and time alone.",
    "I prefer discussing disagreements calmly rather than avoiding them.",
    "I am willing to compromise when my partner and I disagree.",
    "I feel comfortable sharing my fears and insecurities.",
    "I express my feelings rather than hide what bothers me."
    };

    character characters[CHARACTERS] =
{
    {"Peeta Mellark", "The Hunger Games",
     "A kind and loyal tribute known for his compassion and courage.",
     "Loyal, Caring, Brave",
     "Baking, Painting, Helping Others",
     {5,5, 5,5, 4,5, 5,5, 5,4, 4,5, 5,5, 2,3, 5,5, 5,5}},

    {"Hermione Granger", "Harry Potter",
     "An intelligent witch who values knowledge and friendship.",
     "Smart, Determined, Responsible",
     "Reading, Learning, Problem Solving",
     {4,5, 4,5, 3,3, 4,5, 3,3, 4,4, 4,5, 5,4, 4,3, 3,4}},

    {"Elizabeth Bennet", "Pride and Prejudice",
     "A witty woman who values independence and personal growth.",
     "Independent, Intelligent, Honest",
     "Reading, Socializing, Walking",
     {3,3, 5,5, 2,2, 3,3, 2,2, 5,3, 4,5, 5,4, 3,3, 3,5}},

    {"Mr. Darcy", "Pride and Prejudice",
     "A reserved gentleman who forms deep and meaningful relationships.",
     "Reserved, Loyal, Thoughtful",
     "Reading, Estate Management, Travel",
     {2,3, 4,5, 3,3, 5,5, 2,2, 2,3, 4,5, 4,5, 3,3, 1,2}},

    {"Romeo Montague", "Romeo and Juliet",
     "A passionate romantic willing to do anything for love.",
     "Romantic, Emotional, Impulsive",
     "Poetry, Romance, Adventure",
     {5,4, 5,4, 4,4, 3,2, 5,5, 4,2, 2,4, 1,1, 1,2, 5,5}},

    {"Sherlock Holmes", "Sherlock Holmes Stories",
     "A brilliant detective with unmatched powers of deduction.",
     "Analytical, Logical, Curious",
     "Investigating, Violin, Solving Mysteries",
     {2,2, 3,5, 1,1, 3,4, 1,1, 2,1, 5,5, 5,5, 3,1, 1,1}},

    {"Tony Stark", "Iron Man",
     "A genius inventor who hides a caring heart behind confidence.",
     "Confident, Intelligent, Creative",
     "Technology, Engineering, Innovation",
     {4,5, 2,2, 5,5, 4,4, 3,3, 2,2, 3,3, 5,4, 2,2, 2,1}},

    {"Jack Sparrow", "Pirates of the Caribbean",
     "A clever pirate always looking for his next adventure.",
     "Funny, Clever, Adventurous",
     "Sailing, Exploring, Treasure Hunting",
     {5,4, 2,1, 4,4, 2,2, 4,3, 3,2, 2,1, 5,5, 2,3, 1,1}},

    {"Shrek", "Shrek",
     "An ogre who values loyalty, honesty, and friendship.",
     "Loyal, Honest, Independent",
     "Privacy, Nature, Family",
     {2,2, 4,4, 2,3, 4,4, 3,3, 2,3, 4,5, 5,5, 2,4, 2,3}},

    {"Harry Potter", "Harry Potter",
     "A brave wizard who is deeply loyal to his friends.",
     "Brave, Loyal, Selfless",
     "Quidditch, Adventure, Friendship",
     {3,4, 4,4, 3,3, 4,4, 3,3, 2,4, 4,5, 4,3, 2,4, 2,2}},

    {"Katniss Everdeen", "The Hunger Games",
     "A survivor who fiercely protects the people she loves.",
     "Independent, Brave, Protective",
     "Hunting, Archery, Family",
     {1,2, 3,4, 2,2, 5,5, 2,3, 1,2, 2,4, 5,5, 2,2, 1,1}},

    {"Mario", "Super Mario Bros.",
     "A heroic plumber known for helping others.",
     "Friendly, Courageous, Helpful",
     "Adventure, Sports, Saving Others",
     {3,3, 3,2, 3,4, 5,5, 3,3, 2,4, 5,5, 3,3, 4,4, 3,3}},

    {"Darth Vader", "Star Wars",
     "A powerful but conflicted figure seeking redemption.",
     "Strong, Serious, Determined",
     "Leadership, Strategy, Power",
     {2,4, 2,2, 1,1, 2,2, 1,1, 1,1, 1,3, 4,4, 1,1, 1,2}},

    {"Pikachu", "Pokemon",
     "A cheerful Pokémon known for loyalty and friendship.",
     "Energetic, Friendly, Loyal",
     "Training, Friendship, Adventure",
     {4,4, 5,5, 3,4, 4,4, 4,4, 4,4, 4,5, 2,2, 3,4, 4,5}},

    {"Homer Simpson", "The Simpsons",
     "A humorous father who deeply cares about his family.",
     "Funny, Caring, Relaxed",
     "Eating, Television, Family",
     {4,4, 4,3, 5,5, 2,2, 5,5, 3,3, 4,3, 4,3, 2,4, 4,5}}
};
  //^defines an array of characters and how each one would answer the 20 questions (1-5)
  //^order of answers: words(2), time(2), gifts(2), acts of service(2), touch(2), communication(2), trust(2), independence(2), conflict(2), vulnerability(2)

  string names[CHARACTERS];
  for(int i = 0; i < CHARACTERS; i++)
  {
      names[i] = characters[i].name;
      //^gives security.c a simple list of character names for the block list
  }

  privacy settings;
  defaultPrivacy(&settings);
  if (!loadPrivacy(&settings, CHARACTERS))
  //^loads saved settings and the block list from the last session
  {
      defaultPrivacy(&settings);
      //^if the file is missing or damaged, safely fall back to defaults
  }

  if (!startMenu(&settings, names, CHARACTERS))
  //^start menu with the quiz, privacy settings, privacy notice, and quit options
  {
      return 0;
  }

  for(int i = 0; i < QUESTIONS; i++)
  //^for loop used to navigate through the 20 questions in the string array
  {
      answers[i] = getAnswer(i + 1, questions[i]);
      //^calls the getAnswer function and uses the return from it as the corresponding element in the answer array
  }

  printf("\nAll responses have been recorded! Thank you!\n");
  //^thank you message to the user signaling the quiz is complete

  float scores[CHARACTERS];
  int ranking[CHARACTERS];
  //^arrays to hold each character's match percent and the order to print them in

  int available = 0;
  //^counts how many characters are NOT blocked

  for(int i = 0; i < CHARACTERS; i++)
  //^for loop to calculate the match percent for every character
  {
      scores[i] = matchPercent(answers, characters[i].answers);

      if (!settings.blocked[i])
      {
          ranking[available] = i;
          available++;
          //^only unblocked characters are added to the ranking
      }
  }

  if (available == 0)
  //^if the user has blocked everyone there is nothing to show
  {
      printf("\nEvery character is blocked, so there are no matches to show.\n");
      printf("Unblock someone in Privacy and safety settings to see matches.\n");
      return 0;
  }

  for(int i = 0; i < available - 1; i++)
  //^bubble sort to put the characters in order from best match to worst match
  {
      for(int j = 0; j < available - 1 - i; j++)
      {
          if(scores[ranking[j]] < scores[ranking[j + 1]])
          {
              int temp = ranking[j];
              ranking[j] = ranking[j + 1];
              ranking[j + 1] = temp;
              //^swaps the two characters if the second one is a better match
          }
      }
  }

  printf("\n===== YOUR MATCHES =====\n");
  for(int i = 0; i < available; i++)
  //^prints every unblocked character from best to worst match
  {
      int c = ranking[i];
      if (settings.showCompatibility)
      {
          printf("%2i. %-17s (%s): %.1f%%\n", i + 1, characters[c].name, characters[c].source, scores[c]);
      }
      else
      {
          printf("%2i. %-17s (%s)\n", i + 1, characters[c].name, characters[c].source);
          //^compatibility percent is hidden when the user turned that setting off
      }
  }

  if (settings.showCompatibility)
  {
      printf("\nYour perfect match is %s from %s!\n", characters[ranking[0]].name, characters[ranking[0]].source);
  }
  else
  {
      printf("\nYour top match is %s from %s!\n", characters[ranking[0]].name, characters[ranking[0]].source);
  }
  //^announces the user's top match

  char decision;
  //^creates a character variable to store whether the user enters A, R, B, or F

  int currentMatch = 0;
  //^stores which ranked character is currently being offered

  while (currentMatch < available)
  //^continues until the user accepts a match or runs out of suggestions
  {
      int c = ranking[currentMatch];
      //^gets the character's position from the ranking array

      printf("\n===== MATCH DECISION =====\n");
      printf("Suggested match: %s from %s\n",
             characters[c].name,
             characters[c].source);
      //^prints the current suggested character's name and where they are from

      if (settings.showCompatibility)
      {
          printf("Compatibility: %.1f%%\n", scores[c]);
      }
      //^displays the compatibility percentage unless the user hid it

      decision = getDecision();
      //^asks the user to accept, reject, block, or report (returns A, R, B, or F)

      if (decision == 'A')
      {
          printf("\nYou accepted %s from %s!\n",
                 characters[c].name,
                 characters[c].source);
          //^prints the accepted character

          if (settings.showCompatibility)
          {
              printf("Compatibility: %.1f%%\n", scores[c]);
          }

          printf("Your decision has been recorded.\n");
          //^confirms the user's choice

          printf("\n===== YOUR MATCH PROFILE =====\n");
          //^begins the profile page for the accepted character

          showProfile(characters[c], scores[c], &settings);
          //^displays the accepted character's profile (respecting visibility settings)

          break;
          //^ends the loop because the user accepted the match
      }
      else
      {
          if (decision == 'B')
          {
              blockCharacter(&settings, names, CHARACTERS, c);
              //^blocks the character so they never appear again
          }
          else if (decision == 'F')
          {
              reportCharacter(&settings, names, CHARACTERS, c);
              //^logs a report and blocks the character
          }
          else
          {
              printf("\nYou rejected %s from %s.\n",
                     characters[c].name,
                     characters[c].source);
          }

          currentMatch++;
          //^moves to the next person in the ranking list

          if (currentMatch < available)
          {
              int next = ranking[currentMatch];
              //^finds the original array position of the next-best match

              printf("\nMoving to your next best match...\n");
              printf("Next match: %s from %s\n",
                     characters[next].name,
                     characters[next].source);
          }
      }
  }

  if (currentMatch == available)
  {
      printf("\nNo match was selected.\n");
  }
  //^prints this message if the user went through every available suggestion

  return 0;
  //^returns 0 to terminate the program
}

int getAnswer(int questionNumber, string question)
//^getAnswer method used to ask the question and get a response from the user
{
    printf("\nQuestion %i: %s\n", questionNumber, question);
    //^prints the question number and corresponding question

    printf("1: Strongly Disagree\n");
    printf("2: Disagree\n");
    printf("3: Neutral\n");
    printf("4: Agree\n");
    printf("5: Strongly Agree\n");
    //^prints the meaning of each answer choice

    int answer = 0;
    //^stores the user's answer

    do
    {
        answer = get_int("Enter your Answer (1-5)!: ");
        //^prompts the user for input
    }
    while (answer < 1 || answer > 5);
    //^continues asking until a valid answer is entered

    return answer;
    //^returns the validated answer to main
}

float matchPercent(int userAnswers[], int characterAnswers[])
//^matchPercent method used to compare the user's answers to a character's answers
{
  int totalDifference = 0;
  //^holds how far apart the user and character are across all questions

  for(int i = 0; i < QUESTIONS; i++)
  {
      totalDifference += abs(userAnswers[i] - characterAnswers[i]);
      //^adds the gap on this question (0 = same answer, 4 = complete opposites)
  }

  int maxDifference = QUESTIONS * MAX_DIFFERENCE;
  //^the worst possible total (opposite answers on every question) = 80

  return 100.0 * (maxDifference - totalDifference) / maxDifference;
  //^turns the difference into a percent: 0 difference = 100%, max difference = 0%
}

void showProfile(character c, float compatibility, privacy *p)
//^showProfile method used to display detailed information about a character
{
    printf("\n============================================\n");
    //^visual divider separating profile pages

    printf("NAME: %s\n", c.name);
    //^prints the character's name

    printf("SOURCE: %s\n\n", c.source);
    //^prints where the character is from

    if (p->showBio)
    {
        printf("BIOGRAPHY:\n%s\n\n", c.bio);
        //^prints the character's biography
    }

    if (p->showTraits)
    {
        printf("PERSONALITY TRAITS:\n%s\n\n", c.traits);
        //^prints the character's personality traits
    }

    if (p->showInterests)
    {
        printf("INTERESTS:\n%s\n\n", c.interests);
        //^prints the character's hobbies and interests
    }

    if (p->showCompatibility)
    {
        printf("COMPATIBILITY: %.1f%%\n", compatibility);
        //^prints the compatibility percentage calculated by the matching system
    }

    printf("============================================\n");
    //^closing divider marking the end of the profile page
}
