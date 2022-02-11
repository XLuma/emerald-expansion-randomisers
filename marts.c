//Only affect special marts/secondary marts, we dont touch npc's in regular marts
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <strings.h>
#include <string.h>
#include <time.h>
#include "includes/tm.h"
#include "42-libft/libft.h"

const char *mart_paths[] = {
  "data/maps/LavaridgeTown_HerbShop/scripts.inc",
  "data/maps/LilycoveCity_DepartmentStore_2F/scripts.inc",
  "data/maps/LilycoveCity_DepartmentStore_3F/scripts.inc",
  "data/maps/LilycoveCity_DepartmentStore_4F/scripts.inc",
  "data/maps/SlateportCity/scripts.inc", //Energy guru and power tm
};

char *convertToLine(int n, char **input) {
  /* compute the needed size,
     of course can also use malloc then realloc to avoid that */
  int size = 0;
  int i;

  for (i = 0; i != n; ++i)
    size += strlen(input[i]) + 1;

  /* copy the strings */
  char * string = (char*)malloc(size); /* sizeof(char) is 1 by definition */
  char * p = string;

  for (i = 0; i != n; ++i) 
  {
	  strcpy(p, input[i]);
	  p += strlen(p);
	  *p++ = ' ';
  }
  p[-1] = 0;

  return string;
}

int main(void)
{
  srand(time(NULL));
  //Use our array of strings, and simply... while loop through all of them. Need to recreate file tree for ease of replacing

  FILE *current_file;
  FILE *current_new_file;
  long size_current;
  char *new_file;

  char **file_tab;
  long  lenght;
  char *line = NULL;

  int i = 0;
  while (mart_paths[i]) //loop through all paths
  {
    current_file = fopen(mart_paths[i], "rb");
    new_file = ft_strjoin("dist/", mart_paths[i]);
    current_new_file = fopen(new_file, "w+");

    if (current_new_file == NULL || current_file == NULL)
    {
      printf("Cannot open a file ! Make sure the the file exists at its proper source\n");
      printf("Current file: %s\n", mart_paths[i]);
      fclose(current_file);
      fclose(current_new_file);
      return 1;
    }
	fseek(current_file, 0L, SEEK_END);
	size_current = ftell(current_file);
	rewind(current_file);
	file_tab = (char **)malloc((size_current * sizeof(char *)) + 1);
	while (!feof(current_file))
	{
		getline(&line, &lenght, current_file);
		file_tab[i] = ft_strdup(line);
		i++;
	}
	i = 0;

    i++;
  }
  return 0;
}