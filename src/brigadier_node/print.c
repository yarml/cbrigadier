#include <brigadier.h>
#include <stdio.h>
#include <stddef.h>

void brigadier_print_node(brigadier_node *node)
{
  printf("%s: ", node->name);
  switch (node->type)
  {
  case BRIGADIER_LITERAL:
    printf("literal(\"%s\")", node->literal);
    break;
  case BRIGADIER_STR:
    printf("str");
    break;
  case BRIGADIER_INT:
    printf("int(%d, %d)", node->min, node->max);
    break;
  case BRIGADIER_FLOAT:
    printf("float(%d, %d)", node->fmin, node->fmax);
    break;
  case BRIGADIER_ENUM:
    printf("enum{ ");
    for (size_t i = 0; i < node->enum_len; ++i)
    {
      printf("%s ", node->f_display_name(i));
    }
    printf("}");
    break;
  case BRIGADIER_ROOT:
    printf("root");
    break;
  }

  // If node has action attached, print an X near its name
  if(node->action)
    printf(" X");
}
