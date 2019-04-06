#include <stdio.h>
#include <stdlib.h>
 

int main (int argc, char *argv[]) 
{
   FILE *fp;
   char buffer[100];
   // printf("%s", argv[1]);

   for(int i=1; i<argc; i++)
   {
      fp = fopen(argv[i], "r");

      if(fp==NULL)
      {
         printf("my-cat: cannot open file\n");
         exit(1);
      }
      
      while(fgets(buffer, 100, fp)!=NULL)
      {
         printf("%s", buffer);
      }

      fclose(fp);
   } 
   return 0;
}