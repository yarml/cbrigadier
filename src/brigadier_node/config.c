#include <brigadier.h>
#include <limits.h>
#include <stdlib.h>
#include <float.h>

brigadier_node *brigadier_node_new(char *name, char *prompt, char *children_prompt)
{
  brigadier_node *node = calloc(1, sizeof(brigadier_node));
  node->name = name;
  node->prompt = prompt;
  node->child_prompt = children_prompt;
  return node;
}

void brigadier_node_clean(brigadier_node *node)
{
  if(!node)
    return;

  // We only need to clean the first child, the first child will itself
  // clean its siblings
  brigadier_node_clean(node->children);

  // Clean our sibling
  brigadier_node_clean(node->next);

  // Clean ourselves
  free(node);
}

void brigadier_node_literal(brigadier_node *node, char *literal)
{
  node->type = BRIGADIER_LITERAL;
  node->literal = literal;
}

void brigadier_node_int(brigadier_node *node)
{
  node->type = BRIGADIER_INT;
  node->min = INT_MIN;
  node->max = INT_MAX;
}

void brigadier_node_int_extra(brigadier_node *node, int min, int max)
{
  node->type = BRIGADIER_INT;
  node->min = min;
  node->max = max;
}
void brigadier_node_float(brigadier_node *node)
{
  node->type = BRIGADIER_FLOAT;
  node->fmin = FLT_MIN;
  node->fmax = FLT_MAX;
}
void brigadier_node_float_extra(brigadier_node *node, float fmin, float fmax)
{
  node->type = BRIGADIER_FLOAT;
  node->fmin = fmin;
  node->fmax = fmax;
}
void brigadier_node_str(brigadier_node *node)
{
  node->type = BRIGADIER_STR;
  node->f_validate_string = 0;
}

void brigadier_node_str_extra(
    brigadier_node *node,
    brigadier_validate_string_func f_validate_str)
{
  node->type = BRIGADIER_STR;
  node->f_validate_string = f_validate_str;
}

void brigadier_node_enum(
    brigadier_node *node,
    brigadier_enum_display_name_func f_display_name,
    int enum_len)
{
  node->type = BRIGADIER_ENUM;
  node->f_display_name = f_display_name;
  node->enum_len = enum_len;
}
