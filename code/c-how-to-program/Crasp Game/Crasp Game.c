#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// enumeration constants represent game status
enum Status { CONTINUE, WON, LOST };
void lostDialog(void);
void winDialog(void);
int rollDice(void); // function prototype

int main(void)
{
    static int balance = 1000;
    puts("------- GAME STARTED -------");
    printf("your balance : %d\n",balance);
    unsigned int wage = balance +1;
    do{
        printf("%s","Enter wage : ");
        scanf("%u", &wage);
    }while (wage > balance);
   // randomize random number generator using current time
   srand(time(NULL));
   int myPoint; // player must make this point to win
   enum Status gameStatus; // can contain CONTINUE, WON, or LOST
   int sum = rollDice(); // first roll of the dice

   // determine game status based on sum of dice
   switch(sum) {

      // win on first roll
      case 7: // 7 is a winner
      case 11: // 11 is a winner
         gameStatus = WON;
         break;

      // lose on first roll
      case 2: // 2 is a loser
      case 3: // 3 is a loser
      case 12: // 12 is a loser
         gameStatus = LOST;
         break;

      // remember point
      default:
         gameStatus = CONTINUE; // player should keep rolling
         myPoint = sum; // remember the point
         printf("Point is %d\n", myPoint);
         break; // optional
   }

   // while game not complete
   while (CONTINUE == gameStatus) { // player should keep rolling
     /* puts("Press any key to roll");
      while (getchar() == EOF){} // pause for any key*/
      sum = rollDice(); // roll dice again

      // determine game status
      if (sum == myPoint) { // win by making point
         gameStatus = WON;
      }
      else {
         if (7 == sum) { // lose by rolling 7
            gameStatus = LOST;
         }
      }
   }

   // display won or lost message
   if (WON == gameStatus) { // did player win?
        balance += wage ;
      puts("You win");
      winDialog();
   }
   else { // player lost
       balance -= wage ;
      puts("You lost");
      if (balance > 0) {lostDialog();}
   }
   if (balance <= 0){
    puts("your balance is zero, you are OUT!");
   }else{
    printf("Play again (y/n)? ");
    char rec ;
    while ((rec = getchar()) != 'n'){
        if (rec == 'y' || rec == 'Y'){
            main();break;
        }
    }
   }
}
void winDialog(void){
    int r = rand()%3;
    switch(r){
    case 0:
        puts("IT'S YOUR LUCKY DAY");
        break;
    case 1:
        puts("WINNER WINNER CHICKEN DINNER");
        break;
    case 2:
        puts("YOU ARE THE BOSS");
        break;
    }
}
void lostDialog(void){
    int r = rand()%3;
    switch(r){
    case 0:
        puts("YOU LOST, THAT HAPPENS, BUT NEVER GIVE UP");
        break;
    case 1:
        puts("LET THEM LAUGH , IT'S YOUR TURN TO WIN");
        break;
    case 2:
        puts("WINNING IS COMING");
        break;
    }
}
// roll dice, calculate sum and display results
int rollDice(void)
{
   int die1 = 1 + (rand() % 6); // pick random die1 value
   int die2 = 1 + (rand() % 6); // pick random die2 value

   // display results of this roll
   printf("You rolled %d + %d = %d\n", die1, die2, die1 + die2);
   return die1 + die2; // return sum of dice
}
