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
//^libraries needed for code

#define QUESTIONS 20
//^defines questions as 20 to be used throughout the program (easy to change)
#define CHARACTERS 15
//^defines the number of fictional characters the user can be matched with
#define MAX_DIFFERENCE 4
//^the biggest possible gap on one question (answering 1 when the character answered 5)

typedef struct
{
  string name;
  string source;
  int answers[QUESTIONS];
}
character;
//^defines a character type that holds a name, where they are from, and how THEY would answer each question

int getAnswer(int questionNumber, string question);
float matchPercent(int userAnswers[], int characterAnswers[]);
//^defining functions early to be used later

int main(int argc, char *argv[1])
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
  //^defines an array of characters and how each one would answer the 20 questions (1-5)
  //^order of answers: words(2), time(2), gifts(2), acts of service(2), touch(2), communication(2), trust(2), independence(2), conflict(2), vulnerability(2)
  {
    {"Peeta Mellark",   "The Hunger Games",        {5,5, 5,5, 4,5, 5,5, 5,4, 4,5, 5,5, 2,3, 5,5, 5,5}},
    {"Hermione Granger","Harry Potter",            {4,5, 4,5, 3,3, 4,5, 3,3, 4,4, 4,5, 5,4, 4,3, 3,4}},
    {"Elizabeth Bennet","Pride and Prejudice",     {3,3, 5,5, 2,2, 3,3, 2,2, 5,3, 4,5, 5,4, 3,3, 3,5}},
    {"Mr. Darcy",       "Pride and Prejudice",     {2,3, 4,5, 3,3, 5,5, 2,2, 2,3, 4,5, 4,5, 3,3, 1,2}},
    {"Romeo Montague",  "Romeo and Juliet",        {5,4, 5,4, 4,4, 3,2, 5,5, 4,2, 2,4, 1,1, 1,2, 5,5}},
    {"Sherlock Holmes", "Sherlock Holmes stories", {2,2, 3,5, 1,1, 3,4, 1,1, 2,1, 5,5, 5,5, 3,1, 1,1}},
    {"Tony Stark",      "Iron Man",                {4,5, 2,2, 5,5, 4,4, 3,3, 2,2, 3,3, 5,4, 2,2, 2,1}},
    {"Jack Sparrow",    "Pirates of the Caribbean",{5,4, 2,1, 4,4, 2,2, 4,3, 3,2, 2,1, 5,5, 2,3, 1,1}},
    {"Shrek",           "Shrek",                   {2,2, 4,4, 2,3, 4,4, 3,3, 2,3, 4,5, 5,5, 2,4, 2,3}},
    {"Harry Potter",    "Harry Potter",            {3,4, 4,4, 3,3, 4,4, 3,3, 2,4, 4,5, 4,3, 2,4, 2,2}},
    {"Katniss Everdeen","The Hunger Games",        {1,2, 3,4, 2,2, 5,5, 2,3, 1,2, 2,4, 5,5, 2,2, 1,1}},
    {"Mario",           "Super Mario Bros.",       {3,3, 3,2, 3,4, 5,5, 3,3, 2,4, 5,5, 3,3, 4,4, 3,3}},
    {"Darth Vader",     "Star Wars",               {2,4, 2,2, 1,1, 2,2, 1,1, 1,1, 1,3, 4,4, 1,1, 1,2}},
    {"Pikachu",         "Pokemon",                 {4,4, 5,5, 3,4, 4,4, 4,4, 4,4, 4,5, 2,2, 3,4, 4,5}},
    {"Homer Simpson",   "The Simpsons",            {4,4, 4,3, 5,5, 2,2, 5,5, 3,3, 4,3, 4,3, 2,4, 4,5}}
  };

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

  for(int i = 0; i < CHARACTERS; i++)
  //^for loop to calculate the match percent for every character
  {
      scores[i] = matchPercent(answers, characters[i].answers);
      ranking[i] = i;
  }

  for(int i = 0; i < CHARACTERS - 1; i++)
  //^bubble sort to put the characters in order from best match to worst match
  {
      for(int j = 0; j < CHARACTERS - 1 - i; j++)
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
  for(int i = 0; i < CHARACTERS; i++)
  //^prints every character from best to worst match
  {
      int c = ranking[i];
      printf("%2i. %-17s (%s): %.1f%%\n", i + 1, characters[c].name, characters[c].source, scores[c]);
  }

  printf("\nYour perfect match is %s from %s!\n", characters[ranking[0]].name, characters[ranking[0]].source);
  //^announces the user's top match

  return 0;
  //^returns 0 to terminate the program
}


int getAnswer(int questionNumber, string question)
//^getAnswer method used to ask the question and get a response from the user
{
  printf("\nQuestion %i: %s\n", questionNumber, question);
  //^prints the question number and the corresponding (i - 1) question from the string array

  printf("1: Strongly Disagree\n");
  printf("2: Disagree\n");
  printf("3: Neutral\n");
  printf("4: Agree\n");
  printf("5: Strongly Agree\n");
  //^prints out the criteria/meaning of each number

  int answer = 0;
  //^defines an integer variable to hold the user's input (currently 0)

  do
  {
    answer = get_int("Enter your Answer (1-5)!: ");
    //^answer variable redefined to a get_int to prompt the user
  }
  while (answer < 1 || answer > 5);
  //^do while loop to continue asking for an answer until the user enters an integer between 1 and 5

  return answer;
  //returns the user's answer to be inputted into the array in main
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
