#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <strings.h>
#include <string.h>
#include "includes/moves.h"
#include "includes/battle_moves.h"
#include <time.h>
#include "includes/tm.h"
#include "includes/base_stats.h"
#include "42-libft/libft.h"

int	index_mon; //keeping track of indexes in tm learnset so we dont get lost
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

char *get_species(char *line)
{
	int	i = 0;
	char	**tab = NULL;

	tab = ft_split(line, ' ');
	//[species] should be index 0
	return (tab[0]);
}

int pick_choice(int max)
{
	int min = 1; //min is always1, max is 3 or 10
    return rand() % ((max + 1) - min) + min;
}

char	*create_compat(char *species, char **tmlist)
{
	int	i = 0;
	int	j = 0;
	//so species is "[species_] = ". need to add "TMHM_LEARNSETS()" to it somehow, this is where the array comes it
	char	**tm_tab = NULL;
	int		current_tm;
	int		mon_type1;
	int		mon_type2;
	int		move_type;
	tm_tab = (char **)malloc((60 * sizeof(char *)) + 1); //60 cause 50 tms + hm's that go auto at the end.

	while (i < 50)
	{
		current_tm = ft_atoi(tmlist[i]);
		if (gBaseStats[index_mon].type1 == gBattleMoves[current_tm].type || gBaseStats[index_mon].type2 == gBattleMoves[current_tm].type)
		{
			//90% probability of compat
			if (pick_choice(10) != 10)
			{
				//we got a compat tm !
				if (j == 0)
				{
					//case so we dont put |
					tm_tab[j] = ft_strjoin("TMHM(", tmhmDefines[i]);
					tm_tab[j] = ft_strjoin(tm_tab[j], ")");
					j++;
				}
				else //every other tm index
				{
					tm_tab[j] = ft_strjoin("| TMHM(", tmhmDefines[i]);
					tm_tab[j] = ft_strjoin(tm_tab[j], ")");
					j++;
				}
			}
			//sorry try again
		}
		else
		{
			//33% chance of compatible tm
			if (pick_choice(4) == 4)
			{
				//we got a compat tm !
				if (j == 0)
				{
					//case so we dont put |
					tm_tab[j] = ft_strjoin("TMHM(", tmhmDefines[i]);
					tm_tab[j] = ft_strjoin(tm_tab[j], ")");
					j++;
				}
				else //every other tm index
				{
					tm_tab[j] = ft_strjoin("| TMHM(", tmhmDefines[i]);
					tm_tab[j] = ft_strjoin(tm_tab[j], ")");
					j++;
				}
			}
		}
		i++;
	}
	//out of main loop, gotta add HM's now
	while (i < 58)
	{
		tm_tab[j] = ft_strjoin("| TMHM(", tmhmDefines[i]);
		tm_tab[j] = ft_strjoin(tm_tab[j], ")");
		j++;
		i++;
	}
	//tab should be done
	char	*ret = NULL;
	char 	*tmp = NULL;
	tmp = ft_strjoin(convertToLine(j, tm_tab), "),\n");
	ret = ft_strjoin(species, "TMHM_LEARNSET(");
	
	return (ft_strjoin(ret, tmp));
}

int main(void)
{
    printf("Pokeemerald Expansion TM Compat randomiser\nBy XLuma for cool dudes\n");

    const char  *tmhm_path = "src/data/pokemon/tmhm_learnsets.h";
    const char  *tmlist_path = "dist/tmlist.txt";
    const char  *new_tmhm = "dist/tmhm_learnsets.h";

    sleep(1);
	srand(time(NULL));
    //opening files
    FILE  *file_tmhm;
    FILE  *file_tmlist;
    FILE  *file_new_tmhm;

    file_tmhm = fopen(tmhm_path, "rb");
    file_tmlist = fopen(tmlist_path, "rb");
    file_new_tmhm = fopen(new_tmhm, "w+");
    if (file_new_tmhm == NULL || file_tmlist == NULL || file_tmhm == NULL)
    {
      printf("Error: Cannot open file. make sure tmhm_learnsets, tmlist.txt exists and are in their expected path\n");
      fclose(file_tmhm);
      fclose(file_tmlist);
      fclose(file_new_tmhm);
      return 1;
    }
    //get sizes

    long  size_tmhm;
    long  size_tmlist;

    fseek(file_tmhm, 0L, SEEK_END);
    fseek(file_tmlist, 0L, SEEK_END);

    size_tmhm = ftell(file_tmhm);
    size_tmlist = ftell(file_tmlist);
	rewind(file_tmlist);
	rewind(file_tmhm);

	printf("Allocating mem for files\n");
	char	**tab_tmhm;
	char	**tab_tmlist;
	tab_tmhm = (char **)malloc(size_tmhm + 1);
	tab_tmlist = (char **)malloc((51 * sizeof(char *)) + 1);
	long lenght;
	int i = 0;
	char *line = NULL;

	printf("Parsing files\n");
	while (!feof(file_tmhm))
	{
		getline(&line, &lenght, file_tmhm);
		tab_tmhm[i] = ft_strdup(line);
		i++;
	}
	i = 0;
	while (!feof(file_tmlist))
	{
		getline(&line, &lenght, file_tmlist);
		tab_tmlist[i] = ft_strdup(line);
		i++;
	}
	i = 0;
	printf("Randomizing TM compatibility...\n");
	index_mon = 0;
	while (tab_tmhm[i])
	{
		while (i < 18)
		{
			fputs(ft_strdup(tab_tmhm[i]), file_new_tmhm);
			i++;
		}
		if (ft_strnstr(tab_tmhm[i], "SPECIES", ft_strlen(tab_tmhm[i])) != NULL && index_mon <= 808 )
		{
			char	*species = NULL;
			index_mon++;
			species = ft_strjoin(get_species(tab_tmhm[i]), " = ");
			fputs(create_compat(species, tab_tmlist), file_new_tmhm);
			i++;
		}
		else
		{
			if (index_mon > 808)
				fputs(ft_strdup(tab_tmhm[i]), file_new_tmhm);
			i++;
		}
	}
	printf("Randomised TM Compat ! Output files can be found in dist/\nMake sure to replace both party_menu.h and tmhm_learnsets.h in src/data !\n");
	free(tab_tmlist);
	free(tab_tmhm);
	fclose(file_new_tmhm);
	fclose(file_tmhm);
	fclose(file_tmlist);
	return 0;
}