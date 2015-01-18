#include <pebble.h>

#define TAP_NOT_DATA false

static Window *s_main_window;
static TextLayer *s_output_layer;
static TextLayer *static_text;

int delta;
int strokescore = 0;

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  delta = data[0].y - data[1].y;
  
  if (delta < 0) {
    delta *= -1; 
  }
  
  // Compose string of all data
  if (delta > 400) {
    strokescore++;
    snprintf(s_buffer, sizeof(s_buffer), "%d", strokescore);
    } 
  //Show the data
  text_layer_set_text(s_output_layer, s_buffer);

}

static void main_window_load(Window *window) {
  
  // Create output TextLayer
  
  static_text = text_layer_create(GRect(0,20, 144, 50));
  text_layer_set_text(static_text, "Stroke Score");
  text_layer_set_background_color(static_text, GColorClear);
  text_layer_set_text_color(static_text, GColorWhite);  
  text_layer_set_font(static_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(static_text, GTextAlignmentCenter);

  s_output_layer = text_layer_create(GRect(0, 60, 144, 168));
  text_layer_set_background_color(s_output_layer, GColorClear);
  text_layer_set_text_color(s_output_layer, GColorWhite);  
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_output_layer));
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(static_text));

}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
  text_layer_destroy(static_text);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);

  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  
  accel_data_service_unsubscribe();
  
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}