#include<stdio.h>
#include<stdbool.h> 

#define SZNAME 25
#define POSSIZE 10
#define TEAMSIZE 20
#define SQUAD_SIZE 5
#define NUM_TEAMS 4

// structure for age

typedef struct{
    int year, month, day;
}dob_t;

typedef struct{

    char name[SZNAME];
    int kitnumber;
    char club[SZNAME];
    dob_t age;
    char position[];

}player_t;

// structure for team
typedef struct{

    char name[SZNAME];
    player_t players[SQUAD_SIZE];
    int ActiveSize;
}team_t;

team_t teams[NUM_TEAMS];
int team_count = 0;

void display_menu() {
    printf("Menu:\n");
    printf("1. Enroll Club\n");
    printf("2. Add Player\n");
    printf("3. Search Player\n");
    printf("4. Display Club Statistics\n");
    printf("5. Exit\n");
}

int main(){

    player_t player1;
    
    printf("Please enter the name, kit number, club, age and position: \n");
    scanf("%s %d %s %d %d %s", player1.name, player1.kitnumber, &player1.club, &player1.age.year, &player1.age.month, &player1.age.day, player1.position);
    return 0;
}