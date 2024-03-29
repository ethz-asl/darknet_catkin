#ifndef SCALE_CHANNELS_LAYER_H
#define SCALE_CHANNELS_LAYER_H

#include "options.h"
#include "layer.h"
#include "network.h"

namespace darknet {
layer make_scale_channels_layer(int batch, int index, int w, int h, int c, int w2, int h2, int c2);
void forward_scale_channels_layer(const layer l, network_state state);
void backward_scale_channels_layer(const layer l, network_state state);
void resize_scale_channels_layer(layer *l, int w, int h);

#ifdef GPU
void forward_scale_channels_layer_gpu(const layer l, network_state state);
void backward_scale_channels_layer_gpu(const layer l, network_state state);
#endif
} // namespace darknet

#endif  // SCALE_CHANNELS_LAYER_H
