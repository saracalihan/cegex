#include <stdio.h>
// #include "test.h"
#include "cegex.h"


int main()
{

// Tests({
//   Describe("+ operation", {
//       Test("5 + 2 should equal to 7", {
//           int a = 5;
//           int b = 2;
//           a + b == 7; // return value
//       });
//       Test("This test fail", {
//           int a = 5;
//           int b = 2;
//           a + b == 8; // return value
//       });
//   });

//   Describe("- operation", {
//     int a = 5;
//     int b = 2;
//     Test("5 - 2 should equal to 3", {
//         a - b == 3; // return value
//     });
//   });
// });
  // printf("C: %i\n",c);
  char* regex = ".A?A*.c?", *text ="1AAbcd_efZxxx";
  CegexValue res = cegex_match(regex, text);

  printf("regex: '%s'\ntext: '%s'\nindex: %i\nlength: %i\n", regex, text, res.index, res.length);
  return 0;
}
