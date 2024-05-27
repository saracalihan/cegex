#include <stdio.h>
#include "cegex.h"

int main()
{
  // char* regex = ".A?A*a", *text ="1AAabcd_efZxxx";
  char* regex = "z(a|(b)+)x", *text ="zbbbx";
  CegexValue res = cegex_match(regex, text);

  printf("regex: '%s'\ntext: '%s'\nindex: %i\nlength: %i\n", regex, text, res.index, res.length);
  return 0;
}
