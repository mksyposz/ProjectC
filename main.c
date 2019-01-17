#include<stdlib.h>
#include<stdio.h>

int main() 
{ 
    while(system("./menu") == 0)
    { 
        system("./gtk-talk A &"); 
        system("./gtk-talk B"); 
    } 
} 
