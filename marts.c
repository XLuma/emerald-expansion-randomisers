//Only affect special marts/secondary marts, we dont touch npc's in regular marts
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <strings.h>
#include <string.h>
#include <time.h>
#include "includes/tm.h"
#include "42-libft/libft.h"
#include "includes/define_items.h"

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

char **parse_items(char **item_names)
{
	FILE *item_file;

	item_file = fopen("item_names.txt", "rb");
	if (item_file == NULL)
	{
		fclose(item_file);
		return NULL;
	}
	long item_size;
	fseek(item_file, 0L, SEEK_END);
	item_size = ftell(item_file);
	rewind(item_file);
	item_names = (char **)malloc((item_size * sizeof(char *)) + 1);
	int i = 0;
	char *buf = NULL;
	long lenght;
	while (!feof(item_file))
	{
		getline(&buf, &lenght, item_file);	
		item_names[i] = ft_strdup(buf);
		i++;
	}
	fclose(item_file);
	return item_names;
}

char *get_mart_name(char *line)
{
	char **tmp = NULL;
	int i = 0;

	tmp = ft_split(line, ' ');
	//splitted line, so last index is the name. so index [1]
	while (tmp[1][i] != '\n')
	{
		i++;
	}
	tmp[1][i] = ':';
	return(tmp[1]);
}

int	count_2bytes(char **tab, int index)
{
	int tmp_index = index;
	int ret = 0;
	while (ft_strnstr(tab[tmp_index], "release", ft_strlen(tab[tmp_index])) == NULL) //loop until we find release
	{
		if (ft_strnstr(tab[tmp_index], ".2byte", ft_strlen(tab[tmp_index])) != NULL) //found .2byte
			ret++;
		tmp_index++;
	}
	return ret;
}

int is_legal(int item)
{
	if (item == ITEM_NONE || (item >= ITEM_FIGY_BERRY && item <= ITEM_ROWAP_BERRY) || item > ITEM_TM50 || (item >= ITEM_LUCKY_PUNCH && item <= ITEM_YELLOW_SCARF) || item == ITEM_SHOAL_SALT || item == ITEM_SHOAL_SHELL || (item >= ITEM_GROWTH_MULCH && item <= ITEM_RETRO_MAIL) || item == ITEM_ODD_KEYSTONE || (item >= ITEM_NORMALIUM_Z && item <= ITEM_ULTRANECROZIUM_Z) || item == ITEM_LIGHT_BALL || item == ITEM_ORAN_BERRY)
		return 1;
	return 0;
}

int select_item(void)
{
	int ret;

	ret = rand() % ((759 + 2) - 2) + 2;
	while (is_legal(ret) == 1)
	{
		ret = rand() % ((759 + 2) - 2) + 2;
	}
	return ret;
}
char *new_item(char **item_names)
{
	int item;
	char *ret;

	item = select_item();
	while (ft_strnstr(item_names[item], "ITEM_NONE", ft_strlen(item_names[item])) != NULL)
	{
		item = select_item();
	}
	printf("item: %d\n", item);
	ret = ft_strdup(item_names[item]);
	ret[ft_strlen(ret) - 1] = '\n'; //should change the comma to a newline char

	ret  = ft_strjoin("	.2byte ", ret);
	return ret;
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
	char	**item_names;
	long  lenght;
	char *line = NULL;

	item_names = NULL;
	item_names = parse_items(item_names);
	if (item_names == NULL)
	{
		return 1;
	}
	int i = 2;
	int j = 0;
	while (mart_paths[i]) //loop through all paths
	{
		current_file = fopen(mart_paths[i], "rb");
		new_file = ft_strjoin("dist/", mart_paths[i]);
		current_new_file = fopen(new_file, "w+");

		if (current_new_file == NULL || current_file == NULL)
		{
			printf("Cant open file: Current file: %s\n", mart_paths[i]);
			fclose(current_file);
			fclose(current_new_file);
			return 1;
		}
		fseek(current_file, 0L, SEEK_END);
		size_current = ftell(current_file);
		rewind(current_file);
		file_tab = (char **)malloc((size_current * sizeof(char *)) + 1);
		j = 0;
		while (!feof(current_file))
		{
			getline(&line, &lenght, current_file);
			file_tab[j] = ft_strdup(line);
			j++;
		}
		j = 0;
		printf("Randomizing %s...\n", mart_paths[i]);

		free(line);
		line = NULL; //resetting our line buf for later

		int nb_2bytes = 0; //line counter for items
		while(file_tab[j])
		{
			/* Loop through the entire file. for each "pokemart " we find, keep the value in memory somehow.
			and we KEEP searching for other pokemarts while we output to the new file to avoid possible conflitcs.
			*/

			if (ft_strnstr(file_tab[j], "pokemart ", ft_strlen(file_tab[j])) != NULL) //found "pokemart "! space to avoid pokemartdecoration
			{
				line = get_mart_name(file_tab[j]);
				line = ft_strjoin(line, "\n");
				//fputs(file_tab[j], current_new_file); //have to force writing since we arent near the shop yet.
			}
			if (line != NULL && ft_strnstr(file_tab[j], line, ft_strlen(file_tab[j])) != NULL) //if we got the shop name. hopefully this works and wont do weird shit
			{
				fputs(file_tab[j], current_new_file); //still gotta force write the first line
				/* So here we need to somehow parse exactly n lines with ".2byte" in them until "release".
				Best solution is to probably create an isolated counter from this function and return the number of 2byte found.
				Shouldn't affect our main pointer increment ? */
				nb_2bytes = count_2bytes(file_tab, j); //i stays the same hopefully
				int tmp_counter = 0;
				while (tmp_counter < nb_2bytes - 1)// - 1 to stay protected against .2byte ITEM_NONE
				{
					/* Loop for nb_2bytes time, creating a new .2byte string
					on each iteration, then writing it. more efficient than trying to edit an entire double
					array of them, since we only care about when it end */
					fputs(new_item(item_names), current_new_file);
					tmp_counter++;
					j++;//this one is so were jumping over all of em
				}
			}
			else
			{
				fputs(file_tab[j], current_new_file);
			}
			j++;
		}
		/* So here, we should have written a new file with new items. So we have to free all current mem used, and making sure its all good for next file */
		free(file_tab);
		fclose(current_file);
		fclose(current_new_file);
		i++;
	}
	free(item_names);
	printf("Done !\n");
	return 0;
}