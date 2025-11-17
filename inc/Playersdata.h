#ifndef PLAYERDATA_H
#define PLAYERDATA_H

typedef enum {
    AFGHANISTAN = 1,
    AUSTRALIA,
    BANGLADESH,
    ENGLAND,
    INDIA,
    NEWZEALAND,
    PAKISTAN,
    SOUTHAFRICA,
    SRILANKA,
    WESTINDIES
} TeamID;

typedef struct PlayerNode {
    int playerId;
    char name[50];
    char teamName[50];
    char role[30];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    TeamID teamId;
    float performanceIndex;
    struct PlayerNode* next;
    struct PlayerNode* prev;
} PlayerNode;

#endif
