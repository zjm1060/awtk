﻿/**
 * File:   main_loop_sdl_gpu.c
 * Author: AWTK Develop Team
 * Brief:  nanovg implemented main_loop interface
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-04-11 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include <SDL.h>
#include "lcd/lcd_nanovg.h"
#include "main_loop/main_loop_sdl_gpu.h"
#include "main_loop/main_loop_simple.h"

#define LOOP_SDL_WINDOW(loop) ((SDL_Window*)(((main_loop_simple_t*)loop)->user1))
#define LOOP_SDL_WINDOW_SET(loop, v) (((main_loop_simple_t*)loop)->user1 = v)

#include "sdl2_common.inc"

static ret_t main_loop_sdl_gpu_destroy(main_loop_t* l) {
  const char* errMsg = SDL_GetError();
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  if (errMsg && strlen(errMsg)) {
    SDL_Log("Error : %s\n", errMsg);
  }

  if (LOOP_SDL_WINDOW(loop)) {
    SDL_DestroyWindow(LOOP_SDL_WINDOW(loop));
  }

  SDL_Quit();
  main_loop_simple_reset(loop);

  return RET_OK;
}

static ret_t main_loop_sdl_gpu_create_window(main_loop_simple_t* l) {
  int32_t x = SDL_WINDOWPOS_UNDEFINED;
  int32_t y = SDL_WINDOWPOS_UNDEFINED;
  const char* title = system_info()->app_name;
  uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

  if (title == NULL) {
    title = "AWTK Simulator";
  }

  if (system_info()->app_type == APP_DESKTOP) {
    flags |= SDL_WINDOW_RESIZABLE;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    log_debug("Failed to initialize SDL: %s", SDL_GetError());
    exit(0);
    return RET_FAIL;
  }

  LOOP_SDL_WINDOW_SET(l, SDL_CreateWindow(title, x, y, l->w, l->h, flags));
  return_value_if_fail(LOOP_SDL_WINDOW(l) != NULL, RET_FAIL);

  canvas_init(&(l->base.canvas), lcd_nanovg_init(LOOP_SDL_WINDOW(l)), font_manager());

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = main_loop_simple_init(w, h);

  loop->base.destroy = main_loop_sdl_gpu_destroy;
  loop->dispatch_input = main_loop_sdl2_dispatch;

  main_loop_sdl_gpu_create_window(loop);
  SDL_StopTextInput();

  return (main_loop_t*)loop;
}
