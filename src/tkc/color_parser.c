﻿#include "color_parser.h"
#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

typedef struct _color_map_t {
  const char* name;
  const char* value;
} color_map_t;

static color_map_t colors_map[] = {{"aliceblue", "#f0f8ff"},
                                   {"antiquewhite", "#faebd7"},
                                   {"aqua", "#00ffff"},
                                   {"aquamarine", "#7fffd4"},
                                   {"azure", "#f0ffff"},
                                   {"beige", "#f5f5dc"},
                                   {"bisque", "#ffe4c4"},
                                   {"black", "#000000"},
                                   {"blanchedalmond", "#ffebcd"},
                                   {"blue", "#0000ff"},
                                   {"blueviolet", "#8a2be2"},
                                   {"brown", "#a52a2a"},
                                   {"burlywood", "#deb887"},
                                   {"cadetblue", "#5f9ea0"},
                                   {"chartreuse", "#7fff00"},
                                   {"chocolate", "#d2691e"},
                                   {"coral", "#ff7f50"},
                                   {"cornflowerblue", "#6495ed"},
                                   {"cornsilk", "#fff8dc"},
                                   {"crimson", "#dc143c"},
                                   {"cyan", "#00ffff"},
                                   {"darkblue", "#00008b"},
                                   {"darkcyan", "#008b8b"},
                                   {"darkgoldenrod", "#b8860b"},
                                   {"darkgray", "#a9a9a9"},
                                   {"darkgreen", "#006400"},
                                   {"darkkhaki", "#bdb76b"},
                                   {"darkmagenta", "#8b008b"},
                                   {"darkolivegreen", "#556b2f"},
                                   {"darkorange", "#ff8c00"},
                                   {"darkorchid", "#9932cc"},
                                   {"darkred", "#8b0000"},
                                   {"darksalmon", "#e9967a"},
                                   {"darkseagreen", "#8fbc8f"},
                                   {"darkslateblue", "#483d8b"},
                                   {"darkslategray", "#2f4f4f"},
                                   {"darkturquoise", "#00ced1"},
                                   {"darkviolet", "#9400d3"},
                                   {"deeppink", "#ff1493"},
                                   {"deepskyblue", "#00bfff"},
                                   {"dimgray", "#696969"},
                                   {"dodgerblue", "#1e90ff"},
                                   {"firebrick", "#b22222"},
                                   {"floralwhite", "#fffaf0"},
                                   {"forestgreen", "#228b22"},
                                   {"fuchsia", "#ff00ff"},
                                   {"gainsboro", "#dcdcdc"},
                                   {"ghostwhite", "#f8f8ff"},
                                   {"gold", "#ffd700"},
                                   {"goldenrod", "#daa520"},
                                   {"gray", "#bebebe"},
                                   {"green", "#008000"},
                                   {"greenyellow", "#adff2f"},
                                   {"honeydew", "#f0fff0"},
                                   {"hotpink", "#ff69b4"},
                                   {"indianred", "#cd5c5c"},
                                   {"indigo", "#4b0082"},
                                   {"ivory", "#fffff0"},
                                   {"khaki", "#f0d58c"},
                                   {"lavender", "#e6e6fa"},
                                   {"lavenderblush", "#fff0f5"},
                                   {"lawngreen", "#7cfc00"},
                                   {"lemonchiffon", "#fffacd"},
                                   {"lightblue", "#add8e6"},
                                   {"lightcoral", "#f08080"},
                                   {"lightcyan", "#e0ffff"},
                                   {"lightgoldenrodyellow", "#fafad2"},
                                   {"lightgreen", "#90ee90"},
                                   {"lightgrey", "#d3d3d3"},
                                   {"lightpink", "#ffb6c1"},
                                   {"lightsalmon", "#ffa07a"},
                                   {"lightseagreen", "#20b2aa"},
                                   {"lightskyblue", "#87cefa"},
                                   {"lightslategray", "#778899"},
                                   {"lightsteelblue", "#b0c4de"},
                                   {"lightyellow", "#ffffe0"},
                                   {"lime", "#00ff00"},
                                   {"limegreen", "#32cd32"},
                                   {"linen", "#faf0e6"},
                                   {"magenta", "#ff00ff"},
                                   {"maroon", "#800000"},
                                   {"mediumaquamarine", "#66cdaa"},
                                   {"mediumblue", "#0000cd"},
                                   {"mediumorchid", "#ba55d3"},
                                   {"mediumpurple", "#9370db"},
                                   {"mediumseagreen", "#3cb371"},
                                   {"mediumslateblue", "#7b68ee"},
                                   {"mediumspringgreen", "#00fa9a"},
                                   {"mediumturquoise", "#48d1cc"},
                                   {"mediumvioletred", "#c71585"},
                                   {"midnightblue", "#191970"},
                                   {"mintcream", "#f5fffa"},
                                   {"mistyrose", "#ffe4e1"},
                                   {"moccasin", "#ffe4b5"},
                                   {"navajowhite", "#ffdead"},
                                   {"navy", "#000080"},
                                   {"oldlace", "#fdf5e6"},
                                   {"olive", "#808000"},
                                   {"olivedrab", "#6b8e23"},
                                   {"orange", "#ffa500"},
                                   {"orangered", "#ff4500"},
                                   {"orchid", "#da70d6"},
                                   {"palegoldenrod", "#eee8aa"},
                                   {"palegreen", "#98fb98"},
                                   {"paleturquoise", "#afeeee"},
                                   {"palevioletred", "#db7093"},
                                   {"papayawhip", "#ffefd5"},
                                   {"peachpuff", "#ffdab9"},
                                   {"peru", "#cd853f"},
                                   {"pink", "#ffc0cb"},
                                   {"plum", "#dda0dd"},
                                   {"powderblue", "#b0e0e6"},
                                   {"purple", "#800080"},
                                   {"red", "#ff0000"},
                                   {"rosybrown", "#bc8f8f"},
                                   {"royalblue", "#4169e1"},
                                   {"saddlebrown", "#8b4513"},
                                   {"salmon", "#fa8072"},
                                   {"sandybrown", "#f4a460"},
                                   {"seagreen", "#2e8b57"},
                                   {"seashell", "#fff5ee"},
                                   {"sienna", "#a0522d"},
                                   {"silver", "#c0c0c0"},
                                   {"skyblue", "#87ceeb"},
                                   {"slateblue", "#6a5acd"},
                                   {"slategray", "#708090"},
                                   {"snow", "#fffafa"},
                                   {"springgreen", "#00ff7f"},
                                   {"steelblue", "#4682b4"},
                                   {"tan", "#d2b48c"},
                                   {"teal", "#008080"},
                                   {"thistle", "#d8bfd8"},
                                   {"tomato", "#ff6347"},
                                   {"turquoise", "#40e0d0"},
                                   {"violet", "#ee82ee"},
                                   {"wheat", "#f5deb3"},
                                   {"white", "#ffffff"},
                                   {"whitesmoke", "#f5f5f5"},
                                   {"yellow", "#ffff00"},
                                   {"yellowgreen", "#9acd32"}};

static int compare_color(const color_map_t* a, const color_map_t* b) {
  return strcmp(a->name, b->name);
}

static const char* map_name_to_value(const char* name) {
  color_map_t color_map;
  color_map.name = name;
  color_map.value = NULL;

  color_map_t* ret = (color_map_t*)bsearch(&color_map, colors_map, ARRAY_SIZE(colors_map),
                                           sizeof(color_map_t), (tk_compare_t)compare_color);

  return ret ? ret->value : NULL;
}

static bool_t color_parse_rgba(const char* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
  uint32_t len = strlen(color);
  return_value_if_fail(len > 6, FALSE);

  int ir = 0;
  int ig = 0;
  int ib = 0;
  float fa = 1;

  if (strstr(color, "rgba") != NULL) {
    tk_sscanf(color, "rgba(%d,%d,%d,%f)", &ir, &ig, &ib, &fa);
  } else {
    tk_sscanf(color, "rgb(%d,%d,%d)", &ir, &ig, &ib);
  }

  *r = ir & 0xff;
  *g = ig & 0xff;
  *b = ib & 0xff;
  *a = (int)(fa * 255) & 0xff;

  return TRUE;
}

static bool_t color_parse_hex(const char* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
  uint32_t len = strlen(color);
  return_value_if_fail(len > 6, FALSE);

  int ir = 0;
  int ig = 0;
  int ib = 0;
  int ia = 0xff;

  if (len < 8) {
    tk_sscanf(color, "#%02x%02x%02x", &ir, &ig, &ib);
  } else {
    tk_sscanf(color, "#%02x%02x%02x%02x", &ir, &ig, &ib, &ia);
  }

  *r = ir & 0xff;
  *g = ig & 0xff;
  *b = ib & 0xff;
  *a = ia & 0xff;

  return TRUE;
}

static bool_t color_parse_impl(const char* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
  str_t s;
  str_t* str;
  bool_t ret = FALSE;
  return_value_if_fail(color != NULL && r != NULL && g != NULL && b != NULL && a != NULL, FALSE);

  *r = 0;
  *b = 0;
  *g = 0;
  *a = 0;

  str = str_init(&s, 10);
  str_set(str, color);

  if (str_start_with(str, "rgb")) {
    str_to_lower(str);
    str_replace(str, " ", "");

    ret = color_parse_rgba(str->str, r, g, b, a);
  } else {
    const char* value = color[0] == '#' ? color : map_name_to_value(color);

    if (value) {
      str_set(str, value);
      str_to_lower(str);

      ret = color_parse_hex(str->str, r, g, b, a);
    }
  }
  str_reset(str);

  return ret;
}

color_t color_parse(const char* color) {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 0xff;

  color_parse_impl(color, &r, &g, &b, &a);

  return color_init(r, g, b, a);
}

color_t* color_create_with_str(const char* str) {
  color_t* c = TKMEM_ZALLOC(color_t);
  return_value_if_fail(c != NULL, NULL);

  *c = color_parse(str);

  return c;
}
