// push constant 0
@SP
A=M
M=0
@SP
M=M+1
// pop local 0
@SP
M=M-1
@LCL
D=M
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
// label LOOP_START
(global$LOOP_START)
// push argument 0
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// push local 0
@LCL
A=M
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
// pop local 0
@SP
M=M-1
@LCL
D=M
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
// push argument 0
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto LOOP_START
@SP
M=M-1
@SP
A=M
D=M
@global$LOOP_START
D;JNE
// push local 0
@LCL
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@END_PROGRAM
(END_PROGRAM)
0;JMP