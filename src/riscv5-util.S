.section .text
.globl readTime

readTime:
    rdcycle a0
    ret

manipulateStack:
    ;x2 is the sp
    ;x1 is the return address
    ;x8 is the frame pointer
    ;The R5 ABI doesn't specify what things are pushed onto the stack.
    ;TODO: How do you know how much to pop off the stack?
    ;Does GCC automatically store the return address on the stack? (Does it know the depth of the function calls)
    ;This may be something that we have to check when debugging the program then put a hardcoded val to pop off
    ret
