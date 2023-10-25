/*
    Telephone number word generator
    Coded by: Mahmoud Elnagar.

    ======== CODE TABLE =========
    |   2   ABC  |   3   DEF    |
    |   4   GHI  |   5   JKL    |
    |   6   MNO  |   7   PRS    |
    |   8   TUV  |   9   WXY    |
    =============================

    Input: 7-digit number.
    Output: all possible combinations of Corresponding chars.
*/
#include <stdio.h>
#define WORDS 2187 //3^7

struct names{
    char first[WORDS];
    char second[WORDS];
    char third[WORDS];
    char fourth[WORDS];
    char fifth[WORDS];
    char sixth[WORDS];
    char seventh[WORDS];
};
//Function Prototypes
void fill(char *string, int number, int m);
int power(int x, int n);
//Main function
int main(void){
    //Enter the 7 digits as string
    char num[8] = "";
    printf("%s", "Enter 7 digits (2-9): ");
    fgets(num,8,stdin);
    //convert each digit from char to integer
    int number [7] = {0};
    for (size_t i = 0; i < 7; i++){
        number[i] = num[i] - '0';
    }
    //Generate the names from the number
        //7 digits Number: 2345678
        //Fill Index (m) : 6543210
    struct names code;
    fill(code.first, number[0], 6);
    fill(code.second, number[1], 5);
    fill(code.third, number[2], 4);
    fill(code.fourth, number[3], 3);
    fill(code.fifth, number[4], 2);
    fill(code.sixth, number[5], 1);
    fill(code.seventh, number[6], 0);
    //printing names to the file
    FILE * data;
    if ((data = fopen("Data.txt", "w")) == NULL){
        puts("Error in printing words");
    }else{
        for (int i = 0; i < WORDS; i++){
            fprintf(data,"%c%c%c%c%c%c%c\n", code.first[i],code.second[i],code.third[i],
                    code.fourth[i],code.fifth[i],code.sixth[i],code.seventh[i]);
        }
        puts("File Data.txt generated successfully");
    }
    fclose(data);
}
//Fill structure member with the the chars
void fill(char *string, int number, int m){
    //char set
    char *chars[8] = {"ABC", "DEF", "GHI", "JKL", "MNO", "PRS", "TUV", "WXY"};
    int loops = power(3, 6 - m); // 3^(6-m)
    int repeats = power(3, m);// 3^m
    int index = 0;
    //main loop
    for (size_t i = 1 ; i <= loops; i++){
        //for each char
        for (int charselector = 0; charselector < 3 ; charselector++){
            //place current char for repeats times
            for(size_t n = 1; n <= repeats; n++){
                string[index] = *(chars[number - 2] + charselector);
                index++;
            }
        }

    }
}
//int power function
int power(int x, int n){
    int result = 1;
    for (int i = 1 ; i <= n ; i++){
        result *= x;
    }
    return result;
}
