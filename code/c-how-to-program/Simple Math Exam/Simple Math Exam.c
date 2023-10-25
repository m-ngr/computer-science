#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUESTIONS 10
#define MIN_GRADE 75.0

enum operation {ADD,SUBTRACT,MULTIPLY,DIVIDE,MIX};

int questionAnswer(size_t level, enum operation examType);
void correctAnswer(void);
void wrongAnswer(void);
void printExamHeader(size_t level, enum operation examType);

int main (void){
    srand(time(NULL));
    float totalGrades = 0.0;
    puts("---------- EXAM STARTED ----------");
    puts("Available Exam Types:");
    puts("1.Addition\t2.Subtraction\t3.Multiplication");
    puts("4.Division\t5.All");
    //choose Exam Type
    enum operation examType = MIX;
    do{
        printf("%s","Choose your exam type: ");
        scanf("%u",&examType);
        examType--;
    }while (examType > 5 || examType < 0);
    puts("----------------------------------");
    //Start Exam
    for (size_t level = 1 ; level <= 100 ; level*=10){
        printExamHeader(level,examType);
        unsigned int points = 0;
        //Start Exam at Selected level.
        for (size_t i = 1 ; i <= QUESTIONS ; i++){
            printf("Q.%d: ",i);
            int ans = questionAnswer(level,examType); //This function prints the question
            printf("%s","Answer: ");
            int x ;
            scanf("%d", &x);
            //if answer is correct at first time give a point to the student.
            if (ans == x){
                points++;
                correctAnswer(); //tell the student answer is correct.
            }else{
                wrongAnswer(); //tell the student answer is wrong and try again.
            }
            //repeat until the correct answer is Entered but don't give the student any points.
            while (ans != x){
                scanf("%d", &x);
                if (ans == x){
                    correctAnswer(); //tell the student answer is correct.
                }else {
                    wrongAnswer(); //tell the student answer is wrong and try again.
                }
            }
            puts("----------------------------------");
        }//Next Question.
        //Calculate results.
        float percent = (float)points / QUESTIONS * 100; //Calculate this exam results.
        totalGrades += percent ; //Add this exam results to Total grades.
        //print the results
        printf("%3.2f %% ", percent);
        if (percent >= MIN_GRADE){
            puts("PASSED");
            puts("==================================");
        }else{
            puts("FAILED");
            puts("==================================");
            break;
        }
    }//Next Level.
    totalGrades /= 3; // divide by number of exams.
    //print total Grades.
    printf("Total Grades: %3.2f %% ", totalGrades);
    if (totalGrades >= MIN_GRADE){
        puts("PASSED");
    }else{
        puts("FAILED");
    }
    puts("==================================");
}//end of main
//print question and return the correct answer
int questionAnswer(size_t level, enum operation opType){
    unsigned int a,b;
    //if Mixture Type then choose a random operation.
    if (opType == MIX){opType = rand()%4;}
    // random 2 numbers : zero not included : level increase number of digits
    a = 1 + rand()%(10*level);
    b = 1 + rand()%(10*level);
    if (a > 2){a--;}
    if (b > 2){b--;}
    //print question and return answer.
    switch (opType){
    case ADD:
        printf("%d + %d Equals?\n",a,b);
        return a+b;
        break;
    case SUBTRACT:
        printf("%d - %d Equals?\n",a,b);
        return a-b;
        break;
    case MULTIPLY:
        printf("%d * %d Equals?\n",a,b);
        return a*b;
        break;
    case DIVIDE:
        printf("%d / %d ,with remainder of %d Equals?\n",a,b,a%b);
        return a/b;
        break;
    }
}
//print a random comment for correct answers
void correctAnswer(void){
    switch (rand()%4){
case 0:
    puts("Very good!");
    break;
case 1:
    puts("Excellent!");
    break;
case 2:
    puts("Nice work!");
    break;
case 3:
    puts("Keep up the good work!");
    break;
    }
}
//print a random comment for wrong answers
void wrongAnswer(void){
    switch (rand()%4){
case 0:
    printf("%s","No. Please try again: ");
    break;
case 1:
    printf("%s","Wrong. Try once more: ");
    break;
case 2:
    printf("%s","Don't give up: ");
    break;
case 3:
    printf("%s","No. Keep trying: ");
    break;
    }
}
//print Exam header : [type] Exam , Level [x]
void printExamHeader(size_t level, enum operation examType){
    switch (examType){
case ADD:
    printf("%s","Addition Exam , ");
    break;
case SUBTRACT:
    printf("%s","Subtraction Exam , ");
    break;
case MULTIPLY:
    printf("%s","Multiplication Exam , ");
    break;
case DIVIDE:
    printf("%s","Division Exam , ");
    break;
case MIX:
    printf("%s","Mixture Exam , ");
    break;
    }
    switch(level){
case 1:
    printf("%s","Level 1\n");
    break;
case 10:
    printf("%s","Level 2\n");
    break;
case 100:
    printf("%s","Level 3\n");
    break;
    }
    puts("----------------------------------");
}
