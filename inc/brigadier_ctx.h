#ifndef BRIGADIER_CTX_H
#define BRIGADIER_CTX_H

struct BRIGADIER_CTX;
typedef struct BRIGADIER_CTX brigadier_ctx;

#include <brigadier_node.h>

struct BRIGADIER_CTX
{
  brigadier_node *root;
};

// Initializes a new context
void brigadier_ctx_init(brigadier_ctx *ctx);

// Cleans the context, with all it's tree nodes
void brigadier_ctx_clean(brigadier_ctx *ctx);

// Adds a child to the root node
void brigadier_ctx_add_command(brigadier_ctx *ctx, brigadier_node *cmd);

// Executes a command line as specified by the context
char *brigadier_execute(brigadier_ctx *ctx, char *cmdline);

#endif