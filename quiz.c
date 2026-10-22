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
#include <string.h>
//^libraries needed for code

int getAnswer(int questionNumber, string question);
//^defining functions early to be used later

int main(int argc, char *argv[1])
//^main method
{
  int questionNumber = 20;
  //^defines an integer used to store the number of questions
  int answers[questionNumber] = [];
  //^defines an integer array to hold all the users answers (currently empty)
  string questions[questionNumber] =
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

  for(int i = 0; i < 20; i++)
  //^for loop used to navigate through the 20 questions in the string array
  {
      answers[i] = getAnswer(i + 1, questions[i]);
      //^calls the getAnswer function and uses the return from it as the corresponding element in the answer array
  }

  printf("\nAll responses have been recorded! Thank you!\n");
  //^thank you message to the user signaling the quiz is complete

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
    answer = get_int(Enter your Answer (1-5)!: );
    //^answer variable redefined to a get_int to prompt the user
  }
  while (answer < 1 || answer > 5)
  //^do while loop to continue asking for an answer until the user enters an integer between 1 and 5

  return answer;
  //returns the user's answer to be inputted into the array in main
}
