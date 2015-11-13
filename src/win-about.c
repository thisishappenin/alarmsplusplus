#include <pebble.h>
#include "win-about.h"
#include "localize.h"
#include "pebble_process_info.h"
#include "debug.h"

extern const PebbleProcessInfo __pbl_app_info;

static void window_load(Window* window);
static void window_unload(Window* window);
static void layer_header_update(Layer* layer, GContext* ctx);

static Window*          s_window;
static Layer*           s_layer_header;
static TextLayer*       s_layer_text;
static ScrollLayer*     s_layer_scroll;

void win_about_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
#ifndef PBL_COLOR
  window_set_fullscreen(s_window,true);
#endif
}

void win_about_show(void) {
  window_stack_push(s_window, true);
}

static void window_load(Window* window) {
  GRect window_size = layer_get_bounds(window_get_root_layer(window));
  s_layer_header = layer_create(GRect(1, 1, window_size.size.w, 24));
  layer_set_update_proc(s_layer_header, layer_header_update);
  layer_add_child(window_get_root_layer(window),s_layer_header);

  s_layer_text = text_layer_create(GRect(0, 0, window_size.size.w, 500));
  text_layer_set_font(s_layer_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(s_layer_text, _("Hints"));
  text_layer_set_text_alignment(s_layer_text,GTextAlignmentCenter);
  GSize max_size = text_layer_get_content_size(s_layer_text);
  APP_LOG(APP_LOG_LEVEL_DEBUG,"Size: %d,%d",max_size.w,max_size.h);
  text_layer_set_size(s_layer_text, max_size);
  s_layer_scroll = scroll_layer_create(GRect(0,24,window_size.size.w,window_size.size.h-24));
  scroll_layer_set_content_size(s_layer_scroll, GSize(window_size.size.w, max_size.h + 12));
  scroll_layer_add_child(s_layer_scroll,text_layer_get_layer(s_layer_text));
  scroll_layer_set_click_config_onto_window(s_layer_scroll, window);
  layer_add_child(window_get_root_layer(window), scroll_layer_get_layer(s_layer_scroll));
#ifdef PBL_ROUND
  text_layer_enable_screen_text_flow_and_paging(s_layer_text, 3);
  text_layer_set_size(s_layer_text, GSize(window_size.size.w,500));
  scroll_layer_set_paging(s_layer_scroll,true);
  max_size = text_layer_get_content_size(s_layer_text);
  text_layer_set_size(s_layer_text, max_size);
  scroll_layer_set_content_size(s_layer_scroll, GSize(window_size.size.w, max_size.h + 12));
#endif
}

static void window_unload(Window* window) {
  layer_destroy(s_layer_header);
  text_layer_destroy(s_layer_text);
  scroll_layer_destroy(s_layer_scroll);
}

static void layer_header_update(Layer* layer, GContext* ctx) {
#ifdef PBL_COLOR
  graphics_context_set_fill_color(ctx, GColorBlue);
#endif
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 12, GCornersAll);
  GRect layer_size = layer_get_bounds(layer);
  char version_text[15];
  snprintf(version_text, sizeof(version_text), "v%d.%d",__pbl_app_info.process_version.major,__pbl_app_info.process_version.minor);
  graphics_draw_text(ctx, version_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(0, -2, layer_size.size.w, 24), GTextOverflowModeFill, GTextAlignmentCenter, 0);
}