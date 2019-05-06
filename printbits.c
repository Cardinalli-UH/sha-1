#include <stdio.h>
#include "printbits.h"

/* 
   Displays the bit pattern for a 32-bit int
   number: is the 32-bit integer to be displayed
*/
void printbits(unsigned int number){
  //loop counter 
  int i = 0; 
  /*
    A mask is used by bitwise AND to determine 
    if a specific 1 or a 0 is present.
    Masks are used to select some bits 
    (using 1s - which returns 1, if 1, and returns 0, if 0)
    or hide other bits 
    (using 0s - which make everything zero).
    This starts with a 1 at the 31st (leftmost) position,
    followed by 31 zeros.
  */
  unsigned int mask = 0x80000000; 
  
  //loop through the 32 bits
  for(i=31;i>=0;i--) { 
    //printf("i=%i, mask=0x%x",i,mask);
    //determine if a 1 or 0 exists at each bit location
    if(0 != (number & mask)){
      printf("1");
    }
    else{
      printf("0");
    }
    //add a space for easy reading
    if((24==i) || (16==i) || (8==i)){
      printf(" ");
    }
    //shift bits in mask to the right by one bit
    mask = mask >> 1; 
  }//end of for loop
  
    /*
    Display the unsigned integer.  
    %u is the format character for unsigned integers. 
    %d is for signed integers - use if you want to display negative numbers.
    Leave room for 8 extra spaces for hexadecimal numbers.
  */
  printf(" = 0x%.8X = %d \n", number, number);
  
}//end of function

