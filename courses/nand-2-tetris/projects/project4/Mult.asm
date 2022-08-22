// Mahmoud Elnagar - 22/08/2022

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// High-level Code
// R2 = 0
// i = 0
// while (R1-i != 0){
//   R2 = R2 + R0
//   i++
// }
// END


// R2 = 0
  @R2
  M=0
// i = 0
  @i
  M=0
// while (R1-i != 0)
(LOOP)
  @R1
  D=M
  @i
  D=D-M
  @END
  D;JEQ
// R2 = R2 + R0
  @R2
  D=M
  @R0
  D=D+M
  @R2
  M=D
// i++
  @i
  M=M+1
// Loop back
  @LOOP
  0;JMP
// End the program
(END)
  @END
  0;JMP