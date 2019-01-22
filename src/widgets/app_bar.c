﻿/**
 * File:   app_bar.h
 * Author: AWTK Develop Team
 * Brief:  app_bar
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/app_bar.h"

static const widget_vtable_t s_app_bar_vtable = {
    .size = sizeof(app_bar_t), .type = WIDGET_TYPE_APP_BAR, .create = app_bar_create};

widget_t* app_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, &s_app_bar_vtable, x, y, w, h);
}

widget_t* app_bar_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_app_bar_vtable, NULL);

  return widget;
}
