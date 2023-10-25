/*  SIMPLETRON COMPUTER V2 CODED BY MAHMOUD ELNAGAR.
    Program Example : Add 2 Integers.

    MEMORY
   LOCATION     INSTRUCTION      CODE
     000         READ  a        010007
     001         READ  b        010008
     002         LOAD  a        020007
     003         ADD   b        030008
     004         STORE a        021007
     005         WRITE a        011007
     006         HALT           043000
     007         a              000000
     008         b              000000
*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MEMORYSIZE 1000
#define MAX_VALUE 999999 //(MEMORYSIZE * MEMORYSIZE) - 1

//Instruction set.
//INPUT/OUTPUT
#define READ 10
#define WRITE 11
#define NEWLINE 12
#define READ_STRING 13
#define WRITE_STRING 14
#define READ_FLOAT 15
#define WRITE_FLOAT 16
//REGISTER
#define LOAD 20
#define STORE 21
#define LOAD_FLOAT 22
#define STORE_FLOAT 23
//INTEGER OPERATIONS
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define REMAINDER 34
#define EXPONENTIATION 35
//BRANCHING
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43
//FLOAT OPERATIONS
#define ADD_FLOAT 50
#define SUBTRACT_FLOAT 51
#define DIVIDE_FLOAT 52
#define MULTIPLY_FLOAT 53
#define REMAINDER_FLOAT 54
#define EXPONENTIATION_FLOAT 55

//Functions Prototype.
//MEMORY & REGISTERS.
void printRegisters(int *accumulator, int *accumulatorF,int *instructionCounter, int *instructionRegister, int *operationCode, int *operand);
void printRegistersFile(int *accumulator, int *accumulatorF,int *instructionCounter, int *instructionRegister, int *operationCode, int *operand);
void printMemory(int memory[]);
void printMemoryFile(int memory[]);
int readMemoryFile(int memory[]);
//STRING FUNCTIONS.
int readString(int memory[],int *location, char string[]);
int writeString(int memory[],int *location);
//FLOAT FUNCTIONS.
int readFloat(int memory[], int *location, double value);
void writeFloat(int memory[],int *location);
void splitFloat(double *value, int *intValue, int *floatValue);
void mergeFloat(double *value, int *intValue, int *floatValue);
//EXECUTION FUNCTIONS.
void fetch(int memory[], int *instructionCounter, int *instructionRegister, int *operationCode, int *operand);
int validCode(int memory[], int *accumulator, int *accumulatorF, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand);
int execute(int memory[],int *accumulator, int *accumulatorF, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand);
void executeMath(int memory [], int *accumulator,int *accumulatorF,int *operationCode, int *operand);

//main Function.
int main(void){
    //Initialization.
    int memory[MEMORYSIZE] = {0};
    int accumulator = 0;
    int accumulatorF = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    //Input the program code to memory.
    puts("*** WOLCOME TO SIMPLETRON COMPUTER V 2 ***");
    if (readMemoryFile(memory) != 0){
        //Execute the program.
        puts("*** SIMPLETRON program execution start ***");
        while (execute(memory, &accumulator, &accumulatorF, &instructionCounter, &instructionRegister, &operationCode, &operand)){}
    }
}//End of main function.

//Execute current instruction.
int execute(int memory[],int *accumulator, int *accumulatorF, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    //initialize user input variables.
    int userInput = 0;
    double userFloat = 0.0;
    char userString[MEMORYSIZE] = {0};
    int branched = 0;
    //load current instruction from memory to register.
    fetch(memory, instructionCounter, instructionRegister, operationCode, operand);
    //check if Instruction code is valid.
    if (validCode(memory,accumulator, accumulatorF, instructionCounter, instructionRegister, operationCode, operand) == 0){
        //if not valid print Registers content and Memory content.
        printRegisters(accumulator, accumulatorF,instructionCounter, instructionRegister, operationCode, operand);
        printMemory(memory);
        return 0;//Stop execution.
    }
    //execute
    switch(*operationCode){
    case READ:
        printf("Enter Integer Value: ");
        scanf("%d", &userInput);
        //check for input overflow.
        while (userInput < - MAX_VALUE || userInput > MAX_VALUE){
            //if overflow occurs ask the user to re-enter the value again.
            printf("OVERFLOW! : Integer Value range (%d : %d).\n", - MAX_VALUE, MAX_VALUE);
            printf("Enter Integer Value: ");
            scanf("%d", &userInput);
        }
        //Input integer value to memory.
        memory[*operand] = userInput;
        break;
    case WRITE:
        //Output integer value from memory.
        printf("SIMPLETRON OUTPUT: %d\n",memory[*operand]);
        break;
    case NEWLINE:
        puts("");
        break;
    case READ_STRING:
        //Input string to memory.
        printf("Enter a String: ");
        fgets(userString, MEMORYSIZE, stdin);
        //check for reading string overflow.
        if (readString(memory, operand, userString) == 0){
            //overflow.
            puts("***  ERROR: Reading String Overflow !  ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            printRegisters(accumulator, accumulatorF, instructionCounter, instructionRegister, operationCode, operand);
            printMemory(memory);
            return 0;//Stop execution.
        }
        break;
    case WRITE_STRING:
        //Output a string from memory.
        //check for writing string overflow.
        if (writeString(memory, operand) == 0){
            //overflow.
            puts("***  ERROR: Writing String Overflow !  ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            printRegisters(accumulator, accumulatorF, instructionCounter, instructionRegister, operationCode, operand);
            printMemory(memory);
            return 0;//Stop execution.
        }
        break;
    case READ_FLOAT:
        //Input a float value to memory.
        printf("Enter Float Value: ");
        scanf("%lf", &userFloat);
        //check for float overflow.
        while (readFloat(memory, operand, userFloat) == 0){
            //Overflow: ask the user to re-enter the value again.
            printf("OVERFLOW! : Float Value range (%d.%d : %d.%d).\n", - MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE);
            printf("Enter Float Value: ");
            scanf("%lf", &userFloat);
        }
        break;
    case WRITE_FLOAT:
        //Output a float value from memory.
        writeFloat(memory,operand);
        break;
    case LOAD:
        //load integer value from memory to accumulator.
        *accumulator = memory[*operand];
        *accumulatorF = 0;
        break;
    case STORE:
        //Save integer value from accumulator to memory.
        memory[*operand] = *accumulator;
        break;
    case LOAD_FLOAT:
        //load float value from memory to accumulator.
        *accumulator = memory[*operand];//load integer part.
        ++*operand;//move to next memory location.
        *accumulatorF = memory[*operand];//load fraction part.
        break;
    case STORE_FLOAT:
        //Save float value from accumulator to memory.
        memory[*operand] = *accumulator;//save integer part.
        ++*operand;//move to next memory location.
        memory[*operand] = *accumulatorF;//save fraction part.
        break;
    //Math Operations (integer and float).
    case ADD:
    case SUBTRACT:
    case DIVIDE:
    case MULTIPLY:
    case REMAINDER:
    case EXPONENTIATION:
    case ADD_FLOAT:
    case SUBTRACT_FLOAT:
    case DIVIDE_FLOAT:
    case MULTIPLY_FLOAT:
    case REMAINDER_FLOAT:
    case EXPONENTIATION_FLOAT:
        executeMath(memory,accumulator,accumulatorF,operationCode,operand);
        break;
    //Branching Operations.
    case BRANCH:
        *instructionCounter = *operand;
        branched = 1;//Don't increase instruction counter.
        break;
    case BRANCHNEG:
        //Branch if the value of accumulator is negative.
        if (*accumulator < 0){
            *instructionCounter = *operand;
            branched = 1;//Don't increase instruction counter.
        }
        break;
    case BRANCHZERO:
        //Branch if the value of accumulator is zero.
        if (*accumulator == 0){
            *instructionCounter = *operand;
            branched = 1;//Don't increase instruction counter.
        }
        break;
    case HALT:
        //Program completed all tasks successfully.
        puts("*** SIMPLETRON program execution ended ***");
        printMemoryFile(memory);
        printRegistersFile(accumulator, accumulatorF,instructionCounter, instructionRegister, operationCode, operand);
        return 0;//stop execution.
        break;
    default:
        //Undefined Operation code.
        puts("***  ERROR: Invalid Operation Number!  ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        printRegisters(accumulator, accumulatorF,instructionCounter, instructionRegister, operationCode, operand);
        printMemory(memory);
        return 0;//stop execution.
    }
    //check for accumulator Overflow.
    if (*accumulator > MAX_VALUE || *accumulator < - MAX_VALUE){
        puts("*** ERROR: Overflow in the accumulator ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        printRegisters(accumulator, accumulatorF,instructionCounter, instructionRegister, operationCode, operand);
        printMemory(memory);
        return 0;//Stop execution.
    }
    //if the program did't branch, move to the next instruction.
    if (branched == 0){
        ++*instructionCounter;
    }

    return 1;//Continue execution.
}
//execute math function of integer and float.
void executeMath(int memory [], int *accumulator,int *accumulatorF,int *operationCode, int *operand){
    //initialize
    double xFloat = 0.0,yFloat = 0.0;
    //execute
    switch(*operationCode){
    //Integer operations
    case ADD:
        *accumulator += memory[*operand];
        break;
    case SUBTRACT:
        *accumulator -= memory[*operand];
        break;
    case DIVIDE:
        *accumulator /= memory[*operand];
        break;
    case MULTIPLY:
        *accumulator *= memory[*operand];
        break;
    case REMAINDER:
        *accumulator %= memory[*operand];
        break;
    case EXPONENTIATION:
        *accumulator = round(pow(*accumulator, memory[*operand]));
        break;
    //Float operations
    case ADD_FLOAT:
    case SUBTRACT_FLOAT:
    case DIVIDE_FLOAT:
    case MULTIPLY_FLOAT:
    case REMAINDER_FLOAT:
    case EXPONENTIATION_FLOAT:
        //get the float value from accumulator & accumulatorF to xFloat
        mergeFloat(&xFloat, accumulator, accumulatorF);
        //get the float value from the memory to yFloat
        mergeFloat(&yFloat, &memory[*operand], &memory[*operand + 1]);
        //do the float operation.
        if (*operationCode == ADD_FLOAT){xFloat += yFloat;}
        else if (*operationCode == SUBTRACT_FLOAT){xFloat -= yFloat;}
        else if (*operationCode == DIVIDE_FLOAT){xFloat /= yFloat;}
        else if (*operationCode == MULTIPLY_FLOAT){xFloat *= yFloat;}
        else if (*operationCode == REMAINDER_FLOAT){xFloat = fmod(xFloat,yFloat);}
        else if (*operationCode == EXPONENTIATION_FLOAT){xFloat = pow(xFloat, yFloat);}
        //set the result of float operation to Registers (accumulator & accumulatorF)
        splitFloat(&xFloat, accumulator, accumulatorF);
        break;
    }
}
//determine if all inputs is valid to execute.
int validCode(int memory[], int *accumulator, int *accumulatorF, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    //check for accumulator Overflow
    if (*accumulator > MAX_VALUE || *accumulator < - MAX_VALUE){
        puts("*** ERROR: Overflow in the accumulator ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        return 0;//False: Invalid Code, fetal error.
    }
    //check for accumulatorF Overflow
    if (*accumulatorF > MAX_VALUE || *accumulatorF < - MAX_VALUE){
        puts("***  ERROR: Overflow the accumulatorF  ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        return 0;//False: Invalid Code, fetal error.
    }
    //check for instructionRegister Overflow
    if (*instructionRegister > MAX_VALUE || *instructionRegister < - MAX_VALUE){
        puts("*** ERROR:instructionRegister Overflow ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        return 0;//False: Invalid Code, fetal error.
    }
    //check for current instruction location.
    if (*instructionCounter > MEMORYSIZE - 1){
        puts("***  ERROR: Invalid Memory Location !  ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        return 0;//False: Invalid Code, fetal error.
    }
    //check for current operand memory location.
    if (*operand > MEMORYSIZE - 1){
        puts("***  ERROR: Invalid Memory Location !  ***");
        puts("***  SIMPLETRON Terminated Abnormally  ***");
        return 0;//False: Invalid Code, fetal error.
    }
    //check for errors within operations.
    switch (*operationCode){
        //[FLOAT] check for next operand memory location.
    case READ_FLOAT:
    case WRITE_FLOAT:
    case LOAD_FLOAT:
    case STORE_FLOAT:
    case ADD_FLOAT:
    case SUBTRACT_FLOAT:
    case MULTIPLY_FLOAT:
    case EXPONENTIATION_FLOAT:
        if (*operand + 1 > MEMORYSIZE - 1){
            puts("***  ERROR: Invalid Memory Location !  ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            return 0;//False: Invalid Code, fetal error.
        }
        break;
        //[FLOAT] check for division by zero and next operand memory location.
    case DIVIDE_FLOAT:
    case REMAINDER_FLOAT:
        if (*operand + 1 > MEMORYSIZE - 1){
            puts("***  ERROR: Invalid Memory Location !  ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            return 0;//False: Invalid Code, fetal error.
        }
        if (memory[*operand] == 0 & memory[*operand + 1] == 0){
            puts("*** ERROR: Dividing by Zero is illegal ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            return 0;//False: Invalid Code, fetal error.
        }
        break;
        //[INTEGER] check for division by zero.
    case DIVIDE:
    case REMAINDER:
        if (memory[*operand] == 0){
            puts("*** ERROR: Dividing by Zero is illegal ***");
            puts("***  SIMPLETRON Terminated Abnormally  ***");
            return 0;//False: Invalid Code, fetal error.
        }
        break;
    }//end of switch.
    return 1;//True: Valid Code, ready to execute.
}
//get the next instruction from the memory.
void fetch(int memory[], int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    if (*instructionCounter < MEMORYSIZE){
        //move the current instruction to instruction register.
        *instructionRegister = memory[*instructionCounter];
        //split the instruction to operation code (first 3-digits) and operand(last 3-digits).
        *operationCode = *instructionRegister / MEMORYSIZE;
        *operand = *instructionRegister % MEMORYSIZE;
    }
}
//Operation Functions
//Input a string to the memory.
int readString(int memory [],int *location ,char string[] ){
    //read all char until NULL char is found.
    while(*string != '\0' ){ //&& *string != '\n'){ //to exclude newline.
        //set the char value to memory.
        memory[*location] = (int)*string;
        //get next location.
        ++*location;
        //get next char.
        ++string;
        //check for valid memory location.
        if (*location > MEMORYSIZE - 1){
            return 0;//Error : no enough memory.
            break;
        }
    }
    //set a NULL char at the end of the string.
    memory[*location] = 0;
    return 1;//Reading completed.
}
//Output a string from the memory.
int writeString(int memory[], int *location){
    //print all chars until a NULL char is found
    while(memory[*location] != 0){
        //print current char.
        printf("%c",memory[*location]);
        //get next char.
        ++*location;
        //check for valid memory location.
        if (*location > MEMORYSIZE - 1){
            return 0; //Error : no enough memory.
            break;
        }
    }
    return 1;//Writing completed.
}
//Input a float number to the memory.
int readFloat(int memory[], int *location, double value){
    //Initialize components
    int intValue = 0, floatValue = 0;
    //Split double to its components
    splitFloat(&value, &intValue, &floatValue);
    //check for overflow.
    if (intValue > MAX_VALUE || intValue < - MAX_VALUE){
        return 0; //return Overflow.
    }
    //read the components in two memory slots.
    memory[*location] = intValue;
    ++*location;
    memory[*location] = floatValue;
    return 1; //return True: reading complete.
}
//Output a float number from the memory.
void writeFloat(int memory[], int *location){
    double value = 0.0;
    //merge components from 2 memory slots to a single double variable.
    mergeFloat(&value, &memory[*location], &memory[*location + 1]);
    printf("SIMPLETRON OUTPUT: %.6f\n", value);
}
//Split a double variable to 2 integer variables.
void splitFloat(double *value, int *intValue, int *floatValue){
    *intValue = (int)*value;
    *floatValue = round((*value - *intValue) * (MAX_VALUE + 1));
}
//Merge the component of the float number from 2 integers to a single double variable.
void mergeFloat(double *value, int *intValue, int *floatValue){
    *value = *intValue + ((double)*floatValue / (MAX_VALUE + 1));
}

//Memory & Register Functions
void printRegistersFile(int *accumulator, int *accumulatorF,int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    FILE *registerfile;
    if ((registerfile = fopen("registers.txt", "w")) == NULL){
        puts("*** Error: Failed to Print Registers File ***");
    }else{
        //print Registers value.
        fprintf(registerfile, "\t%-23s%+07d\n","accumulator",*accumulator);
        fprintf(registerfile, "\t%-23s%+07d\n","accumulatorF",*accumulatorF);
        fprintf(registerfile, "\t%-27s%03d\n","instructionCounter",*instructionCounter);
        fprintf(registerfile, "\t%-23s%+07d\n","instructionRegister",*instructionRegister);
        fprintf(registerfile, "\t%-27s%03d\n","operationCode",*operationCode);
        fprintf(registerfile, "\t%-27s%03d\n","operand",*operand);
    }
    fclose(registerfile);
}
void printRegisters(int *accumulator, int *accumulatorF,int *instructionCounter, int *instructionRegister, int *operationCode, int *operand){
    puts("\nREGISTERS : ");
    //print Registers value.
    printf("\t%-23s%+07d\n","accumulator",*accumulator);
    printf("\t%-23s%+07d\n","accumulatorF",*accumulatorF);
    printf("\t%-27s%03d\n","instructionCounter",*instructionCounter);
    printf("\t%-23s%+07d\n","instructionRegister",*instructionRegister);
    printf("\t%-27s%03d\n","operationCode",*operationCode);
    printf("\t%-27s%03d\n","operand",*operand);
    printRegistersFile(accumulator, accumulatorF,instructionCounter, instructionRegister, operationCode, operand);
}
//Read program from file to memory.
int readMemoryFile(int memory[]){
    //Initialize.
    int input = 0;
    int i = 0;
    char filename[30];
    FILE *program;
    puts("***   Loading your program to memory   ***");
    //Enter program File Path.
    printf("%s", "Enter Program File Name: ");
    scanf("%29s", filename);
    //open program file.
    if ((program = fopen(filename, "r"))== NULL){
        puts("***   Failed to read the program file  ***");
        return 0;//Failed
    }else{
        //Read program until user entered the EXIT_CODE or memory is full.
    while (i < MEMORYSIZE && !feof(program)){
        //Read a valid code from user
        fscanf(program, "%d", &input);
        //if EXIT_CODE entered then run the program
        if (input == -(MAX_VALUE + 1)){break;}
        //check for valid code range
        if (input < - MAX_VALUE || input > MAX_VALUE){
            puts("***  SIMPLETRON Program: Code overflow ***");
            printf("Code Overflow at %06d\n", input);
            return 0;//Failed
            break;
        }
        //set the code to memory.
        memory[i] = input;
        //move to next memory slot.
        i++;
    }
    puts("*** SIMPLETRON Program load: completed ***");
    return 1;//Completed
    }
    fclose(program);
}
//Print Memory content to File
void printMemoryFile(int memory[]){
    FILE *memoryfile;
    if((memoryfile = fopen("memory.txt", "w")) == NULL){
        puts("*** Error: Failed to Print Memory File ***");
    }else{
        //Print Header (0 : 9).
        for (size_t i = 0 ; i < 10 ; i++){
            fprintf(memoryfile ,"%9d",i);
        }
        //Print Memory Content.
        for (size_t i = 0 ; i < MEMORYSIZE ; i++){
            //Print 10 Elements per line.
            if (i % 10 == 0){fprintf(memoryfile ,"\n%3d  ",i);}
            fprintf(memoryfile ,"%+07d  ",memory[i]);
        }
    }
    fclose(memoryfile);
}
//Print Memory content (In decimal)
void printMemory(int memory[]){
    printf("\nMEMORY :\n%3s","");
    //Print Header (0 : 9).
    for (size_t i = 0 ; i < 10 ; i++){
        printf("%9d",i);
    }
    //Print Memory Content.
    for (size_t i = 0 ; i < MEMORYSIZE ; i++){
        //Print 10 Elements per line.
        if (i % 10 == 0){printf("\n%3d  ",i);}
        printf("%+07d  ",memory[i]);
    }
    puts("");
    //print memory file
    printMemoryFile(memory);
}
