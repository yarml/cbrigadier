#include <brigadier.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Implementation copied from
// https://github.com/ivanrad/getline/blob/master/getline.c
// We don't directly use getline, because it is not available in Windows
static ssize_t brigadier_internal_getdelim(
  char **lineptr,
  size_t *n,
  int delim,
  FILE *stream
) {
  char *cur_pos, *new_lineptr;
  size_t new_lineptr_len;
  int c;

  if(lineptr == NULL || n == NULL || stream == NULL)
  {
    return -1;
  }

  if(*lineptr == NULL)
  {
    *n = 128; /* init len */
    if((*lineptr = (char *)malloc(*n)) == NULL)
    {
      return -1;
    }
  }

  cur_pos = *lineptr;
  for (;;)
  {
    c = getc(stream);

    if(ferror(stream) || (c == EOF && cur_pos == *lineptr))
      return -1;

    if(c == EOF)
      break;

    if((*lineptr + *n - cur_pos) < 2)
    {
      if(SSIZE_MAX / 2 < *n)
      {
        return -1;
      }
      new_lineptr_len = *n * 2;

      if((new_lineptr = (char *)realloc(*lineptr, new_lineptr_len)) == NULL)
      {
        return -1;
      }
      cur_pos = new_lineptr + (cur_pos - *lineptr);
      *lineptr = new_lineptr;
      *n = new_lineptr_len;
    }

    *cur_pos++ = (char)c;

    if(c == delim)
      break;
  }

  *cur_pos = '\0';
  return (ssize_t)(cur_pos - *lineptr);
}

static ssize_t brigadier_internal_getline(char **lineptr, size_t *n, FILE *stream)
{
  return getdelim(lineptr, n, '\n', stream);
}

static void brigadier_internal_clearline()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

char *brigadier_execute(brigadier_ctx *ctx, char *cmdline)
{
  char *token = strtok(cmdline, " \t");
  brigadier_node *target_node = ctx->root;

  brigadier_cmd_ctx *cmd_ctx = calloc(1, sizeof(brigadier_cmd_node));
  brigadier_cmd_ctx *cmd_ctx_tail = cmd_ctx;

  cmd_ctx->node = ctx->root;

  while (token)
  {
    // If target node has no children nodes, and we still have a token, then
    // that token is trailing
    if(!target_node->children)
    {
      printf("Trailing token `%s`. Expected end of command.\n", token);
      brigadier_cmd_ctx_release(cmd_ctx);
      // Execute nothing
      return "Syntax Error";
    }

    // Search in children nodes for a node that matches with this token
    brigadier_node *new_target = 0;
    brigadier_node *current = target_node->children;
    while (current)
    {
      brigadier_cmd_node *new_cmd_node = brigadier_node_matches(current, token);
      if(new_cmd_node)
      {
        new_target = current;
        cmd_ctx_tail->next = new_cmd_node;
        cmd_ctx_tail = new_cmd_node;
        break;
      }
      current = current->next;
    }

    // If no children node matches the token, then it's an unexpected token
    if(!new_target)
    {
      printf("Unrecognized token `%s`.\n", token);
      // Printf a list of expectations
      brigadier_node *current = target_node->children;
      printf("Expected one of:\n");
      while (current)
      {
        printf("\t");
        brigadier_print_node(current);
        puts("");
        current = current->next;
      }
      brigadier_cmd_ctx_release(cmd_ctx);
      // Execute nothing
      return "Syntax Error";
    }

    target_node = new_target;
    token = strtok(0, " \t");
  }

  // When loop ends, there are two possibilities
  // target_node is executable, if so execute it
  // Otherwise we need to prompt the user for the next inputs
  // until we reach an executable node
  while (!target_node->action) // While target node is not executable
                               // prompt user for more info
  {
    // First, if target node has multiple children, we need
    // to know which one the user wants
    size_t children_count = 0;
    brigadier_node *current_child = target_node->children;
    while (current_child)
    {
      ++children_count;
      current_child = current_child->next;
    }

    if(!children_count) // No children and not executable,
                         // dead branch, shouldn't happen
    {
      brigadier_cmd_ctx_release(cmd_ctx);
      return "Nothing";
    }

    size_t target_child_idx = 0;

    if(children_count > 1)
    {
      // Print a menu with each child's name, and let the user choose
      while (1)
      {
        printf("%s\n", target_node->child_prompt);
        int idx = 0;
        current_child = target_node->children;
        while (current_child)
        {
          printf("%d. %s\n", idx + 1, current_child->name);
          current_child = current_child->next;
          ++idx;
        }
        int in;
        scanf("%d", &in);
        brigadier_internal_clearline();
        target_child_idx = in - 1;
        if(target_child_idx >= 0 && target_child_idx < children_count)
          break;
        printf("Invalid choice `%lu`. Try again.\n", target_child_idx + 1);
      }
    }

    brigadier_node *target_child = target_node->children;

    for (size_t i = 0; i < target_child_idx; ++i)
      target_child = target_child->next;

    // Now that we have target_child, we need to get
    // the data that that node stores
    switch (target_child->type)
    {
    default:
      break;
    case BRIGADIER_LITERAL:
      {
        // Make CMD node
        brigadier_cmd_node *new_node = calloc(1, sizeof(brigadier_cmd_node));
        new_node->node = target_child;
        cmd_ctx_tail->next = new_node;
        cmd_ctx_tail = new_node;
      }
      break;
    case BRIGADIER_STR:
    case BRIGADIER_INT:
      {
        // Ask the user for input until they give a valid string/int
        char *input = 0;
        size_t n = 0;

        while(1)
        {
          printf("%s: ", target_child->prompt);
          brigadier_internal_getline(&input, &n, stdin);
          input[strlen(input) - 1] = 0; // Remove trailing new line
          brigadier_cmd_node *new_node = brigadier_node_matches(
            target_child,
            input
          );
          if(new_node)
          {
            free(input);
            cmd_ctx_tail->next = new_node;
            cmd_ctx_tail = new_node;
            break;
          }
          printf("Invalid input `%s`. Try again.\n", input);
        }
      }
      break;
    case BRIGADIER_ENUM:
      {
        int idx;
        while(1)
        {
          // Display a menu of enum elements
          for(size_t i = 0; i < target_child->enum_len; ++i)
            printf("%lu. %s\n", i+1, target_child->f_display_name(i));
          scanf("%d", &idx);
          brigadier_internal_clearline();
          --idx;
          if(idx >= 0 && idx < target_child->enum_len)
            break;
          printf("Invalid option `%d`. Try again.\n", idx+1);
        }

        // Make cmd node
        brigadier_cmd_node *new_node = calloc(1, sizeof(brigadier_cmd_node));
        new_node->node = target_child;
        new_node->val = idx;

        cmd_ctx_tail->next = new_node;
        cmd_ctx_tail = new_node;
      }
      break;
    }

    target_node = target_child;
  }
  char *result = target_node->action(cmd_ctx);

  brigadier_cmd_ctx_release(cmd_ctx);
  return result;
}