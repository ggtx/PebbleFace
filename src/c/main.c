#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;
// static BitmapLayer *s_background_layer;
// static GBitmap *s_background_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void update_date() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[20];
  strftime(s_buffer, sizeof(s_buffer), "%a | %d", tick_time);
  text_layer_set_text(s_date_layer, s_buffer);
}

static void tick_date_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_date();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  /*
  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_ONE);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  */
  
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(43, 40), bounds.size.w, 55));
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(103, 97), bounds.size.w, 30));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorDarkGray);
  // text_layer_set_background_color(s_time_layer, GColorMidnightGreen);
  // text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite); 
  // Create time GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUSTOM_ONE_50));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Creative date GFont
  text_layer_set_background_color(s_date_layer, GColorDarkGray);
  text_layer_set_text_color(s_date_layer, GColorWhite); 
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUSTOM_ONE_20));
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(DAY_UNIT, tick_date_handler);
}

static void main_window_unload(Window *window) {
   // Unload GFont
  fonts_unload_custom_font(s_time_font);
}

static void init() {
  // Create the Window
  s_main_window = window_create();
  
  // window_set_background_color(s_main_window, GColorCobaltBlue);
  window_set_background_color(s_main_window, GColorDarkGray); 
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Push to the stack, animated
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  // Destroy the Window
  window_destroy(s_main_window);
}

int main(void) {
  // Initialize the app
  init();

  // Wait for app events
  app_event_loop();

  // Deinitialize the app
  deinit();

  // App finished without error
  return 0;
}

