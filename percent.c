#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void removeChar(char *s, int c){ 
	  
	    int j, n = strlen(s); 
	        for (int i=j=0; i<n; i++) 
			       if (s[i] != c) 
				                 s[j++] = s[i]; 
		      
		    s[j] = '\0'; 
} 

void main(){
   FILE *fp;
   int num, x, hits=0, percent;
   char line [150];
   char *line_split;
   unsigned long long total = 0;

   fp = fopen("sites.txt", "r");
   for(int x = 0; x <= 1010; x++){
   fgets (line , 150 , fp);
   //printf("%c", *line);
   line_split = strtok(line, "\t");
   num = 0;
     while (line_split != NULL)
	       {
		           //printf ("%d:\t%s\n", num, line_split);
			   if(num == 2){
				//printf("%d:\t%s\n", hits, line_split);
				removeChar(line_split, ',');
				//printf("%d\n", atoi(line_split));
				total += atoi(line_split);
				//printf("%llu\n", total);
			   }
			       line_split = strtok (NULL, "\t");
			       num++;
	       }
     hits++;
   }
   printf("Total:%llu\n", total);

   fclose(fp);
   //free(line_split);
}
