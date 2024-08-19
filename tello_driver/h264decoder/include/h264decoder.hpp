#pragma once

// for ssize_t (signed int type as large as pointer type)
#include <cstdlib>
#include <stdexcept>

struct AVCodecContext;
struct AVFrame;
struct AVCodec;
struct AVCodecParserContext;
struct SwsContext;
struct AVPacket;

class H264Exception : public std::runtime_error {
public:
  H264Exception(const char *s) : std::runtime_error(s) {}
};

class H264InitFailure : public H264Exception {
public:
  H264InitFailure(const char *s) : H264Exception(s) {}
};

class H264DecodeFailure : public H264Exception {
public:
  H264DecodeFailure(const char *s) : H264Exception(s) {}
};

class H264Decoder {
  /* Persistent things here, using RAII for cleanup. */
  AVCodecContext *context;
  AVFrame *frame;
  AVCodec *codec;
  AVCodecParserContext *parser;
  AVPacket *pkt;

public:
  H264Decoder();
  ~H264Decoder();
  ssize_t parse(const unsigned char *in_data, ssize_t in_size);
  bool is_frame_available() const;
  const AVFrame &decode_frame();
};

class ConverterRGB24 {
  SwsContext *context;
  AVFrame *framergb;

public:
  ConverterRGB24();
  ~ConverterRGB24();

  /*  Returns, given a width and height,
      how many bytes the frame buffer is going to need. */
  int predict_size(int w, int h);
  /*  Given a decoded frame, convert it to RGB format and fill
out_rgb with the result. Returns a AVFrame structure holding
additional information about the RGB frame, such as the number of
bytes in a row and so on. */
  const AVFrame &convert(const AVFrame &frame, unsigned char *out_rgb);
};

void disable_logging();

/* Wrappers, so we don't have to include libav headers. */
std::pair<int, int> width_height(const AVFrame &);
int row_size(const AVFrame &);
