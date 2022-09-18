// push argument 1
@ARG
A=M+1
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 1
@SP
M=M-1
@SP
A=M
D=M
@THAT
M=D
// push constant 0
@SP
A=M
M=0
@SP
M=M+1
// pop that 0
@SP
M=M-1
@THAT
D=M
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// push constant 1
@SP
A=M
M=1
@SP
M=M+1
// pop that 1
@SP
M=M-1
@THAT
D=M+1
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// push argument 0
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
@SP
A=M
D=M
@SP
M=M-1
@SP
A=M
A=M
D=A-D
@SP
A=M
M=D
@SP
M=M+1
// pop argument 0
@SP
M=M-1
@ARG
D=M
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// label MAIN_LOOP_START
(global$MAIN_LOOP_START)
// push argument 0
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto COMPUTE_ELEMENT
@SP
M=M-1
@SP
A=M
D=M
@global$COMPUTE_ELEMENT
D;JNE
// goto END_PROGRAM
@global$END_PROGRAM
0;JMP
// label COMPUTE_ELEMENT
(global$COMPUTE_ELEMENT)
// push that 0
@THAT
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// push that 1
@THAT
A=M+1
D=M
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
M=M-1
@SP
A=M
D=M
@SP
M=M-1
@SP
A=M
A=M
D=D+A
@SP
A=M
M=D
@SP
M=M+1
// pop that 2
@SP
M=M-1
@2
D=A
@THAT
D=D+M
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// push pointer 1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@SP
A=M
M=1
@SP
M=M+1
// add
@SP
M=M-1
@SP
A=M
D=M
@SP
M=M-1
@SP
A=M
A=M
D=D+A
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 1
@SP
M=M-1
@SP
A=M
D=M
@THAT
M=D
// push argument 0
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@SP
A=M
M=1
@SP
M=M+1
// sub
@SP
M=M-1
@SP
A=M
D=M
@SP
M=M-1
@SP
A=M
A=M
D=A-D
@SP
A=M
M=D
@SP
M=M+1
// pop argument 0
@SP
M=M-1
@ARG
D=M
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// goto MAIN_LOOP_START
@global$MAIN_LOOP_START
0;JMP
// label END_PROGRAM
(global$END_PROGRAM)
@END_PROGRAM
(END_PROGRAM)
0;JMP