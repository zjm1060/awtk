﻿/**
 * File:   color_component.
 * Author: AWTK Develop Team
 * Brief:  color_component
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/image_manager.h"
#include "base/pixel_pack_unpack.h"
#include "color_picker/rgb_and_hsv.h"
#include "color_picker/color_component.h"

static ret_t color_component_update_h(widget_t* widget);
static ret_t color_component_update_sv(widget_t* widget);
static ret_t color_component_set_type(widget_t* widget, const char* type);

static ret_t color_component_update_pressed(widget_t* widget, pointer_event_t* e) {
  point_t p = {e->x, e->y};
  event_t evt = event_init(EVT_VALUE_CHANGING, widget);
  color_component_t* color_component = COLOR_COMPONENT(widget);

  widget_to_local(widget, &p);
  color_component->color_x = p.x;
  color_component->color_y = p.y;
  widget_invalidate_force(widget, NULL);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t color_component_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      color_component_update_pressed(widget, evt);
      widget_grab(widget->parent, widget);
    } break;
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (evt->pressed) {
        color_component_update_pressed(widget, evt);
      }
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      widget_ungrab(widget->parent, widget);
      if (evt->pressed) {
        event_t changed = event_init(EVT_VALUE_CHANGED, widget);
        widget_dispatch(widget, &changed);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t color_component_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t src;
  rect_t dst;
  wh_t w = widget->w;
  wh_t h = widget->h;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  xy_t x = tk_max(0, tk_min(color_component->color_x, (w - 1)));
  xy_t y = tk_max(0, tk_min(color_component->color_y, (h - 1)));
  bitmap_t* image = &(color_component->image);

  if (color_component->update == NULL) {
    color_component_set_type(widget, widget->name);
  }

  if (color_component->need_update) {
    color_component->update(widget);
    image->flags |= BITMAP_FLAG_CHANGED;
    color_component->need_update = FALSE;
  }

  dst = rect_init(0, 0, w, h);
  src = rect_init(0, 0, image->w, image->h);
  canvas_draw_image(c, image, &src, &dst);

  if (color_component->update == color_component_update_sv) {
    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_hline(c, 0, y, w);
    canvas_set_stroke_color(c, color_init(0xd0, 0xd0, 0xd0, 0xff));
    if (y > 0) {
      canvas_draw_hline(c, 0, y - 1, w);
    }
    if ((y + 1) < h) {
      canvas_draw_hline(c, 0, y + 1, w);
    }

    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_vline(c, x, 0, h);
    canvas_set_stroke_color(c, color_init(0xd0, 0xd0, 0xd0, 0xff));
    if (x > 0) {
      canvas_draw_vline(c, x - 1, 0, h);
    }
    if ((x + 1) < w) {
      canvas_draw_vline(c, x + 1, 0, h);
    }
  } else {
    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_hline(c, 0, y, w);
  }

  return RET_OK;
}

static ret_t color_component_on_destroy(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);

  bitmap_destroy(&(color_component->image));

  return RET_OK;
}

static const widget_vtable_t s_color_component_vtable = {
    .size = sizeof(color_component_t),
    .type = WIDGET_TYPE_COLOR_COMPONENT,
    .create = color_component_create,
    .on_destroy = color_component_on_destroy,
    .on_event = color_component_on_event,
    .on_paint_self = color_component_on_paint_self};

static ret_t bitmap_destroy_data(bitmap_t* bitmap) {
  void* data = (void*)bitmap->data;

  TKMEM_FREE(data);

  return RET_OK;
}

static ret_t color_component_init_image(bitmap_t* image, const char* name, int32_t w, int32_t h) {
  int32_t size = w * h * 4;

  memset(image, 0x00, sizeof(bitmap_t));
  image->w = w;
  image->h = h;
  image->flags = 0;
  image->name = name;
#ifdef WITH_BITMAP_BGRA
  image->format = BITMAP_FMT_BGRA8888;
#else
  image->format = BITMAP_FMT_RGBA8888;
#endif /*WITH_BITMAP_BGRA*/
  image->data = (uint8_t*)TKMEM_ALLOC(size);
  return_value_if_fail(image->data != NULL, RET_OOM);
  image->destroy = bitmap_destroy_data;
  bitmap_set_line_length(image, 0);

  memset((void*)(image->data), 0xff, size);

  return RET_OK;
}

static ret_t color_component_update_sv(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  rgba_t rgba = color_component->c.rgba;
  bitmap_t* image = &(color_component->image);

  float H = 0;
  float S = 0;
  float V = 0;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = image->w;
  int32_t h = image->h;
  uint32_t* dst = (uint32_t*)(image->data);

  convertRGBtoHSV(rgba.r, rgba.g, rgba.b, &H, &S, &V);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      V = (float)x / (float)w;
      S = 1 - (float)y / (float)h;
      convertHSVtoRGB(H, S, V, &r, &g, &b);
      *dst++ = rgb_to_image8888(r, g, b);
    }
  }
  color_component->last_hue = H;

  return RET_OK;
}

static ret_t color_component_update_h(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  bitmap_t* image = &(color_component->image);

  float H = 0;
  float S = 1;
  float V = 1;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  int32_t x = 0;
  int32_t y = 0;
  uint32_t v = 0;
  int32_t w = image->w;
  int32_t h = image->h;
  uint32_t* dst = (uint32_t*)(image->data);

  for (y = 0; y < h; y++) {
    H = (1 - (float)y / (float)h) * 360;
    convertHSVtoRGB(H, S, V, &r, &g, &b);
    v = rgb_to_image8888(r, g, b);
    for (x = 0; x < w; x++) {
      *dst++ = v;
    }
  }

  return RET_OK;
}

widget_t* color_component_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, &s_color_component_vtable, x, y, w, h);
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL, NULL);

  color_component_init_image(&(color_component->image), "", w, h);
  color_component->c = color_init(0xff, 0xff, 0xff, 0xff);
  color_component->need_update = TRUE;
  color_component->last_hue = -1;

  return widget;
}

ret_t color_component_set_color(widget_t* widget, color_t c) {
  float H = 0;
  float S = 0;
  float V = 0;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  color_component->c = c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &H, &S, &V);
  if (color_component->update == color_component_update_sv) {
    if ((int32_t)H != color_component->last_hue) {
      color_component->need_update = TRUE;
      log_debug("hue changed(%d != %d)\n", color_component->last_hue, (int32_t)H);
    }
    color_component->color_x = V * widget->w;
    color_component->color_y = (1 - S) * widget->h;
  } else {
    color_component->color_y = (1 - H / 360.0f) * widget->h;
  }
  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t color_component_set_type(widget_t* widget, const char* type) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && type != NULL, RET_BAD_PARAMS);

  color_component->image.name = type;

  if (tk_str_eq(type, COLOR_PICKER_CHILD_SV)) {
    color_component->update = color_component_update_sv;
  } else if (tk_str_eq(type, COLOR_PICKER_CHILD_H)) {
    color_component->update = color_component_update_h;
  } else {
    log_debug("not supported color type\n");
  }

  return RET_OK;
}

float color_component_get_h(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  float v = 360 - color_component->color_y * 360 / widget->h;

  return v;
}

float color_component_get_s(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  float v = 1 - (float)(color_component->color_y) / (float)(widget->h);

  return v;
}

float color_component_get_v(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  float v = (float)(color_component->color_x) / (float)(widget->w);

  return v;
}
