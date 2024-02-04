#ifndef PRISONERS_DILEMMA_DEFINITIONS_H
#define PRISONERS_DILEMMA_DEFINITIONS_H

#include <set>
#include <string>

#define COOPERATE 0
#define DEFECT 1

#define DETAILED 2
#define FAST 3
#define TOURNAMENT 4

#define RANDOM 5
#define REPEAT 6
#define SWITCH 7

#define COOPERATE_ALL 8
#define DEFECT_ALL 9
#define SUCCESSFUL_ROUND 10
#define NEUTRAL_ROUND 11
#define UNSUCCESSFUL_ROUND 12

#define SUCCESS 13
#define EVENT 14
#define SETBACK 15

typedef struct MyChoice {
    char decision;
    char round_info;
} MyChoice;

typedef struct Memory {
    MyChoice my_choice;
    std::array<char, 2> others_choices;
} Memory;

typedef struct Behavior {
    char condition;
    char return_d;
} Behavior;

typedef struct Strategies {
    std::string name;
    double probability;
} strategies;

#endif
