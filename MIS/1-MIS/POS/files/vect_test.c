#include "str_vector.h"

int main(void)
{
  struct StrVector v;
  v_init(&v);
  v_append(&v, "prvy");
  v_append(&v, "druhy");
  v_append(&v, "treti");
  v_append(&v, "stvrty");
  v_print(&v);
  char **x = v_to_a(&v);
  printf("Char array\n");
  for(int i=0; i<v_size(&v); ++i)
    printf("%s\n", x[i]);
  printf("End char array\n");

  printf("Size before destroy: %d\n", v_size(&v));
  v_destroy(&v);
  printf("Size after destroy: %d\n", v_size(&v));
  v_append(&v, "new one");
  v_append(&v, "new two");
  v_print(&v);
  printf("Size before destroy: %d\n", v_size(&v));
  v_destroy(&v);
  printf("Size after destroy: %d\n", v_size(&v));
  return 0;
}
