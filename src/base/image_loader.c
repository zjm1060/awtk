﻿/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  image_loader interface
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
 * 2018-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/image_loader.h"

ret_t image_loader_load(image_loader_t* loader, const asset_info_t* asset, bitmap_t* bitmap) {
  return_value_if_fail(loader != NULL && loader->load != NULL && asset != NULL && bitmap != NULL,
                       RET_BAD_PARAMS);

  return loader->load(loader, asset, bitmap);
}
