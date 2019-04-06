#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

void sed(char* find, char* replace, void* fp)
{
   int find_len = strlen(find);
   char* loc, *tmp;
   int tmp_size = 100;
   tmp = malloc(tmp_size);
   if(tmp==NULL)
   {
      printf("Memory allocation error!");
      exit(1);
   }

   char* buffer = NULL;
   size_t m = 0;
   int n, x;

   while((x=getline(&buffer, &m, fp))!=-1)
   {
      loc = strstr(buffer, find);
      if(loc)
      {
         n = loc - buffer;
         
         if(n>=tmp_size)
         {
            while(n>=tmp_size)
               tmp_size *= 2;
            tmp = realloc(tmp, tmp_size);
            
            if(tmp==NULL)
            {
               printf("Memory allocation error!");
               exit(1);
            }
         }
         if(n>0)
         {
            strncpy(tmp, buffer, n);
            tmp[n] = '\0';
            printf("%s", tmp);
         }
         printf("%s", replace);
         printf("%s", loc + find_len);

      }
      else
         printf("%s", buffer);
   }
}

int main (int argc, char *argv[]) 
{
   if(argc<3)
   {
      printf("my-sed: find_term replace_term [file ...]\n");
      exit(1);
   }
   char* find = argv[1];
   char* replace = argv[2];
   
   if(argc==3)
      sed(find, replace, stdin);
   else 
   {
      FILE *fp;
      for(int i=3; i<argc; i++)
      {
         fp = fopen(argv[i], "r");
         if(fp==NULL)
         {
            printf("my-sed: cannot open file\n");
            exit(1);
         }
         sed(find, replace, fp);
         fclose(fp);
      }
   }
   return 0;
}