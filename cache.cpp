//Jethro Chan August 2015
/*Program  Description
256 Byte Direct Mapped Cache
Write Back (includes Dirty Bit)
    Dirty bit becomes True when value is written to that line of cache
Write Miss causes appropriate line to be
brought from cache into Memory
*/
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
int numline;
int WRITTENtags[100];
int WRITTENlines[100];

int myCounterArr = 0; //Counter for Array
bool ifAble;

char *inputAr = new char[20]; //For reading in test file
char intOut;
char *address = new char[5];
char *choose = new char[4]; //Choice Variable, Read or Write

char *newdata = new char[4];
char *newtag = new char[3];

int binary;

int off; //Offset

int TypeConverter(char input) /*Convert Hex to Binary*/
{
  if(input == '0'){
    return 0;
  }
  else if(input == '1'){
    return 1;
  }
  else if(input == '2'){
    return 10;
  }
  else if(input == '3'){
    return 11;
  }
  else if(input == '4'){
    return 100;
  }
  else if(input == '5'){
    return 101;
  }
  else if(input == '6'){
    return 110;
  }
  else if(input == '7'){
    return 111;
  }
  else if(input == '8'){
    return 1000;
  }
  else if(input == '9'){
    return 1001;
  }
  else if(input == 'A'){
    return 1010;
  }
  else if(input == 'B'){
    return 1011;
  }
  else if(input == 'C'){
    return 1100;
  }
  else if(input == 'D'){
    return 1101;
  }
  else if(input == 'E'){
    return 1110;
  }
  else if(input == 'F'){
    return 1111;
  }

  else return 0;

}


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
bool hit = false;


int main(int argc, char **argv)
{
class offset
{
public:
  char data[3];
  int my_input;
};


class index
{
public:
  offset offsetnum[5];
  char tag[3];
  bool isdirty; //DirtyBit, 1 or 0
  int retval;
};

class myData
{
public:
  index lin[64];
};
//Read in Test File-------------
 ofstream output;
 output.open ("my-out.txt");

 myData *mainmemory = new myData[256];

 for (int i=0 ; i < 256; i++)
   for(int k=0; k <64; k++)
     for(int j=0; j <4; j++)
       strcpy(mainmemory[i].lin[k].offsetnum[j].data, "00");//Set Data as 00

 index *line = new index[64];
 for (int cntry=0; cntry < 64; cntry++)
   {
     //INITIALIZE VALUES
     for(int j=0; j<4; j++)
       {
	 strcpy(line[cntry].offsetnum[j].data, "00");
       }
     strcpy(line[cntry].tag,"-1"); //set tag value as -1
     line[cntry].isdirty = false; //set isdirty default value as 0 or FALSE
   }

 FILE *fp = fopen(argv[1],"r");

 while(fgets(inputAr, 12, fp))
   {
     strcpy(address,strtok(inputAr, " ")); //Copy at the Raw Data's Address
    strcpy(choose,strtok(NULL, " ")); //Copy Raw Data's Choice, Read or Write
    strcpy(newdata,strtok(NULL, "\n")); //Copy Raw Data's Cache Line, ie Data itself

    newtag[0] = address[0]; // first 8 bits, recall tag = cache address
    newtag[1] = address[1]; //1 corresponds to 1, 0 corresponds to 0, etc

    binary = TypeConverter(address[2]) * 10000 + TypeConverter(address[3]); // get last 8 bits

    off = binary % 100; // get lower two bits, our OFFSET
    numline = binary / 100; // get middle 6 bits, our LINE NUMBER

    off = bin_dec(off); // change offset to decimal
    numline = bin_dec(numline); // change line number to decimal

    if(!strcmp(newtag,line[numline].tag)) // if tag hit, that means its existing in cache already
      hit = true;

    if (hit and strcmp(choose,"FF"))// if read (ie read from cache) and NOT a cache miss, FF means a read
      {
	output << line[numline].offsetnum[off].data << ' ' << hit << ' '
	       << line[numline].isdirty << endl;
      }

    if(strcmp(newtag,line[numline].tag))
      {
	if (line[numline].isdirty) // write back, if Dirty Bit is TRUE
	  {
	    for(int k=0; k < myCounterArr; k++)
	      for (int i=0; i < 4; i++)
		strcpy(mainmemory[WRITTENtags[k]].lin[WRITTENlines[k]].offsetnum[i].data,
		       line[WRITTENlines[k]].offsetnum[i].data); //Copy What's in Cache to Main Memory, ie a Write Back
	    myCounterArr = 0;
	  }
	for (int i=0; i < 4; i++)// read from memory
	  strcpy(line[numline].offsetnum[i].data,
		 mainmemory[bin_dec(TypeConverter(newtag[0])*10000+TypeConverter(newtag[1]))]
		 .lin[numline].offsetnum[i].data);
	strcpy(line[numline].tag,newtag);
	line[numline].isdirty = false; //cache data updated not written to memory yet, now Dirty

	if (strcmp(choose,"FF")) // if read
	  output << line[numline].offsetnum[off].data << ' ' << hit << ' '
		 << line[numline].isdirty << endl;
      }

    if (!strcmp(choose,"FF")) // if write
      {
	strcpy(line[numline].offsetnum[off].data,newdata);
	WRITTENtags[myCounterArr] = TypeConverter(newtag[0]) * 10000 + TypeConverter(newtag[1]);
	WRITTENtags[myCounterArr] = bin_dec(WRITTENtags[myCounterArr]);
	line[numline].isdirty = true;
	WRITTENlines[myCounterArr] = numline;
	myCounterArr++;
      }

    hit = false;
   }
 output.close();
 fclose(fp);

 return (0);
}

