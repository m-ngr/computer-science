/*
testPalindrome recursive problem.
coded by Mahmoud Elnagar.
*/
#include <stdio.h>
#include <ctype.h>
int testPalindrome(char array[],int i, int size);
int main(void){
    char array [] = "a man a plan a canal panama";
    int size = sizeof(array)/sizeof(array[0]);
    if (testPalindrome(array,0,size-2)){
        printf("\"%s\" is a Palindrome\n",array );
    }else{
        printf("\"%s\" is not a Palindrome\n",array );
    }
}
int testPalindrome(char array[],int i,int f){
    if (array[i] == '\0' || f == 0){
        //base case : last element!
        return 1;
    }else{
        //recursive step.
        if (isalnum(array[i]) && isalnum(array[f])){
            //printf("%c  %c\n",array[i],array[f]);
            return (array[i] == array[f])&&(testPalindrome(array,i + 1, f - 1));//test current and next.
        }else if (isalnum(array[i]) == 0){ //array[i] not an alpha/num.
            return testPalindrome(array, i + 1, f);//test next i with current f.
        }else{
            return testPalindrome(array, i, f - 1);//test current i with next f.
        }
    }
}
