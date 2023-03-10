#ifndef BRIGADIER_FUNC_H
#define BRIGADIER_FUNC_H

#include <brigadier_ctx.h>

// Function pointer type, to validate string
typedef int (*brigadier_validate_string_func)(char *str);
// Function pointer type, to get an enum's element display name fromit's index
typedef char *(*brigadier_enum_display_name_func)(int index);
// Function pointer type, action attached to command
// Returns a string for the execution status
typedef char *(*brigadier_action)(brigadier_cmd_ctx *cmd);

#endif