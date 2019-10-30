#ifndef HTTP_STREAM_H
#define HTTP_STREAM_H

#include "options.h"
#include "darknet.h"

#include "image.h"
#include <stdint.h>

namespace darknet {
void send_json(detection *dets, int nboxes, int classes, char **names, long long int frame_id, int port, int timeout);

#ifdef OPENCV
void send_mjpeg(mat_cv* mat, int port, int timeout, int quality);
#endif  // OPENCV
} // namespace darknet

#endif // HTTP_STREAM_H
