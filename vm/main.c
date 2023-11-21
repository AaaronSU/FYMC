#include "register.h"

int add_asm(int a, int b)
{
    return a + b;
}

int main(int arcg, char** argv)
{

}


loop:
    add a,b
    jmp loop

loop2:
    jmp loop2