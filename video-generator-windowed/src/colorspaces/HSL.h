#ifndef HSL_H
#define HSL_H

struct rgb {
  float r, g, b;
};

struct rgb hsl2rgb(float h, float s, float l);

#endif
