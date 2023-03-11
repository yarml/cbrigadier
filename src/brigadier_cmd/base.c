#include <brigadier.h>
#include <stdlib.h>
#include <stddef.h>

void brigadier_cmd_ctx_release(brigadier_cmd_ctx *ctx)
{
  brigadier_cmd_node *current = ctx;
  while (current)
  {
    brigadier_cmd_ctx *next = current->next;
    if(current->node->type == BRIGADIER_STR)
      free(current->str);
    free(current);
    current = next;
  }
}
char *brigadier_cmd_get_str(brigadier_cmd_ctx *ctx, int idx)
{
  // Travel the command chain until idx, then return the associated
  // string, no type checking is done
  brigadier_cmd_node *current = ctx;
  for (size_t i = 0; i < idx; ++i)
  {
    if(!current)
      return 0;
    current = current->next;
  }
  return current->str;
}
int brigadier_cmd_get_val(brigadier_cmd_ctx *ctx, int idx)
{
  // Travel the command chain until idx, then return the associated
  // string, no type checking is done
  brigadier_cmd_node *current = ctx;
  for (size_t i = 0; i < idx; ++i)
  {
    if(!current)
      return 0;
    current = current->next;
  }
  return current->val;
}

float brigadier_cmd_get_fval(brigadier_cmd_ctx *ctx, int idx)
{
  // Travel the command chain until idx, then return the associated
  // string, no type checking is done
  brigadier_cmd_node *current = ctx;
  for (size_t i = 0; i < idx; ++i)
  {
    if(!current)
      return 0;
    current = current->next;
  }
  return current->fval;
}
