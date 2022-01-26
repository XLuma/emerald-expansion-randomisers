#include "libft.h"

char *ft_convertToLine(int n, char **input) {
  /* compute the needed size,
     of course can also use malloc then realloc to avoid that */
  int size = 0;
  int i;

  for (i = 0; i != n; ++i)
    size += ft_strlen(input[i]) + 1;

  /* copy the strings */
  char * string = (char*)malloc(size); /* sizeof(char) is 1 by definition */
  char * p = string;

  for (i = 0; i != n; ++i) 
  {
	  ft_strlcpy(p, input[i], ft_strlen(input[i]));
	  p += ft_strlen(p);
	  *p++ = ' ';
  }
  p[-1] = 0;

  return string;
}