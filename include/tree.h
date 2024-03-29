#ifndef TREE_H
#define TREE_H

#include "options.h"
#include "darknet.h"

//typedef struct{
//    int *leaf;
//    int n;
//    int *parent;
//	int *child;
//    int *group;
//    char **name;
//
//    int groups;
//    int *group_size;
//    int *group_offset;
//} tree;

namespace darknet {
//tree *read_tree(char *filename);
int hierarchy_top_prediction(float *predictions, tree *hier, float thresh, int stride);
void hierarchy_predictions(float *predictions, int n, tree *hier, int only_leaves);
void change_leaves(tree *t, char *leaf_list);
float get_hierarchy_probability(float *x, tree *hier, int c);
} // namespace darknet

#endif
