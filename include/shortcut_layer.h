#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "options.h"
#include "layer.h"
#include "network.h"

namespace darknet {
layer make_shortcut_layer(int batch, int index, int w, int h, int c, int w2, int h2, int c2, int assisted_excitation);
void forward_shortcut_layer(const layer l, network_state state);
void backward_shortcut_layer(const layer l, network_state state);
void resize_shortcut_layer(layer *l, int w, int h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network_state state);
void backward_shortcut_layer_gpu(const layer l, network_state state);
#endif
} // namespace darknet

#endif
