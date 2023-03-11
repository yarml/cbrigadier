#include <brigadier.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// Takes a node and a token and returns a pointer to a command node
// if it matches, otherwise, returns 0
brigadier_cmd_node *brigadier_node_matches(brigadier_node *node, char *token)
{
  size_t token_len = strlen(token);
  switch (node->type)
  {
  default:
    return 0;
  case BRIGADIER_LITERAL:
    {
      if(!strcmp(node->literal, token))
      {
        brigadier_cmd_node *cmd_node = calloc(1, sizeof(brigadier_cmd_node));
        cmd_node->node = node;
        return cmd_node;
      }
      return 0;
    }
  case BRIGADIER_STR:
    {
      if(node->f_validate_string && !node->f_validate_string(token))
        return 0;
      brigadier_cmd_node *cmd_node = calloc(1, sizeof(brigadier_cmd_node));
      cmd_node->node = node;
      cmd_node->str = malloc(token_len + 1);
      strcpy(cmd_node->str, token);
      return cmd_node;
    }
  case BRIGADIER_INT:
    {
      // If token is empty string, this can't be a number
      if(!token_len)
        return 0;
      // Next, we check that ALL characters in token are digits,
      // or -/+ for the first character,
      // if so, it is a number
      for (size_t i = 0; i < token_len; ++i)
        if(
            !isdigit(token[i]) || (i == 0 && (token[i] == '-' || token[i] == '+')))
          return 0;
      // If it is all digits, we need to check if it respects the bounds
      // Convert the number to int
      int num = 0;
      for (size_t i = 0; i < token_len; ++i)
        if(isdigit(token[i]))
        {
          num *= 10;
          num += token[i] - '0';
        }
      if(token[0] == '-')
        num *= -1;
      if(!(node->min <= num && num <= node->max))
        return 0;
      brigadier_cmd_node *cmd_node = calloc(1, sizeof(brigadier_cmd_node));
      cmd_node->node = node;
      cmd_node->val = num;
      return cmd_node;
    }
  case BRIGADIER_FLOAT:
    {
      // Check that the string is a float
      // Copied from https://stackoverflow.com/a/45554707/21296545
      int len;
      float fval;
      int ret = sscanf(token, "%f %n", &fval, &len);
      if(!(ret==1 && !token[len]))
      {
        // Valid float
        brigadier_cmd_node *cmd_node = calloc(1, sizeof(brigadier_cmd_node));
        cmd_node->node = node;
        cmd_node->fval = fval;
        return cmd_node;
      }
      return 0;
    }
  case BRIGADIER_ENUM:
    {
      // Check all enum elements, if one of them matches token, then this node
      // matches the token
      for (size_t i = 0; i < node->enum_len; ++i)
      {
        if(!strcmp(token, node->f_display_name(i)))
        {
          brigadier_cmd_node *cmd_node = calloc(1, sizeof(brigadier_cmd_node));
          cmd_node->node = node;
          cmd_node->val = i;
          return cmd_node;
        }
      }
      // If no enum element matches the token, then this node does not match
      // the token
      return 0;
    }
  }
}