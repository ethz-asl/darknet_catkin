#ifndef PARSER_H
#define PARSER_H
#include "network.h"

namespace darknet {
network parse_network_cfg(const char *filename);
network parse_network_cfg_custom(const char *filename, int batch, int time_steps);
void save_network(network net, char *filename);
void save_weights(network net, char *filename);
void save_weights_upto(network net, char *filename, int cutoff);
void save_weights_double(network net, char *filename);
void load_weights(network *net, const char *filename);
void load_weights_upto(network *net, const char *filename, int cutoff);
} // namespace darknet

#endif
