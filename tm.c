//make it so we also do compat right here
//only compatible with the pokeemerald-expansion project
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "includes/moves.h"
#include "includes/battle_moves.h"
#include <time.h>
#include "includes/tm.h"
#include "includes/base_stats.h"
#include "42-libft/libft.h"

int tms[50];
int tm_index_max;
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

int	check_index(int move)
{
	int	i;
	int j;
	int tmp;

	tmp = 0;
	i = 0;
	j = 0;
	while(tms[j] && j <= tm_index_max)
	{
		if (move == tms[j])
		{
			printf("Duplicate index: %d\n", move); //print dupe move number
			return (1);
		}
		j++;
	}
	return 0;
}

int pick_choice()
{
    int max = 677;
	int min = 1; //min is always1, max is 3 or 10
    int move = 0;
    int i = 0;
    move = rand() % ((max + 1) - min) + min;
    while (check_index(move) != 0)
    {
        move = rand() % ((max + 1) - min) + min;
    }
    tms[tm_index_max] = move;
    tm_index_max++;
    return move;
}

char *format_tm(char *line)
{
    int i = 0;
    while (line[i])
    {
        if (line[i] == ' ' || line[i] == '-')
            line[i] = '_';
        else
        {
           line[i] = ft_toupper(line[i]);
        }
        i++;
    }
    return (line);
}

char    *new_tm(char *line, char **movelist, FILE *tmList)
{
    int i = 0;
    int j = 0;
    char    **tab = NULL;
    int move;
    char    *move_name;
    char    *ret;
    
    tab = ft_split(line, ' ');
    //move is always [4]
    move = pick_choice();//should not pick anything gen8
    move_name = ft_strjoin("MOVE_", format_tm(movelist[move]));
    fputs(ft_strjoin(ft_itoa(move), "\n"), tmList);
    tab[4] = ft_strdup(move_name);
    j = 0;
    while (tab[j])
        j++;
    ret = convertToLine(j, tab);
    ret = ft_strjoin("    ", ret);
    j = 0;
    return (ret);
}

int main(void)
{
    srand((unsigned int)time(NULL)); //set random seed
    printf("Pokeemerald expansion TM randomiser\nBy XLuma for cool dudes\n");
    const char *party_menu_path = "src/data/party_menu.h";
    const char  *new_partymenu_path = "dist/party_menu.h";
    const char  *movelist_file = "movelist.txt";
    const char  *newtmlist = "dist/tmlist.txt";
    //const char *tmhm_learnsets_path = "src/data/pokemon/tmhm_learnsets.h";
    sleep(1);
    //check for em
    FILE    *partymenu;
    FILE    *new_partymenu;
    FILE    *movelist;
    FILE    *tmlist;
    char    **tab_partymenu;
    char    **tab_movelist;
    int i;
    partymenu = fopen(party_menu_path, "rb");
    new_partymenu = fopen(new_partymenu_path, "w+");
    movelist = fopen(movelist_file, "rb");
    tmlist = fopen(newtmlist, "w+");
    if (partymenu == NULL)
    {
        printf("Error: cannot open party_menu.h\nMake sure that the files exists and are in the correct paths\n");
        fclose(partymenu);
        return 1;
    }

    //get sizes
    long    partymenu_size;
    long    movelist_size;

    fseek(partymenu, 0L, SEEK_END);
    fseek(movelist, 0L, SEEK_END);

    partymenu_size = ftell(partymenu);
    movelist_size = ftell(movelist);
    rewind(movelist);
    rewind(partymenu);
    //rewind(tmhm);

    printf("Allocating mem for parsing party_menu.h\n");
    tab_partymenu = (char **)malloc(partymenu_size + 1);
    tab_movelist = (char **)malloc(movelist_size + 1);
    long lenght;
    i = 0;
    char    *line = NULL;
    printf("Parsing file\n");
     while (!feof(partymenu))
    {
        getline(&line, &lenght, partymenu);
        tab_partymenu[i] = ft_strdup(line);
        i++;
    }

    i = 0;
    line = NULL;
     while (!feof(movelist))
    {
        getline(&line, &lenght, movelist);
        tab_movelist[i] = ft_strdup(line);
        i++;
    }

    i = 0;
    tm_index_max = 0;
    printf("Randomizing TM's...\n");
    while (tab_partymenu[i])
    {
        while (i >= 1182 && i < 1232)
        {
            //1182 is the index for the first tm def so from there ez shit
            fputs(new_tm(tab_partymenu[i], tab_movelist, tmlist), new_partymenu);
            i++;
        }
        //write normally
        fputs(ft_strdup(tab_partymenu[i]), new_partymenu);
        i++;
    }
    printf("Randomised TM's ! Make sure to run the TM compatibility randomiser !\n");
    free(tab_partymenu);
    free(tab_movelist);
    fclose(movelist);
    fclose(partymenu);
    fclose(tmlist);
    fclose(new_partymenu);
    return 0;
}