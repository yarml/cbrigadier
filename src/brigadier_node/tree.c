#include <brigadier.h>

void brigadier_node_executes(brigadier_node *node, brigadier_action action)
{
  node->action = action;
}

void brigadier_node_add_child(brigadier_node *parent, brigadier_node *node)
{
  // Add node to the linked list parent->children
  // We can just add at the beginning
  // no need to traverse
  node->next = parent->children;
  parent->children = node;
}
