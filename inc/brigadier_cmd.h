#ifndef BRIGADIER_CMD_H
#define BRIGADIER_CMD_H

struct BRIGADIER_CMD_NODE;
typedef struct BRIGADIER_CMD_NODE brigadier_cmd_node;
typedef brigadier_cmd_node brigadier_cmd_ctx;

#include <brigadier_node.h>

struct BRIGADIER_CMD_NODE
{
  brigadier_node *node;
  // union decided by node->type
  union
  {
    char *str; // BRIGADIER_STR
    int val;   // BRIGADIER_INT, BRIGADIER_ENUM
    float fval; // BRIGADIER_FLOAT
  };

  brigadier_cmd_node *next;
};

// Free an entire linked list of CMD nodes
void brigadier_cmd_ctx_release(brigadier_cmd_ctx *ctx);

// Get the string data of a CMD node
char *brigadier_cmd_get_str(brigadier_cmd_ctx *ctx, int idx);

// Get the int data of a CMD node
int brigadier_cmd_get_val(brigadier_cmd_ctx *ctx, int idx);

// Get the float data of a CMD node
float brigadier_cmd_get_fval(brigadier_cmd_ctx *ctx, int idx);

#endif