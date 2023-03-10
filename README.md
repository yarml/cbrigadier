# cbrigadier
A port of [Mojang's Brigadier] to C

This port does not try to be compatible in any way shape or form. The only
reason I am even calling it a port is that Brigadier is what inspired this
project.

Nothing ranging from the implementation to the API is copied from Brigadier.
At the very most, there are some faint similarities.

Also, this is the type of projects that was written in one day, quickly to get
a homework done, use in production is not recommended, bugs exist, I can assure
that, definitely some security vulnerabilities here and there, double free() or
memory leaks may exist as well. I will probably never fix any of them.

Also, a veryvery big difference between the real Brigadier, while brigadier
was designed for Minecraft commands, and can be used in multiple applciations,
this *port* is only supposed to work in command driven terminal applications.

# Features
Parse and dispatch commands. If commands are incomplete, prompt the user for
clarification until they complete their command.

# Usage
Let's say you want to implement these two commands, whatever they mean
```
save <filename>
load <filename> <author>
load <ID:int>
```

The desired behaviour is something in the likes of(user input is preceded with >):
```
> save
Where to save the file?: >test.s
Saving file to test.s
> load
How to load the file?
1. By filename
2. By ID
> 2
What is the ID of the file?: >42
Loading file with ID 42
> load 42
Loading file with ID 42
> save test.s
Saving file to test.s
```

This would be done in the following C code
```c
int main()
{
  brigadier_ctx ctx;
  brigadier_ctx_init(&ctx);

  brigadier_node *save_cmd_node = brigadier_node_new("save", "", "");
  brigadier_node *save_filename_node = brigadier_node_new("filename", "Where to save the file?", "");

  brigadier_node *load_cmd_node = brigadier_node_new("load", "", "How to load the file?");
  brigadier_node *load_filename_node = brigadier_node_new("By filename", "Where to load the file?", "");
  brigadier_node *load_author_node = brigadier_node_new("author", "Who is the author of the file?", "");

  brigadier_node *load_id_node = brigadier_node_new("By ID", "What is the ID of the file?", "");

  // Set node types
  brigadier_node_literal(save_cmd_node, "save");
  brigadier_node_literal(load_cmd_node, "load");

  brigadier_node_str(save_file_node);
  brigadier_node_str(load_filename_node);
  brigadier_node_str(load_author_node);

  brigadier_node_int(load_author_node);

  // Attach nodes
  // save_filename comes after save
  brigadier_node_add_child(save_cmd_node, save_filename_node);

  // load_filename comes after load
  brigadier_node_add_child(load_cmd_node, load_filename_node);

  // ID comes after load
  brigadier_node_add_child(load_cmd_node, load_id_node);

  // Author comes after load_filename
  brigadier_node_add_child(load_filename_node, load_author_node);

  // Register actions to execute in case the commands got executed
  brigadier_node_executes(save_filename_node, save_file);
  brigadier_node_executes(load_author_node, load_file);
  brigadier_node_executes(load_id_node, load_id);

  // Prompt the user for a command
  char *cmd = 0;
  size_t n = 0;

  printf("> ");
  getline(&cmd, &n, stdin);
  cmd[strlen(cmd)-1] = 0;

  // Execute command
  brigadier_execute(&ctx, cmd);

  free(cmd);

  brigadier_ctx_clean(&ctx);
}

char *save_file(brigadier_cmd_ctx *ctx)
{
  char *filename = brigadier_cmd_get_str(ctx, 2);
  /* Save file */
  return "Success";
}
char *load_file(brigadier_cmd_ctx *ctx)
{
  char *filename = brigadier_cmd_get_str(ctx, 2);
  char *author = brigadier_cmd_get_str(ctx, 3);
  /* Load file */
  return "Success";
}
char *load_id(brigadier_cmd_ctx *ctx)
{
  int id = brigadier_cmd_get_val(ctx, 2);
  /* Load file */
  return "Success";
}

```

[Mojang's Brigadier]: https://github.com/Mojang/brigadier
