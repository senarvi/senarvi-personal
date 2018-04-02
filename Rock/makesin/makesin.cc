/* makesin.cpp

   Tekee sini- ja kosinitaulukon */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _2PI            6.28318530718
#define SIN_MUL         (1<<16)
#define SIN_NUM         256

FILE *outputfile;

int init(int numparams,char *paramtab[])
{
  if (numparams < 1)
  {
    printf("usage: makesin <outfile.s>\n");
    return(-1);
  }
  outputfile = fopen(paramtab[1],"w");
  if (outputfile == NULL)
  {
    printf("File not found: %s\n",paramtab[1]);
    return(-1);
  }
  return(0);
}

int main(int argc,char *argv[])
{
  double count;

  if (init(argc,argv) != 0) return(-1);
  fprintf(outputfile,"                .file   \"%s\"\n"
		     "\n"
		     "                .global _sintab,_costab\n"
		     "\n"
		     "                .text\n"
		     "\n"
		     "                .align  4\n"
		     "_sintab:\n",argv[1]);
  for (count=0; count < SIN_NUM/4; count++)
    fprintf(outputfile,"                .long   %#x\n",
	    (signed long)(sin(count/SIN_NUM*_2PI)*SIN_MUL));
  fprintf(outputfile,"\n"
		     "_costab:\n");
  for (count=SIN_NUM/4; count < SIN_NUM; count++)
    fprintf(outputfile,"                .long   %#x\n",
	    (signed long)(sin(count/SIN_NUM*_2PI)*SIN_MUL));
  fprintf(outputfile,"\n");
  for (count=0; count < SIN_NUM/4; count++)
    fprintf(outputfile,"                .long   %#x\n",
	    (signed long)(sin(count/SIN_NUM*_2PI)*SIN_MUL));
  fprintf(outputfile,"\n");
  fclose(outputfile);

  return 0;
}
