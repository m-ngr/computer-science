#include <stdio.h>
#define DISKS 3
/* Hanoi Tower Problem
   1- using recursive solution.
   2- using iterative solution.
   Coded by Mahmoud Adel Elnagar.
*/
/* Hanoi recursive algorithm.
# the base case is move one disk from pole to another pole.
1. move (n-1) top disks from source pole to auxiliary pole.(recursively)
2. move the last disk from source pole to destination pole.
3. move (n-1) disks from auxiliary pole to destination pole.(recursively)
*/
/* Hanoi iterative algorithm.
1. Calculate the total number of moves required : [totalMoves = pow(2, n) -1].
2. If number of disks (n) is even then interchange destination pole and auxiliary pole.
3. for i = 1 to total number of moves:
     if i%3 == 1:
        legal movement top disk between source pole and destination pole (either ways)
     if i%3 == 2:
        legal movement top disk between source pole and auxiliary pole (either ways)
     if i%3 == 0:
        legal movement top disk between auxiliary pole and destination pole (either ways)
*/
void hanoi(int disks,char source,char destination ,char auxiliary);
void hanoiLoop(int disks,char source,char destination ,char auxiliary);
void legalMove(int pole1[],int pole2[],int disks,char pole1Name,char pole2Name);
void moveDisk(int sender[], int receiver[],int diskIndex,char senderName,char receiverName);
int power (int x, int n);

int main(void){
    puts("---------------------------------------");
    printf("Hanoi recursive solution for %d disks : \n",DISKS);
    puts("---------------------------------------");
    hanoi(DISKS,'A','C','B');
    puts("---------------------------------------");
    printf("Hanoi iterative solution for %d disks : \n",DISKS);
    puts("---------------------------------------");
    hanoiLoop(DISKS,'A','C','B');
    puts("---------------------------------------");
}
void hanoiLoop(int disks,char source,char destination ,char auxiliary){
    int totalMoves = power(2,disks)- 1;
    if (disks % 2 == 0) {
        //if disks are even swap temp & to
        char hold = auxiliary;
        auxiliary = destination;
        destination = hold;
    }
    //create array for each pole
    int src[disks];
    int dest[disks];
    int aux[disks];
    for (int i = 0; i < disks ; i++){
        //set all disks in source array , empty other two arrays
        src[i]= 1;
        dest[i] = 0;
        aux[i]=0;
    }
    for (int i = 1 ; i <= totalMoves ; i++){
        //apply the algorithm (3)
        if (i % 3 == 1){legalMove(src,dest,disks,source,destination);}
        if (i % 3 == 2){legalMove(src,aux,disks,source,auxiliary);}
        if (i % 3 == 0){legalMove(dest,aux,disks,destination,auxiliary);}
    }
}
void legalMove(int pole1[],int pole2[],int disks,char pole1Name,char pole2Name){
    //determine the legal move, the sender pole and receiver pole.
    for (int i = 0 ; i < disks; i++){
        //check for the top disks in pole1 or pole2.
        if (pole1[i] != 0){
            //if disk exist move from pole1 to pole2 and exit.
            moveDisk(pole1,pole2,i,pole1Name,pole2Name);
            break;
        }else if (pole2[i] != 0){
            //if disk exist move from pole2 to pole1 and exit.
            moveDisk(pole2,pole1,i,pole2Name,pole1Name);
            break;
        }
    }
}
void moveDisk(int sender[], int receiver[],int diskIndex,char senderName,char receiverName){
    //determine which disk is to be moved (rule: bigger disks can't be placed above smaller ones).
    if (diskIndex == 0){
        //smallest disk can be placed any where , so move it.
        sender[diskIndex] = 0; //remove from the sender.
        receiver[diskIndex] = 1;  //move to the receiver.
        printf("%c -> %c\n",senderName,receiverName); //print the instruction.
    }else{
        //bigger disks can't be placed above smaller ones.
        int move = 1;
        //check that the receiver pole doesn't have any smaller disks placed in it.
        for (int j = 0 ; j <= diskIndex ; j++){
            // if any smaller disk exist, cancel the moving operation.
            if (receiver[j]!=0){move = 0;}
        }
        //if no smaller disk exist then move.
        if (move == 1){
            sender[diskIndex] = 0; //remove from the sender.
            receiver[diskIndex] = 1;  //move to the receiver.
            printf("%c -> %c\n",senderName,receiverName); //print the instruction.
        }
    }
}
int power (int x, int n){
    int result = 1;
    for (int i = 1; i <= n ; i++){
        result*=x;
    }
    return result;
}
void hanoi(int disks,char source,char destination ,char auxiliary){
    if ( disks == 1){
        //base case: move single disk from one pole to another.
        printf("%c -> %c\n",source,destination);
    }else{
        //move (n-1) top disks from source pole to auxiliary pole.
        hanoi(disks-1,source,auxiliary,destination);
        //move the last disk from source pole to destination pole.
        printf("%c -> %c\n",source,destination);
        //move (n-1) disks from auxiliary pole to destination pole.
        hanoi(disks-1,auxiliary,destination,source);
    }
}
