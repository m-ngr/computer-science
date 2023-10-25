/*
Five Card Poker Game, Coded by: Mahmoud Elnagar.
    To win get the higher rank of the following:
        8- Flush and Sequence.
        7- Quad cards(4-Cards of same face).
        6- Full house (Triple cards + Pair cards).
        5- Flush (All cards of same suit).
        4- Sequence.
        3- Triple (3-Cards of same face).
        2- Two Pairs (2 x 2-Cards of same face).
        1- Pair (2-Cards of same face).
        0- Highest Card.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHUFFLE 3 //number of shuffles at a time.
#define C_RANK 4 //the highest rank that computer re-draw until passed.
#define SUITS 4
#define FACES 13
#define CARDS 52
#define HANDS 5 //number of cards in the hand
//Function Prototypes
void reDraw(unsigned int deck[][FACES], size_t *card, int hand [][HANDS]);
void deal(unsigned int deck[][FACES], size_t *card, int hand [][HANDS],size_t handColumn);
void shuffle(unsigned int deck[][FACES]);
void sortHand(int hand[][HANDS]);
void printHand(int hand [][HANDS]);
int pairsNumber(int hand[][HANDS], const size_t number, int value[]);
int handRank(int hand[][HANDS]);
int betterHand(int hand1[][HANDS],int hand2[][HANDS]);
int highcard(int hand [] [HANDS]);
int isSequence(int hand[][HANDS]);
int isFlush(int hand[][HANDS]);
int computerPlay(unsigned int deck[][FACES], size_t *card, int hand [][HANDS]);
void resetDeck(unsigned int deck[][FACES]);
void printDeck(unsigned int deck[][FACES]);

//main function
int main(void){
    //initialize
    size_t card = 1; //current card to draw.
    int evaluate = 0; //decide to end the game or not.
    unsigned int deck[SUITS][FACES] = {0}; //deck of cards
    int hand1 [SUITS][HANDS] = {0}; //player hand cards.
    int hand2 [SUITS][HANDS] = {0}; //computer hand cards.
    srand(time(NULL));
    //start the game.
    puts("================================POKER================================");
    puts("\t\tGame has started , Get Ready to play.");
    //shuffle the deck.
    resetDeck(deck);
    shuffle(deck);
    //deal the cards.
    for (size_t i = 0 ; i < HANDS ; i++){
        deal(deck, &card, hand1,i);
        deal(deck, &card, hand2,i);
    }
    sortHand(hand1);//sort to help player.
    sortHand(hand2);//important for automatic play.
    printHand(hand1);//show player cards.
    //play until player/computer evaluates or cards end.
    while(evaluate != 1 && card <= CARDS){
        printf("Enter (r) to re-draw, (e) to evaluate: ");
        //get player input.
        char c ;
        do{
            c = getchar();
        }while(c != 'r' && c != 'R' && c!= 'e'&& c != 'E');
        //process player input.
        if (c == 'r' || c == 'R'){
            //Change 3 cards at maximum
            for(int i = 0 ; i < 3 ; i++){
                printf("Enter number of card to re-draw, (-1) to exit: ");
                int num ;
                //read number of card to change.
                do{
                    scanf("%d", &num);
                }while(num < 1 && num > 6 && num!= -1);
                //change the card.
                if (num != -1){
                    deal(deck, &card, hand1, num - 1);
                }else{
                    //end changing.
                    break;
                }
            }
            //show player the new cards.
            sortHand(hand1);
            printHand(hand1);
        }else{
            //end the game and evaluate.
            evaluate = 1;
            break;
        }
        //computer play.
        evaluate = computerPlay(deck,&card,hand2);
    }
    //End of the Game.
    //Show player cards and computer cards.
    puts("\n/////////////////////////////////////////////////////////////////////\n");
    puts("\t\t\tYOUR CARDS");
    printHand(hand1);
    puts("\t\t\tCOMPUTER CARDS");
    printHand(hand2);
    //Show the Winner.
    if(betterHand(hand1,hand2) == 1){
        puts("\t\t\tYOU WIN!");
    }else if (betterHand(hand1,hand2) == 2){
        puts("\t\t\tYOU LOST!");
    }else{
        puts("\t\t\tNO BODY WINS!");
    }
}
//reset the deck cards.
void resetDeck(unsigned int deck[][FACES]){
    int card = 1;
    for(size_t row = 0 ; row < SUITS ; row++){
        for(size_t column = 0; column < FACES ; column++){
            deck[row][column] = card;
            card++;
        }
    }
}
//Shuffle the cards.
void shuffle(unsigned int deck[][FACES]){
    //swap each card with a random card in the deck.
    for(int i = 1 ; i <= SHUFFLE ; i++){
        for(size_t row = 0 ; row < SUITS ; row++){
            for(size_t column = 0; column < FACES ; column++){
                //get a random card.
                int r = rand()% SUITS;
                int c = rand()% FACES;
                //swap.
                int hold = deck[row][column];
                deck[row][column] = deck[r][c];
                deck[r][c] = hold;
            }
        }
    }
}
//Computer play.
int computerPlay(unsigned int deck[][FACES], size_t *card, int hand [][HANDS]){
    //check if computer is in a good rank.
    if(handRank(hand) > C_RANK){
        return 1;//end the game.
    }else{
        reDraw(deck,card,hand);
        return 0;//re-draw.
    }
}
//determine the occurrence of cards for [number] times.
int pairsNumber(int hand[][HANDS], const size_t number, int value[]){
    int occurrence = 0; //occurrence counter
    int k = 0; //index of array value[].
    //reset value[] if entered.
    if (value != NULL){
        for (size_t m = 0 ; m < 2; m++) {value[m] = -1;}
    }
    //Search for occurrence.
    for (size_t i = 0 ; i < HANDS - 1 ; i++){
        int occur = 1; //any selected card has already occurred once.
        int e1,e2;
        for(size_t j = i + 1 ; j < HANDS; j++){
            for (size_t row  = 0 ; row < SUITS ; row++){
                if (hand[row][i] != -1){e1 = hand[row][i];}
                if (hand[row][j] != -1){e2 = hand[row][j];}
            }
            //if two cards are equal then add 1 to occur.
            if (e1 == e2){occur++;}
        }
        //determine if number if occurrence matches the input.
        if (occur % number == 0){
            occurrence++;
            //add the value of occurrence to array value[]
            if (value != NULL) {value[k++] = e1;}
        }
    }
    return occurrence;
}
//determine who wins.
int betterHand(int hand1[][HANDS],int hand2[][HANDS]){
    if (handRank(hand1) > handRank(hand2)){
        return 1;//player 1 wins.
    }else if (handRank(hand1) < handRank(hand2)){
        return 2; //player 2 wins.
    }else{//both have the same rank.
        //player who got the highest card is the winner.
        if(highcard(hand1) > highcard(hand2)){
            return 1; //player 1 wins.
        }else if (highcard(hand1) < highcard(hand2)){
            return 2; //player 2 wins.
        }else {//both have the same rank and the same high card.
            return 0; // no body wins.
        }
    }
}
//Automatic change the cards for the computer.
void reDraw(unsigned int deck[][FACES], size_t *card, int hand [][HANDS]){
    int pair[2];
    int rank = handRank(hand);
    int nPair ; //number of pairs.
    switch (rank){
    case 8://Highest Rank (Flush and Sequence.)
        //don't re-draw.
        break;
    case 7://4-Cards.
        //change the 5th card which is different.
        pairsNumber(hand,4,pair);
        for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        break;
    case 6://Full house (3-Cards and 2-Cards)
        //change the 2-Cards.
        pairsNumber(hand,3,pair);
        for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        break;
    case 5://Flush (All cards are of the same Suit.)
        //if 3-Cards in hand : change the other 2 cards.
        nPair =  pairsNumber(hand,3,pair); //number of triples
        if (nPair == 1){//if you got 1 triple then change.
            for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        }else{
            //if 2 pairs (2 x 2-cards): change the 5th card.
            nPair = pairsNumber(hand,2,pair);//number of pairs.
            if (nPair == 2){//if you got 2 pairs then change.
                for (size_t column = 0 ; column < HANDS ; column++){
                    for(size_t row = 0 ; row < SUITS ; row++){
                        if ((hand[row][column] != -1) && (hand[row][column] != pair[0])&& (hand[row][column] != pair[1])){
                            deal(deck,card,hand,column);
                            break;
                        }
                    }
                }
            }
        }

        break;
    case 4://Sequence (ex: 2,3,4,5,6)
        //don't re-draw.
        break;
    case 3://3-Cards
        //change the other 2 cards.
        pairsNumber(hand,3,pair);
        for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        break;
    case 2://2 pairs (2 x 2-Cards).
        //change the 5th card.
        pairsNumber(hand,2,pair);
        for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])&& (hand[row][column] != pair[1])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        break;
    case 1://1 pair (2-cards).
        //change the other 3 cards.
        pairsNumber(hand,2,pair);
        for (size_t column = 0 ; column < HANDS ; column++){
            for(size_t row = 0 ; row < SUITS ; row++){
                if ((hand[row][column] != -1) && (hand[row][column] != pair[0])){
                    deal(deck,card,hand,column);
                    break;
                }
            }
        }
        break;
    default://Lowest rank (high Card).
        //change the lowest 3 cards. Hand cards must be sorted.
        for (size_t column = 0 ; column < 3 ; column++){
            deal(deck,card,hand,column);
        }
        break;
    }
    //sort the hand after re-Draw.
    sortHand(hand);
}
//determine hand rank.
int handRank(int hand[][HANDS]){
    //(8)- Flush and Sequence.
    if (isFlush(hand)!= -1 && isSequence(hand)){ return 8;}
    //(7)- Quad cards (4-Cards).
    if (pairsNumber(hand,4,NULL) == 1){ return 7;}
    //(6)- Full house (triple and pair).
    if ((pairsNumber(hand,3,NULL) == 1) && (pairsNumber(hand,2,NULL) == 2)){return 6;}
    //(5)- Flush (All cards are of the same suit)
    if (isFlush(hand)!= -1 ){return 5;}//isFlush returns: [(-1): No Flush][(0:3): Flush at row (0:3)]
    //(4)- Sequence (ex: 2,3,4,5,6)
    if (isSequence(hand)){return 4;}
    //(3)- Triple (3-Cards).
    if (pairsNumber(hand,3,NULL) == 1){ return 3;}
    //(2)- Two Pairs (2 x 2-Cards).
    if (pairsNumber(hand,2,NULL) == 2){ return 2;}
    //(1)- Pair (2-Cards).
    if (pairsNumber(hand,2,NULL) == 1){ return 1;}
    //(0)- High Card.
    return 0;
}
//Determine the highest card in hand.
int highcard(int hand [] [HANDS]){
    //hand must be sorted.
    for (size_t row = 0 ; row < SUITS ; row++){
            if (hand[row][HANDS - 1] != -1 ){
                return hand[row][HANDS - 1];
                break;
            }
    }
}
//Determine if hand cards are in sequence.
int isSequence(int hand[][HANDS]){
    //hand must be sorted.
    //check each two adjacent cards.
    for(size_t j = 0 ; j < HANDS -1; j++){
        int e1,e2;
        for (size_t row = 0 ; row < SUITS ; row++){
            if (hand[row][j] != -1){ e1 = hand[row][j];}
            if (hand[row][j+1] != -1){ e2 = hand[row][j+1];}
        }
        //if each two adjacent cards are not in sequence the return false.
        if (e2 != e1+1){
            return 0;//false: not in sequence
            break;
        }
    }
    return 1;//true: in sequence.
}
//sort hand cards.
void sortHand(int hand[][HANDS]){
    //check each adjacent cards.
    for(size_t loops = 1 ; loops < HANDS ; loops++){
        for (size_t j = 0 ; j < HANDS - 1 ; j++){
            int r1,r2,c1,c2;
            for(size_t row = 0; row < SUITS; row++){
                if (hand[row][j] != -1){r1 = row; c1 = j;}
                if (hand[row][j+1] != -1){r2 = row; c2 = j+1;}
            }
            if (hand[r2][c2] < hand[r1][c1]){
                //swap to cards in appropriate row.
                //save the values of cards.
                int e1 = hand[r1][c1];
                int e2 = hand[r2][c2];
                //remove the cards from current places.
                hand [r1][c1] = -1;
                hand [r2][c2] = -1;
                //Swap each card to the appropriate place.
                //each card stay in the same row but the column only change.
                hand [r2][c1] = e2;
                hand [r1][c2] = e1;
            }
        }
    }
}
//Determine if hand has a flush cards.
int isFlush(int hand [][HANDS]){
    //Flush if all cards at the same row(suit).
    for (size_t row  = 0 ; row < SUITS ; row++){
        int flag = 1;
        for (size_t j = 0 ; j < HANDS ; j++){
            //if this row does not contain all cards then go to next row.
            if(hand[row][j] == -1){
                flag = 0;
                break;
            }
        }
        if (flag == 1){
            return row; //flush at suit.
        }
    }
    return -1; // no flush
}

//print hand cards with their rank.
void printHand(int hand [][HANDS]){
    puts("=====================================================================");
    const char *suit[SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char *face[FACES] =
      {"Ace", "Deuce", "Three", "Four",
       "Five", "Six", "Seven", "Eight",
       "Nine", "Ten", "Jack", "Queen", "King"};
    int printed = 0;
    for(size_t column = 0 ; column < HANDS ; column++){
        for (size_t row = 0 ; row < SUITS ; row++){
            if (hand[row][column] != -1){
                printed++;
                printf("%d.%5s of %-8s%c",column + 1, face[hand[row][column]], suit[row], printed % 3 == 0 ? '\n' : '\t');
            }
        }
    }
    printf("\n----------------------------- Rank (%d) ------------------------------\n", handRank(hand));
    puts("=====================================================================");
}
//draw one card from the deck to hand's column [handColumn]
void deal(unsigned int deck[][FACES], size_t *card, int hand [][HANDS],size_t handColumn){
   // clear hands column
   for (size_t row = 0; row < SUITS; ++row) {
        hand[row][handColumn] = -1;
   }
   //set next card to hands column
   int finalCard = *card + 1; //draw 1 card
   for (; *card < finalCard && *card <= CARDS ; ++*card) {
      for (size_t row = 0; row < SUITS; ++row) {
         for (size_t column = 0; column < FACES; ++column) {
            if (deck[row][column] == *card) {
                hand[row][handColumn] = column;//set the card.
                break;
            }
         }
      }
   }
}

