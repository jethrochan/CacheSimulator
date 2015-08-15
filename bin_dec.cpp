#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <bitset>
#include <ctype.h>
#include <locale>

using namespace std;

int bin_dec(int inputval) /*Binary to Decimal*/
{
  int retval = 0;
  int rem;
  int divisor=1, cntr = inputval;
  while (cntr > 0)
    {rem = cntr % 10;
      retval = retval + rem * divisor;
      divisor = divisor * 2;
      cntr = cntr / 10;}
  return retval;
}



int main()
{
  int value;
  value = bin_dec(11);
  cout << "value = " << value << endl;
  
}//end main
