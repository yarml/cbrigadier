#ifndef BRIGADIER_NODE_H
#define BRIGADIER_NODE_H

enum BRIGADIER_NODE_TYPE;
typedef enum BRIGADIER_NODE_TYPE brigadier_node_type;

struct BRIGADIER_NODE;
typedef struct BRIGADIER_NODE brigadier_node;

#include <brigadier_func.h>

enum BRIGADIER_NODE_TYPE
{
  BRIGADIER_LITERAL,
  BRIGADIER_STR,
  BRIGADIER_INT,
  BRIGADIER_FLOAT,
  BRIGADIER_ENUM, // Only supports continuous enums
                  // cba to implement a full enum handler,
                  // why am i doing all this anyway, i need sleep
  BRIGADIER_ROOT  // Only root node has this type
};


// Brigadier's nodes are tree nodes
struct BRIGADIER_NODE
{
  brigadier_node_type type;
  union // Elements are defined as decided by type
  {
    char *literal;                                    // BRIGADIER_LITERAL
    brigadier_validate_string_func f_validate_string; // BRIGADIER_STR
    struct                                            // BRIGADIER_INT
    {
      int min;
      int max;
    };
    struct // BRIGADIER_FLOAT
    {
      float fmin;
      float fmax;
    };
    struct // BRIGADIER_ENUM
    {
      brigadier_enum_display_name_func f_display_name;
      int enum_len;
    };
  };
  brigadier_action action;

  char *name;
  char *prompt;
  char *child_prompt;

  brigadier_node *next;
  brigadier_node *children;
};

// Make a new node, uses heap memory
brigadier_node *brigadier_node_new(
  char *name,
  char *prompt,
  char *children_prompt
);
// Release the entire tree that starts with node
void brigadier_node_clean(brigadier_node *node);

// Configure node type

void brigadier_node_literal(brigadier_node *node, char *literal);
void brigadier_node_int(brigadier_node *node);
void brigadier_node_int_extra(brigadier_node *node, int min, int max);
void brigadier_node_float(brigadier_node *node);
void brigadier_node_float_extra(brigadier_node *node, float fmin, float fmax);
void brigadier_node_str(brigadier_node *node);
void brigadier_node_str_extra(
  brigadier_node *node,
  brigadier_validate_string_func f_validate_str
);
void brigadier_node_enum(
  brigadier_node *node,
  brigadier_enum_display_name_func f_display_name,
  int enum_len
);

// Sets the function to execute if command wnds with this node
void brigadier_node_executes(brigadier_node *node, brigadier_action action);

// Adds children to the node
void brigadier_node_add_child(brigadier_node *parent, brigadier_node *node);

// Returns a CMD node if the token matches, uses heap memory
brigadier_cmd_node *brigadier_node_matches(brigadier_node *node, char *token);

// Prints a node
void brigadier_print_node(brigadier_node *node);

#endif