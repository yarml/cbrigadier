#include <brigadier.h>
#include <string.h>

void brigadier_ctx_init(brigadier_ctx *ctx)
{
  memset(ctx, 0, sizeof(*ctx));
  ctx->root = brigadier_node_new(
    "root",
    "",
    "What command do you want to execute?"
  );
  ctx->root->type = BRIGADIER_ROOT;
}

void brigadier_ctx_clean(brigadier_ctx *ctx)
{
  brigadier_node_clean(ctx->root);
  memset(ctx, 0, sizeof(*ctx));
}

// Attach a brigadier command to the root
void brigadier_ctx_add_command(brigadier_ctx *ctx, brigadier_node *cmd)
{
  cmd->next = ctx->root->children;
  ctx->root->children = cmd;
}
