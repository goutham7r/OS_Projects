#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


void uniq(void* fp)
{
   char* prev_buffer = "";
   char* cur_buffer = NULL;
   size_t n = 0;

   while(getline(&cur_buffer, &n, fp)!=-1)
   {
      if(strcmp(cur_buffer,prev_buffer)!=0)
         printf("%s", cur_buffer);
      prev_buffer = cur_buffer;
      cur_buffer = NULL;
   }
}

int main (int argc, char *argv[]) 
{
   
   if(argc==1)
      uniq(stdin);
   else 
   {
      FILE *fp;
      for(int i=1; i<argc; i++)
      {
         fp = fopen(argv[i], "r");
         if(fp==NULL)
         {
            printf("my-uniq: cannot open file\n");
            exit(1);
         }

         uniq(fp);

         fclose(fp);
      }
   }
   return 0;
}