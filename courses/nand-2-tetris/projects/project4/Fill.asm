// Mahmoud Elnagar - 22/08/2022

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// High-level code
// while(true){
//   M = KBD
//   if (M == 0){
//     v = 0
//   }else{
//     v = -1
//   }
//   // Loop and set all values of screen to v
// }


// endAddr = SCREEN + 8192
  @SCREEN
  D=A
  @8192 // 512 * 256 / 16
  D=D+A
  @endAddr
  M=D
// While (true)
(LOOP)
// D = KBD
  @KBD
  D=M
// if D == 0 => goto WHITE
  @WHITE
  D;JEQ
// else => v = -1 
  @v
  M=-1
  @END_IF
  0;JMP
(WHITE)
// v = 0
  @v
  M=0
(END_IF)

// Loop and set all values of screen to v

// addr = SCREEN
  @SCREEN
  D=A
  @addr
  M=D

//while(endAddr-addr != 0)
(SCREEN_LOOP)
  @endAddr
  D=M
  @addr
  D=D-M
  @LOOP
  D;JEQ
// M[addr] = v
  @v
  D=M
  @addr
  A=M
  M=D
  // addr++
  @addr
  M=M+1
// Screen Loop back
  @SCREEN_LOOP
  0;JMP