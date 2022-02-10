//export item names from items.h to a list
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <strings.h>
#include <string.h>
#include <time.h>
#include "includes/tm.h"
#include "42-libft/libft.h"

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

char *retrieve_name(char *line)
{
    int i = 0;
    char **tab = NULL;
    char *ret = NULL;

    tab = ft_split(line, ' ');
    ret = ft_strdup(tab[2]);
    return ret;
}

int main(void)
{
    FILE *file;
    FILE *newfile;
    const char *path = "includes/items.h";
    const char *path_newfile = "item_names.txt";
    char **item_tab;
    int i;

    file = fopen(path, "rb");
    newfile = fopen(path_newfile, "w");
    if (file == NULL || newfile == NULL)
    {
        printf("Error: Cannot open items.h. Make sure the file is present in the includes folder\n");
        fclose(file);
        fclose(newfile);
        return 1;
    }

    //get sizes
    long item_size;
    fseek(file, 0L, SEEK_END);
    item_size = ftell(file);
    rewind(file);

    //read the header file
    printf("Allocating mem for items.h\n");
    item_tab = (char **)malloc((item_size * sizeof(char *)) + 1);
    long lenght;
    char *buf = NULL;
    printf("Parsing file\n");
    i = 0;
    while (!feof(file))
    {
        getline(&buf, &lenght, file);
        item_tab[i] = ft_strdup(buf);
        i++;
    }
    i = 0;
    printf("Reading file...\n");
    while(item_tab[i])
    {
        //go on every .itemId, and output to a new file
        if (ft_strnstr(item_tab[i], ".itemId", ft_strlen(item_tab[i])) != NULL)
        {
            fputs(retrieve_name(item_tab[i]), newfile);
        }
        i++;
    }
    fclose(file);
    fclose(newfile);
    free(item_tab);
    printf("Exported item_names.txt!\n");
    return 0;
}