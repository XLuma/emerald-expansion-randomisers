#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "includes/moves.h"
#include "includes/battle_moves.h"
#include <time.h>
#include "includes/tm.h"
#include "includes/base_stats.h"
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