#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#include "options.h"
#include "image.h"
#include "layer.h"
#include "network.h"

namespace darknet {
layer make_batchnorm_layer(int batch, int w, int h, int c);
void forward_batchnorm_layer(layer l, network_state state);
void backward_batchnorm_layer(layer l, network_state state);

#ifdef GPU
void forward_batchnorm_layer_gpu(layer l, network_state state);
void backward_batchnorm_layer_gpu(layer l, network_state state);
void pull_batchnorm_layer(layer l);
void push_batchnorm_layer(layer l);
#endif
} // namespace darknet

#endif
