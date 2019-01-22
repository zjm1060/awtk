﻿/**
 * File:   tab_button_group.h
 * Author: AWTK Develop Team
 * Brief:  tab_button_group
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/tab_button_group.h"

static ret_t tab_button_group_on_layout_children_non_compact(widget_t* widget) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = widget->h;
  int32_t nr = widget->children->size;
  int32_t item_w = widget->w / nr;
  int32_t first_w = widget->w - (nr - 1) * item_w;

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  w = i == 0 ? first_w : item_w;
  widget_move_resize(iter, x, y, w, h);
  x += w;
  widget_layout_children(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t tab_button_group_on_layout_children_compact(widget_t* widget) {
  value_t v;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = widget->h;

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  iter->h = h;
  if (widget_get_prop(iter, WIDGET_PROP_MIN_W, &v) == RET_OK) {
    w = value_int(&v);
  } else {
    w = iter->w;
  }

  widget_move_resize(iter, x, y, w, h);
  widget_layout_children(iter);
  x += w;
  WIDGET_FOR_EACH_CHILD_END();
  return RET_OK;
}

static ret_t tab_button_group_on_layout_children(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  if (widget->children && widget->children->size > 0) {
    if (tab_button_group->compact) {
      return tab_button_group_on_layout_children_compact(widget);
    } else {
      return tab_button_group_on_layout_children_non_compact(widget);
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static widget_t* tab_button_group_get_pages(widget_t* widget) {
  return_value_if_fail(widget && widget->parent, NULL);

  return widget_lookup_by_type(widget->parent, WIDGET_TYPE_PAGES, TRUE);
}

static ret_t tab_button_group_on_paint_active_button(widget_t* widget, widget_t* button,
                                                     canvas_t* c) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t bd = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  widget_t* pages = tab_button_group_get_pages(widget);

  if (pages != NULL) {
    int32_t x = button->x;
    int32_t w = button->w;
    int32_t y = button->y;

    if ((pages->y + pages->h) <= widget->y) {
      if (fg.rgba.a) {
        canvas_set_stroke_color(c, fg);
        canvas_draw_hline(c, x, y, w);
        canvas_draw_hline(c, x + 1, y + 1, w - 2);
      }

      canvas_set_stroke_color(c, bd);
      if (x > 0) {
        canvas_draw_hline(c, 0, y, x);
      }
      if (widget->w > (x + w)) {
        canvas_draw_hline(c, x + w, y, widget->w - x - w);
      }
    } else {
      y += button->h;
      if (fg.rgba.a) {
        canvas_set_stroke_color(c, fg);
        canvas_draw_hline(c, x, y, w);
        canvas_draw_hline(c, x + 1, y - 1, w - 2);
      }

      canvas_set_stroke_color(c, bd);
      if (x > 0) {
        canvas_draw_hline(c, 0, y, x);
      }
      if (widget->w > (x + w)) {
        canvas_draw_hline(c, x + w, y, widget->w - x - w);
      }
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_on_paint_border(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

static ret_t tab_button_group_on_paint_end(widget_t* widget, canvas_t* c) {
  value_t v;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_get_prop(iter, WIDGET_PROP_VALUE, &v) == RET_OK && value_bool(&v)) {
    return tab_button_group_on_paint_active_button(widget, iter, c);
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t tab_button_group_get_prop(widget_t* widget, const char* name, value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    value_set_bool(v, tab_button_group->compact);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tab_button_group_set_prop(widget_t* widget, const char* name, const value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    tab_button_group->compact = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_tab_button_group_vtable = {
    .size = sizeof(tab_button_group_t),
    .type = WIDGET_TYPE_TAB_BUTTON_GROUP,
    .create = tab_button_group_create,
    .set_prop = tab_button_group_set_prop,
    .get_prop = tab_button_group_get_prop,
    .on_layout_children = tab_button_group_on_layout_children,
    .on_paint_border = tab_button_group_on_paint_border,
    .on_paint_end = tab_button_group_on_paint_end,
    .on_paint_self = tab_button_group_on_paint_self};

widget_t* tab_button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, &s_tab_button_group_vtable, x, y, w, h);
}

widget_t* tab_button_group_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_tab_button_group_vtable, NULL);

  return widget;
}
