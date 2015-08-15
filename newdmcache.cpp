#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int hex_bin(char hex);
int bin_dec(int bin);


int main(int argc, char **argv)
{
class offset
{
public:
  char data[3];

};


class index
{
public:
  char tag[3];
  offset offsetnum[5];
  bool isdirty;
};

class mem
{
public:
  index lin[64];
};
 ofstream output;
 output.open ("dm-out.txt");

 mem *mainmemory = new mem[256];
 for (int i=0 ; i < 256; i++)
   for(int k=0; k <64; k++)
     for(int j=0; j <4; j++)
       strcpy(mainmemory[i].lin[k].offsetnum[j].data, "00");

 index *line = new index[64];
 for (int i=0; i < 64; i++)
   {
     for(int j=0; j<4; j++)
       {
	 strcpy(line[i].offsetnum[j].data, "00");
	 //      cout << line[i].offsetnum[j].data << ' ';
       }
     strcpy(line[i].tag,"-1");
     line[i].isdirty = false;
     //    cout << line[i].tag << ' ' << line[i].isdirty << endl;
   }
 char *lineread = new char[20];
 char *address = new char[5];
 char *command = new char[4];
 char *newdata = new char[4];
 char *newtag = new char[3];
 int binary;
 int numline;
 int wtag[100];
 int wline[100];
 int wcount = 0;
 int numoffset;
 bool cachehit = false;
 FILE *fp = fopen(argv[1],"r");
 while(fgets(lineread, 12, fp))
   {
     strcpy(address,strtok(lineread, " "));
     strcpy(command,strtok(NULL, " "));
     strcpy(newdata,strtok(NULL, "\n"));
     //    cout << address << " " << command << " " << newdata << endl;
     newtag[0] = address[0]; // first 8 bits
     newtag[1] = address[1];
     binary = hex_bin(address[2]) * 10000 + hex_bin(address[3]); // get last 8 bits
     numoffset = binary % 100; // get lower two bits
     numline = binary / 100; // get middle 6 bits
     numoffset = bin_dec(numoffset); // change to dec
     numline = bin_dec(numline); // change to dec
     //    cout << binary << endl << numline << ' ' << numoffset << endl;

     if(!strcmp(newtag,line[numline].tag)) // if tag hit
       cachehit = true;
     if (cachehit and strcmp(command,"FF"))// read
       {
	 output << line[numline].offsetnum[numoffset].data << ' ' << cachehit << ' '
		<< line[numline].isdirty << endl;
       }

     if(strcmp(newtag,line[numline].tag)) // if miss
       {
	 if (line[numline].isdirty) // write back
	   {
	     for(int k=0; k < wcount; k++)
	       for (int i=0; i < 4; i++)
		 strcpy(mainmemory[wtag[k]].lin[wline[k]].offsetnum[i].data,
			line[wline[k]].offsetnum[i].data);
	     wcount = 0;
	   }
	 for (int i=0; i < 4; i++)// reaf from RAM
	   strcpy(line[numline].offsetnum[i].data,
		  mainmemory[bin_dec(hex_bin(newtag[0])*10000+hex_bin(newtag[1]))]
		  .lin[numline].offsetnum[i].data);
	 strcpy(line[numline].tag,newtag);
	 line[numline].isdirty = false;

	 if (strcmp(command,"FF")) // read
	   output << line[numline].offsetnum[numoffset].data << ' ' << cachehit << ' '
		  << line[numline].isdirty << endl;
       }

     if (!strcmp(command,"FF")) // if write
       {
	 strcpy(line[numline].offsetnum[numoffset].data,newdata);
	 wtag[wcount] = hex_bin(newtag[0]) * 10000 + hex_bin(newtag[1]);
	 wtag[wcount] = bin_dec(wtag[wcount]);
	 line[numline].isdirty = true;
	 wline[wcount] = numline;
	 wcount++;
       }
     cachehit = false;
   }
 output.close();
 fclose(fp);
 delete mainmemory;
 delete lineread;
 delete address;
 delete command;
 delete newdata;
 delete newtag;
 delete line;
 return 0;
}

int hex_bin(char hex)
{
  switch(hex)
    {
    case '0': return 0;
    case '1': return 1;
    case '2': return 10;
    case '3': return 11;
    case '4': return 100;
    case '5': return 101;
    case '6': return 110;
    case '7': return 111;
    case '8': return 1000;
    case '9': return 1001;
    case 'A': return 1010;
    case 'B': return 1011;
    case 'C': return 1100;
    case 'D': return 1101;
    case 'E': return 1110;
    case 'F': return 1111;
    }
  return 0;
}


int bin_dec(int bin)
{
  int dec = 0, rem, base=1, num = bin;
  while (num > 0)
    {
      rem = num % 10;
      dec = dec + rem * base;
      base = base * 2;
      num = num / 10;
    }
  return dec;
}
