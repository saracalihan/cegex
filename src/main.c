#include <stdio.h>

#include "cegex.h"

int main(int argc, char *argv[])
{
  char* regex = "A*Z(x.abc+)?X?", *text ="1234Abcd_efZxxx";
  CegexValue res = cegex_match(regex, text);

  printf("regex: '%s'\ntext: '%s'\nindex: %i\nlength: %i\n", regex, text, res.index, res.length);
  return 0;
}
