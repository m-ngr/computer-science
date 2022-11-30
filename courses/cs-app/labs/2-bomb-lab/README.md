# The keys for the bomb

1. Border relations with Canada have never been better.
2. 1 2 4 8 16 32
3. 3 256
4. 0 0
5. ionefg
6. 4 3 2 1 6 5

# Phase 1 Solution

## Annotated Assembly code

```asm
phase_1:
  0x0000000000400ee0 <+0>:	sub    $0x8,%rsp                      # allocate 8 bytes to stack
  0x0000000000400ee4 <+4>:	mov    $0x402400,%esi                 # esi = 0x402400
  0x0000000000400ee9 <+9>:	call   0x401338 <strings_not_equal>   # strings_not_equal(rdi,rsi)
  0x0000000000400eee <+14>:	test   %eax,%eax                      # if (eax == 0)
  0x0000000000400ef0 <+16>:	je     0x400ef7 <phase_1+23>          # then: goto<+23>
  0x0000000000400ef2 <+18>:	call   0x40143a <explode_bomb>        # else: explode_bomb()
  0x0000000000400ef7 <+23>:	add    $0x8,%rsp                      # deallocate 8 bytes from stack
  0x0000000000400efb <+27>:	ret                                   # return
```

## Memory Dump

```asm
# Memory dump at 0x402400:
  0x42 0x6f 0x72 0x64 0x65 0x72 0x20 0x72
  0x65 0x6c 0x61 0x74 0x69 0x6f 0x6e 0x73
  0x20 0x77 0x69 0x74 0x68 0x20 0x43 0x61
  0x6e 0x61 0x64 0x61 0x20 0x68 0x61 0x76
  0x65 0x20 0x6e 0x65 0x76 0x65 0x72 0x20
  0x62 0x65 0x65 0x6e 0x20 0x62 0x65 0x74
  0x74 0x65 0x72 0x2e 0x00
```

## The Key

From the Annotated Assembly code, we can observe that the key is the string stored at memory address `0x402400`.

Key: `Border relations with Canada have never been better.`

# Phase 2 Solution

## Annotated Assembly code

```asm
phase_2:
  0x0000000000400efc <+0>:	push   %rbp                         # save rbp
  0x0000000000400efd <+1>:	push   %rbx                         # save rbx
  0x0000000000400efe <+2>:	sub    $0x28,%rsp                   # allocate 40 bytes to stack
  0x0000000000400f02 <+6>:	mov    %rsp,%rsi                    # rsi = rsp
  0x0000000000400f05 <+9>:	call   0x40145c <read_six_numbers>  # read_six_numbers(rdi, rsi)
  0x0000000000400f0a <+14>:	cmpl   $0x1,(%rsp)                  # if(M[rsp] == 1)
  0x0000000000400f0e <+18>:	je     0x400f30 <phase_2+52>        # then: goto <+52>
  0x0000000000400f10 <+20>:	call   0x40143a <explode_bomb>      # else: explode_bomb()
  0x0000000000400f15 <+25>:	jmp    0x400f30 <phase_2+52>        # goto <+52>
  0x0000000000400f17 <+27>:	mov    -0x4(%rbx),%eax              # eax = M[rbx - 4]
  0x0000000000400f1a <+30>:	add    %eax,%eax                    # eax += eax
  0x0000000000400f1c <+32>:	cmp    %eax,(%rbx)                  # if (M[rbx] == eax)
  0x0000000000400f1e <+34>:	je     0x400f25 <phase_2+41>        # then: goto <+41>
  0x0000000000400f20 <+36>:	call   0x40143a <explode_bomb>      # else: explode_bomb()
  0x0000000000400f25 <+41>:	add    $0x4,%rbx                    # rbx += 4
  0x0000000000400f29 <+45>:	cmp    %rbp,%rbx                    # if (rbx != rbp)
  0x0000000000400f2c <+48>:	jne    0x400f17 <phase_2+27>        # then: goto <+27>
  0x0000000000400f2e <+50>:	jmp    0x400f3c <phase_2+64>        # else: goto <+64>
  0x0000000000400f30 <+52>:	lea    0x4(%rsp),%rbx               # rbx = rsp + 4
  0x0000000000400f35 <+57>:	lea    0x18(%rsp),%rbp              # rbp = rsp + 24
  0x0000000000400f3a <+62>:	jmp    0x400f17 <phase_2+27>        # goto <+27>
  0x0000000000400f3c <+64>:	add    $0x28,%rsp                   # deallocate 40 bytes from stack
  0x0000000000400f40 <+68>:	pop    %rbx                         # restore rbx
  0x0000000000400f41 <+69>:	pop    %rbp                         # restore rbp
  0x0000000000400f42 <+70>:	ret                                 # return

read_six_numbers:
  0x000000000040145c <+0>:	sub    $0x18,%rsp                     # allocate 24 bytes to stack
  0x0000000000401460 <+4>:	mov    %rsi,%rdx                      # rdx = rsi
  0x0000000000401463 <+7>:	lea    0x4(%rsi),%rcx                 # rcx = rsi + 4
  0x0000000000401467 <+11>:	lea    0x14(%rsi),%rax                # rax = rsi + 20
  0x000000000040146b <+15>:	mov    %rax,0x8(%rsp)                 # *(rsp + 8) = rax
  0x0000000000401470 <+20>:	lea    0x10(%rsi),%rax                # rax = rsi + 16
  0x0000000000401474 <+24>:	mov    %rax,(%rsp)                    # *rsp = rax
  0x0000000000401478 <+28>:	lea    0xc(%rsi),%r9                  # r9 = rsi + 12
  0x000000000040147c <+32>:	lea    0x8(%rsi),%r8                  # r8 = rsi + 8
  0x0000000000401480 <+36>:	mov    $0x4025c3,%esi                 # esi = 0x4025c3
  0x0000000000401485 <+41>:	mov    $0x0,%eax                      # eax = 0
  0x000000000040148a <+46>:	call   0x400bf0 <__isoc99_sscanf@plt> # sscanf(rdi,rsi,...)
  0x000000000040148f <+51>:	cmp    $0x5,%eax                      # if (eax > 5)
  0x0000000000401492 <+54>:	jg     0x401499 <read_six_numbers+61> # then: goto <+61>
  0x0000000000401494 <+56>:	call   0x40143a <explode_bomb>        # else: explode_bomb()
  0x0000000000401499 <+61>:	add    $0x18,%rsp                     # deallocate 24 bytes from stack
  0x000000000040149d <+65>:	ret                                   # return
```

## Memory Dump

- The string stored at `0x4025c3` is `%d %d %d %d %d %d`

## Equivalent C Code

```c
/*
- reads input from str
- returns the number of successful reads
*/
int sscanf(const char *str, const char *format, ...);

void phase_2(char* input){
  int num[6];
  read_six_numbers(input, num);
  if(num[0] != 1) explode_bomb();
  for(int i = 1; i < 6; ++i){
    if (num[i] != 2 * num[i - 1]) explode_bomb();
  }
}

void read_six_numbers(char* input, int* num) {
  int cnt = 0;
  cnt = sscanf(input, "%d %d %d %d %d %d",
                num, num + 1, num + 2, num + 3, num + 4, num + 5);
  if (cnt < 6) explode_bomb();
}

```

## The Key

By inspecting the C code, we can see that the key is: `1 2 4 8 16 32`

# Phase 3 Solution

## Annotated Assembly code

```asm
phase_3:
  0x0000000000400f43 <+0>:	sub    $0x18,%rsp                     # allocate 24 bytes to stack
  0x0000000000400f47 <+4>:	lea    0xc(%rsp),%rcx                 # rcx = rsp + 12
  0x0000000000400f4c <+9>:	lea    0x8(%rsp),%rdx                 # rdx = rsp + 8
  0x0000000000400f51 <+14>:	mov    $0x4025cf,%esi                 # esi = 0x4025cf
  0x0000000000400f56 <+19>:	mov    $0x0,%eax                      # eax = 0
  0x0000000000400f5b <+24>:	call   0x400bf0 <__isoc99_sscanf@plt> # sscanf(rdi,rsi,rdx,rcx)
  0x0000000000400f60 <+29>:	cmp    $0x1,%eax                      # if (eax > 1)
  0x0000000000400f63 <+32>:	jg     0x400f6a <phase_3+39>          # then: goto <+39>
  0x0000000000400f65 <+34>:	call   0x40143a <explode_bomb>        # else: explode_bomb()
  0x0000000000400f6a <+39>:	cmpl   $0x7,0x8(%rsp)                 # if (rsp[8] > 7)
  0x0000000000400f6f <+44>:	ja     0x400fad <phase_3+106>         # then: goto <+106>
  0x0000000000400f71 <+46>:	mov    0x8(%rsp),%eax                 # else: eax = rsp[8]
  0x0000000000400f75 <+50>:	jmp    *0x402470(,%rax,8)             # goto M[0x402470 + 8 * rax]
  0x0000000000400f7c <+57>:	mov    $0xcf,%eax                     # eax = 207
  0x0000000000400f81 <+62>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400f83 <+64>:	mov    $0x2c3,%eax                    # eax = 707
  0x0000000000400f88 <+69>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400f8a <+71>:	mov    $0x100,%eax                    # eax = 256
  0x0000000000400f8f <+76>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400f91 <+78>:	mov    $0x185,%eax                    # eax = 389
  0x0000000000400f96 <+83>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400f98 <+85>:	mov    $0xce,%eax                     # eax = 206
  0x0000000000400f9d <+90>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400f9f <+92>:	mov    $0x2aa,%eax                    # eax = 682
  0x0000000000400fa4 <+97>:	jmp    0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400fa6 <+99>:	mov    $0x147,%eax                    # eax = 327
  0x0000000000400fab <+104>: jmp   0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400fad <+106>: call  0x40143a <explode_bomb>        # explode_bomb()
  0x0000000000400fb2 <+111>: mov   $0x0,%eax                      # eax = 0
  0x0000000000400fb7 <+116>: jmp   0x400fbe <phase_3+123>         # goto <+123>
  0x0000000000400fb9 <+118>: mov   $0x137,%eax                    # eax = 311
  0x0000000000400fbe <+123>: cmp   0xc(%rsp),%eax                 # if (eax == rsp[12])
  0x0000000000400fc2 <+127>: je    0x400fc9 <phase_3+134>         # then: goto <+134>
  0x0000000000400fc4 <+129>: call  0x40143a <explode_bomb>        # else: explode_bomb()
  0x0000000000400fc9 <+134>: add   $0x18,%rsp                     # deallocate 24 bytes from stack
  0x0000000000400fcd <+138>: ret                                  # return
```

## Memory Dump

- The string stored at `0x4025cf` is `%d %d`
- The jump table is stored at `0x402470`

  ```asm
  # Memory dump at 0x402470: [ The Jump Table ]
    [00]:	0x0000000000400f7c
    [08]: 0x0000000000400fb9
    [16]:	0x0000000000400f83
    [24]: 0x0000000000400f8a
    [32]:	0x0000000000400f91
    [40]: 0x0000000000400f98
    [48]:	0x0000000000400f9f
    [56]: 0x0000000000400fa6
  ```

## Equivalent C Code

```c
void phase_3(char* input){
  int num[2];
  int eax = 0;
  if (sscanf(input, "%d %d", num, num + 1) < 2) explode_bomb();
  switch(num[0]){
    case 0:
      eax = 207;
      break;
    case 1:
      eax = 311;
      break;
    case 2:
      eax = 707;
      break;
    case 3:
      eax = 256;
      break;
    case 4:
      eax = 389;
      break;
    case 5:
      eax = 206;
      break;
    case 6:
      eax = 682;
      break;
    case 7:
      eax = 327;
      break;
    default:
      explode_bomb();
  }

  if(num[1] != eax) explode_bomb();

}
```

## The Key

From the C code, we can observe that the key is **any** of the following pairs:

- `0 207`
- `1 311`
- `2 707`
- `3 256`
- `4 389`
- `5 206`
- `6 682`
- `7 327`

# Phase 4 Solution

## Annotated Assembly code

```asm
phase_4:
  0x000000000040100c <+0>:	sub    $0x18,%rsp                     # allocate 24 bytes to stack
  0x0000000000401010 <+4>:	lea    0xc(%rsp),%rcx                 # rcx = rsp + 12
  0x0000000000401015 <+9>:	lea    0x8(%rsp),%rdx                 # rdx = rsp + 8
  0x000000000040101a <+14>:	mov    $0x4025cf,%esi                 # esi = 0x4025cf
  0x000000000040101f <+19>:	mov    $0x0,%eax                      # eax = 0
  0x0000000000401024 <+24>:	call   0x400bf0 <__isoc99_sscanf@plt> # sscanf(rdi,rsi,rdx,rcx)
  0x0000000000401029 <+29>:	cmp    $0x2,%eax                      # if (eax != 2)
  0x000000000040102c <+32>:	jne    0x401035 <phase_4+41>          # then: goto <+41>
  0x000000000040102e <+34>:	cmpl   $0xe,0x8(%rsp)                 # else: if (rsp[8] <= 14)
  0x0000000000401033 <+39>:	jbe    0x40103a <phase_4+46>          # then: goto <+46>
  0x0000000000401035 <+41>:	call   0x40143a <explode_bomb>        # else: explode_bomb()
  0x000000000040103a <+46>:	mov    $0xe,%edx                      # edx = 14
  0x000000000040103f <+51>:	mov    $0x0,%esi                      # esi = 0
  0x0000000000401044 <+56>:	mov    0x8(%rsp),%edi                 # edi = rsp[8]
  0x0000000000401048 <+60>:	call   0x400fce <func4>               # func4(rdi,rsi,rdx)
  0x000000000040104d <+65>:	test   %eax,%eax                      # if (eax != 0)
  0x000000000040104f <+67>:	jne    0x401058 <phase_4+76>          # then: goto <+76>
  0x0000000000401051 <+69>:	cmpl   $0x0,0xc(%rsp)                 # else: if(rsp[12] == 0)
  0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>          # then: goto<+81>
  0x0000000000401058 <+76>:	call   0x40143a <explode_bomb>        # else: explode_bomb()
  0x000000000040105d <+81>:	add    $0x18,%rsp                     # deallocate 24 bytes from stack
  0x0000000000401061 <+85>:	ret                                   # retrun

func4:
  0x0000000000400fce <+0>:	sub    $0x8,%rsp                # allocate 8 bytes to stack
  0x0000000000400fd2 <+4>:	mov    %edx,%eax                # eax = edx
  0x0000000000400fd4 <+6>:	sub    %esi,%eax                # eax = eax - esi
  0x0000000000400fd6 <+8>:	mov    %eax,%ecx                # ecx = eax
  0x0000000000400fd8 <+10>:	shr    $0x1f,%ecx               # exc = ecx >> 31 (logical)
  0x0000000000400fdb <+13>:	add    %ecx,%eax                # eax = eax + ecx
  0x0000000000400fdd <+15>:	sar    %eax                     # eax = eax >> 1 (arithmetic)
  0x0000000000400fdf <+17>:	lea    (%rax,%rsi,1),%ecx       # ecx = rax + rsi
  0x0000000000400fe2 <+20>:	cmp    %edi,%ecx                # if(ecx <= edi)
  0x0000000000400fe4 <+22>:	jle    0x400ff2 <func4+36>      # then: goto <+36>
  0x0000000000400fe6 <+24>:	lea    -0x1(%rcx),%edx          # else: edx = rcx-1
  0x0000000000400fe9 <+27>:	call   0x400fce <func4>         # func4(rdi,rsi,rdx)
  0x0000000000400fee <+32>:	add    %eax,%eax                # eax += eax
  0x0000000000400ff0 <+34>:	jmp    0x401007 <func4+57>      # goto <+57>
  0x0000000000400ff2 <+36>:	mov    $0x0,%eax                # eax = 0
  0x0000000000400ff7 <+41>:	cmp    %edi,%ecx                # if (ecx >= edi)
  0x0000000000400ff9 <+43>:	jge    0x401007 <func4+57>      # then: goto<+57>
  0x0000000000400ffb <+45>:	lea    0x1(%rcx),%esi           # else: esi = rcx + 1
  0x0000000000400ffe <+48>:	call   0x400fce <func4>         # func4(rdi,rsi,rdx)
  0x0000000000401003 <+53>:	lea    0x1(%rax,%rax,1),%eax    # eax = 1 + rax + rax
  0x0000000000401007 <+57>:	add    $0x8,%rsp                # deallocate 8 bytes from stack
  0x000000000040100b <+61>:	ret                             # return
```

## Memory Dump

- The string stored at `0x4025cf` is `%d %d`

## Equivalent C Code

```c
void phase_4(char* input){
  int num[2];
  if (sscanf(input, "%d %d", num , num + 1) != 2) explode_bomb();
  if (num[0] > 14) explode_bomb();
  if (func4(num[0],0,14) != 0) explode_bomb();
  if (num[1] != 0) explode_bomb();
}

int func4 (int rdi, int rsi, int rdx){
  int rax, rcx;

  rax = rdx - rsi;
  rcx = ((unsigned)rax) >> 31;
  rax = rax + rcx;
  rax = rax >> 1;
  rcx = rax + rsi;

  if (rcx == rdi) return 0;

  if(rcx < rdi){
    return 1 + 2 * func4(rdi,rcx + 1,rdx);
  } else {
    return 2 * func4(rdi,rsi, rcx - 1);
  }
}
```

## The Key

From the C code, we can see that the key is two integers `a b`, such that:

- (a < 15) and f(a) = 0
- b = 0

Where: f(a) = func4(a, 0, 14)

Running f(a) with a = [0:14], we get the following table:

| a        | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  |
| -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **f(a)** | 0   | 0   | 4   | 0   | 2   | 2   | 6   | 0   | 1   | 1   | 5   | 1   | 3   | 3   | 7   |

From the table, we can see that the key is **any** of the following pairs:

- `0 0`
- `1 0`
- `3 0`
- `7 0`

# Phase 5 Solution

## Annotated Assembly code

```asm
phase_5:
  0x0000000000401062 <+0>:	push   %rbx                             # save rbx
  0x0000000000401063 <+1>:	sub    $0x20,%rsp                       # allocate 32 bytes to stack
  0x0000000000401067 <+5>:	mov    %rdi,%rbx                        # rbx = rdi
  0x000000000040106a <+8>:	mov    %fs:0x28,%rax                    # rax = fs[0x28] (canary)
  0x0000000000401073 <+17>:	mov    %rax,0x18(%rsp)                  # rsp[24] = rax
  0x0000000000401078 <+22>:	xor    %eax,%eax                        # eax = 0
  0x000000000040107a <+24>:	call   0x40131b <string_length>         # string_length(rdi)
  0x000000000040107f <+29>:	cmp    $0x6,%eax                        # if(eax == 6)
  0x0000000000401082 <+32>:	je     0x4010d2 <phase_5+112>           # then: goto <+112>
  0x0000000000401084 <+34>:	call   0x40143a <explode_bomb>          # else: explode_bomb()
  0x0000000000401089 <+39>:	jmp    0x4010d2 <phase_5+112>           # goto: <+112>
  0x000000000040108b <+41>:	movzbl (%rbx,%rax,1),%ecx               # ecx = M[rbx + rax]
  0x000000000040108f <+45>:	mov    %cl,(%rsp)                       # rsp[0] = cl
  0x0000000000401092 <+48>:	mov    (%rsp),%rdx                      # rdx = rsp[0]
  0x0000000000401096 <+52>:	and    $0xf,%edx                        # edx = edx & 0xf
  0x0000000000401099 <+55>:	movzbl 0x4024b0(%rdx),%edx              # edx = M[rdx + 0x4024b0]
  0x00000000004010a0 <+62>:	mov    %dl,0x10(%rsp,%rax,1)            # M[16 + rsp + rax] = dl
  0x00000000004010a4 <+66>:	add    $0x1,%rax                        # rax += 1
  0x00000000004010a8 <+70>:	cmp    $0x6,%rax                        # if(rax != 6)
  0x00000000004010ac <+74>:	jne    0x40108b <phase_5+41>            # then: goto<+41>
  0x00000000004010ae <+76>:	movb   $0x0,0x16(%rsp)                  # else: rsp[22] = 0 (char)
  0x00000000004010b3 <+81>:	mov    $0x40245e,%esi                   # esi = 0x40245e
  0x00000000004010b8 <+86>:	lea    0x10(%rsp),%rdi                  # rdi = rsp[16]
  0x00000000004010bd <+91>:	call   0x401338 <strings_not_equal>     # strings_not_equal(rdi,rsi)
  0x00000000004010c2 <+96>:	test   %eax,%eax                        # if(eax == 0)
  0x00000000004010c4 <+98>:	je     0x4010d9 <phase_5+119>           # then: goto <+119>
  0x00000000004010c6 <+100>: call  0x40143a <explode_bomb>          # else: explode_bomb()
  0x00000000004010cb <+105>: nopl  0x0(%rax,%rax,1)                 # do nothing
  0x00000000004010d0 <+110>: jmp   0x4010d9 <phase_5+119>           # goto <+119>
  0x00000000004010d2 <+112>: mov   $0x0,%eax                        # eax = 0
  0x00000000004010d7 <+117>: jmp   0x40108b <phase_5+41>            # goto <+41>
  0x00000000004010d9 <+119>: mov   0x18(%rsp),%rax                  # rax = rsp[24]
  0x00000000004010de <+124>: xor   %fs:0x28,%rax                    # rax = rax ^ fs[0x28]
  0x00000000004010e7 <+133>: je    0x4010ee <phase_5+140>           # if(rax == 0) goto <+140>
  0x00000000004010e9 <+135>: call  0x400b30 <__stack_chk_fail@plt>  # else: stack_chk_fail()
  0x00000000004010ee <+140>: add   $0x20,%rsp                       # deallocate 32 bytes from stack
  0x00000000004010f2 <+144>: pop   %rbx                             # restore rbx
  0x00000000004010f3 <+145>: ret                                    # return
```

## Memory Dump

- The string stored at `0x4024b0` is `maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?`
- The string stored at `0x40245e` is `flyers`

## Equivalent C Code

```c
void phase_5(char* input){
  char* text = "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?";
  char buff[8];
  if(string_length(input) != 6) explode_bomb();

  for(int i = 0; i < 6; ++i){
    buff[i] = text[input[i] % 16];
  }
  buff[6] = 0;

  if(strings_not_equal(buff, "flyers") != 0) explode_bomb();
}

```

## The Key

From the C code, we can observe that the key is a string of 6 chars, where the word "flyers" is made out of chars in `text` using key[i]%16 as an index:

- index = key[i]%16
- text[index] must concat to "flyers"

The indexable part of `text`:

| index    | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  |
| -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **text** | m   | a   | d   | u   | i   | e   | r   | s   | n   | f   | o   | t   | v   | b   | y   | l   |

From the indexes we can generate a printable string that is based on multiples of 16:

| char | index | ASCII[32] | ASCII[48] | ASCII[64] | ASCII[80] | ASCII[96] |
| ---- | ----- | --------- | --------- | --------- | --------- | --------- |
| f    | 9     | )         | 9         | I         | Y         | i         |
| l    | 15    | /         | ?         | O         | \_        | o         |
| y    | 14    | .         | >         | N         | ^         | n         |
| e    | 5     | %         | 5         | E         | U         | e         |
| r    | 6     | &         | 6         | F         | V         | f         |
| s    | 7     | '         | 7         | G         | W         | g         |

From this table, we can see that the key is **any** of:

- `ionefg`
- `IONEFG`
- `Y_^UVW`
- `9?>567`
- `)/.%&'`

# Phase 6 Solution

## Annotated Assembly code

```asm
phase_6:
  0x00000000004010f4 <+0>:	push   %r14                         # save r14
  0x00000000004010f6 <+2>:	push   %r13                         # save r13
  0x00000000004010f8 <+4>:	push   %r12                         # save r12
  0x00000000004010fa <+6>:	push   %rbp                         # save rbp
  0x00000000004010fb <+7>:	push   %rbx                         # save rbx
  0x00000000004010fc <+8>:	sub    $0x50,%rsp                   # allocate 80 bytes to stack
  0x0000000000401100 <+12>:	mov    %rsp,%r13                    # r13 = rsp
  0x0000000000401103 <+15>:	mov    %rsp,%rsi                    # rsi = rsp
  0x0000000000401106 <+18>:	call   0x40145c <read_six_numbers>  # read_six_numbers(rdi, rsi)
  0x000000000040110b <+23>:	mov    %rsp,%r14                    # r14 = rsp
  0x000000000040110e <+26>:	mov    $0x0,%r12d                   # r12 = 0
  0x0000000000401114 <+32>:	mov    %r13,%rbp                    # rbp = r13
  0x0000000000401117 <+35>:	mov    0x0(%r13),%eax               # eax = M[r13]
  0x000000000040111b <+39>:	sub    $0x1,%eax                    # eax = eax - 1
  0x000000000040111e <+42>:	cmp    $0x5,%eax                    # if(eax <= 5)
  0x0000000000401121 <+45>:	jbe    0x401128 <phase_6+52>        # then: goto <+52>
  0x0000000000401123 <+47>:	call   0x40143a <explode_bomb>      # else: explode_bomb()
  0x0000000000401128 <+52>:	add    $0x1,%r12d                   # r12 += 1
  0x000000000040112c <+56>:	cmp    $0x6,%r12d                   # if(r12 == 6)
  0x0000000000401130 <+60>:	je     0x401153 <phase_6+95>        # then: goto <+95>
  0x0000000000401132 <+62>:	mov    %r12d,%ebx                   # else: ebx = r12
  0x0000000000401135 <+65>:	movslq %ebx,%rax                    # rax = ebx
  0x0000000000401138 <+68>:	mov    (%rsp,%rax,4),%eax           # eax = rsp[rax*4]
  0x000000000040113b <+71>:	cmp    %eax,0x0(%rbp)               # if(M[rbp] != eax)
  0x000000000040113e <+74>:	jne    0x401145 <phase_6+81>        # then: goto <+81>
  0x0000000000401140 <+76>:	call   0x40143a <explode_bomb>      # else: explode_bomb()
  0x0000000000401145 <+81>:	add    $0x1,%ebx                    # ebx += 1
  0x0000000000401148 <+84>:	cmp    $0x5,%ebx                    # if(ebx <= 5)
  0x000000000040114b <+87>:	jle    0x401135 <phase_6+65>        # then: goto <+65>
  0x000000000040114d <+89>:	add    $0x4,%r13                    # else: r13 += 4
  0x0000000000401151 <+93>:	jmp    0x401114 <phase_6+32>        # goto <+32>
  0x0000000000401153 <+95>:	lea    0x18(%rsp),%rsi              # rsi = rsp + 24
  0x0000000000401158 <+100>: mov    %r14,%rax                   # rax = r14
  0x000000000040115b <+103>: mov    $0x7,%ecx                   # ecx = 7
  0x0000000000401160 <+108>: mov    %ecx,%edx                   # edx = ecx
  0x0000000000401162 <+110>: sub    (%rax),%edx                 # edx -= M[rax]
  0x0000000000401164 <+112>: mov    %edx,(%rax)                 # M[rax] = edx
  0x0000000000401166 <+114>: add    $0x4,%rax                   # rax += 4
  0x000000000040116a <+118>: cmp    %rsi,%rax                   # if(rax != rsi)
  0x000000000040116d <+121>: jne    0x401160 <phase_6+108>      # then: goto<+108>
  0x000000000040116f <+123>: mov    $0x0,%esi                   # else: esi = 0
  0x0000000000401174 <+128>: jmp    0x401197 <phase_6+163>      # goto <+163>
  0x0000000000401176 <+130>: mov    0x8(%rdx),%rdx              # rdx = rdx[8]
  0x000000000040117a <+134>: add    $0x1,%eax                   # eax += 1
  0x000000000040117d <+137>: cmp    %ecx,%eax                   # if(eax != ecx)
  0x000000000040117f <+139>: jne    0x401176 <phase_6+130>      # then: goto <+130>
  0x0000000000401181 <+141>: jmp    0x401188 <phase_6+148>      # else: goto <+148>
  0x0000000000401183 <+143>: mov    $0x6032d0,%edx              # edx = 0x6032d0
  0x0000000000401188 <+148>: mov    %rdx,0x20(%rsp,%rsi,2)      # rsp[32+2*rsi] = rdx
  0x000000000040118d <+153>: add    $0x4,%rsi                   # rsi += 4
  0x0000000000401191 <+157>: cmp    $0x18,%rsi                  # if(rsi == 24)
  0x0000000000401195 <+161>: je     0x4011ab <phase_6+183>      # then: goto<+183>
  0x0000000000401197 <+163>: mov    (%rsp,%rsi,1),%ecx          # else: ecx = rsp[rsi]
  0x000000000040119a <+166>: cmp    $0x1,%ecx                   # if(ecx <= 1)
  0x000000000040119d <+169>: jle    0x401183 <phase_6+143>      # then: goto <+143>
  0x000000000040119f <+171>: mov    $0x1,%eax                   # else: eax = 1
  0x00000000004011a4 <+176>: mov    $0x6032d0,%edx              # edx = 0x6032d0
  0x00000000004011a9 <+181>: jmp    0x401176 <phase_6+130>      # goto <+130>
  0x00000000004011ab <+183>: mov    0x20(%rsp),%rbx             # rbx = rsp[32]
  0x00000000004011b0 <+188>: lea    0x28(%rsp),%rax             # rax = rsp + 40
  0x00000000004011b5 <+193>: lea    0x50(%rsp),%rsi             # rsi = rsp + 80
  0x00000000004011ba <+198>: mov    %rbx,%rcx                   # rcx = rbx
  0x00000000004011bd <+201>: mov    (%rax),%rdx                 # rdx = M[rax]
  0x00000000004011c0 <+204>: mov    %rdx,0x8(%rcx)              # M[rcx+8] = rdx
  0x00000000004011c4 <+208>: add    $0x8,%rax                   # rax += 8
  0x00000000004011c8 <+212>: cmp    %rsi,%rax                   # if(rax == rsi)
  0x00000000004011cb <+215>: je     0x4011d2 <phase_6+222>      # then: goto <+222>
  0x00000000004011cd <+217>: mov    %rdx,%rcx                   # else: rcx = rdx
  0x00000000004011d0 <+220>: jmp    0x4011bd <phase_6+201>      # goto <+201>
  0x00000000004011d2 <+222>: movq   $0x0,0x8(%rdx)              # M[rdx+8] = 0
  0x00000000004011da <+230>: mov    $0x5,%ebp                   # ebp = 5
  0x00000000004011df <+235>: mov    0x8(%rbx),%rax              # rax = M[rbx+8]
  0x00000000004011e3 <+239>: mov    (%rax),%eax                 # eax = M[rax]
  0x00000000004011e5 <+241>: cmp    %eax,(%rbx)                 # if (M[rbx] >= eax)
  0x00000000004011e7 <+243>: jge    0x4011ee <phase_6+250>      # then: goto<+250>
  0x00000000004011e9 <+245>: call   0x40143a <explode_bomb>     # else: explode_bomb()
  0x00000000004011ee <+250>: mov    0x8(%rbx),%rbx              # rbx = M[rbx + 8]
  0x00000000004011f2 <+254>: sub    $0x1,%ebp                   # ebp -= 1
  0x00000000004011f5 <+257>: jne    0x4011df <phase_6+235>      # if(ebp != 0) goto <+235>
  0x00000000004011f7 <+259>: add    $0x50,%rsp                  # deallocate 80 bytes from stack
  0x00000000004011fb <+263>: pop    %rbx                        # restore rbx
  0x00000000004011fc <+264>: pop    %rbp                        # restore rbp
  0x00000000004011fd <+265>: pop    %r12                        # restore r12
  0x00000000004011ff <+267>: pop    %r13                        # restore r13
  0x0000000000401201 <+269>: pop    %r14                        # restore r14
  0x0000000000401203 <+271>: ret                                # return
```

## Memory Dump

- Stack dump for input = 6 5 4 3 2 1

  ```asm
  0x7fffffffdc00:	0x00000001	0x00000002	0x00000003	0x00000004
  0x7fffffffdc10:	0x00000005	0x00000006	0x00000000	0x00000000
  0x7fffffffdc20:	0x006032d0	0x00000000	0x006032e0	0x00000000
  0x7fffffffdc30:	0x006032f0	0x00000000	0x00603300	0x00000000
  0x7fffffffdc40:	0x00603310	0x00000000	0x00603320	0x00000000
  ```

- Memory dump at range [`0x006032d0` : `0x603320`] (_linked-list of 6 nodes_)

  ```asm
  # address   nodeN   value(2-bytes)      next(node address)
    0x6032d0 <node1>:	0x000000010000014c	0x00000000006032e0
    0x6032e0 <node2>:	0x00000002000000a8	0x00000000006032f0
    0x6032f0 <node3>:	0x000000030000039c	0x0000000000603300
    0x603300 <node4>:	0x00000004000002b3	0x0000000000603310
    0x603310 <node5>:	0x00000005000001dd	0x0000000000603320
    0x603320 <node6>:	0x00000006000001bb	0x0000000000000000
  ```

## The Key

From the annotated assembly code, we can observe that the key is made of 6 numbers:

- the numbers are unique
- the numbers are in range [1:6]
- the numbers are used to re-order the nodes of the linked-list
- the correct key is the key that sorts the nodes in a descending order.
- the node value is of type short (2-bytes on the LSB)

From the memory-dump we can build the following table:

| input | address    | node | value[hex] | value | order |
| ----- | ---------- | ---- | ---------- | ----- | ----- |
| 6     | 0x006032d0 | 1    | 0x014c     | 332   | 5     |
| 5     | 0x006032e0 | 2    | 0x00a8     | 168   | 6     |
| 4     | 0x006032f0 | 3    | 0x039c     | 924   | 1     |
| 3     | 0x00603300 | 4    | 0x02b3     | 691   | 2     |
| 2     | 0x00603310 | 5    | 0x01dd     | 477   | 3     |
| 1     | 0x00603320 | 6    | 0x01bb     | 443   | 4     |

The key is the input that sorts the values in a descending ordered. From this table we can see that the key is: `4 3 2 1 6 5`
