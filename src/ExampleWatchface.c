#include <pebble.h>
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;
static TextLayer *s_weather_layer;
static GFont s_time_font;
static GFont s_date_font;
static GFont s_day_font;
static GFont s_weather_font;

// Store incoming information
static char temperature_buffer[8];
static char conditions_buffer[32];
static char weather_layer_buffer[32];


static void update_date() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[18];
  strftime(s_buffer, sizeof(s_buffer), "%b %d, %Y", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_date_layer, s_buffer);
	
//	Uncomment line below to test font width
//  text_layer_set_text(s_time_layer, "Sat, Jan 29, 2016");
}

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
	
//	Uncomment line below to test font width
//  text_layer_set_text(s_time_layer, "23:50");
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
	update_date();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	//******************************************************************************
	//* Time Text Layer
	//******************************************************************************
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(22, 20), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorYellow);
//  text_layer_set_text(s_time_layer, "00:00");
//  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	// Create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELSCREEN_TIME_43));

	// Apply to TextLayer
	text_layer_set_font(s_time_layer, s_time_font);
	
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
	//******************************************************************************
	//* End Time Text Layer
	//******************************************************************************
	
	//******************************************************************************
	//* Date Text Layer
	//******************************************************************************
	// Create Date Layer
	s_date_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(25, 90), bounds.size.w, 20));

	// Style the text
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
//	text_layer_set_text(s_date_layer, "Loading...");
	// Create second custom font, apply it and add to Window
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELSCREEN_17));
	text_layer_set_font(s_date_layer, s_date_font);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	//******************************************************************************
	//* End Date Text Layer
	//******************************************************************************
	//******************************************************************************
	//* Weather Text Layer
	//******************************************************************************
	// Create temperature Layer
	s_weather_layer = text_layer_create(
    	GRect(0, PBL_IF_ROUND_ELSE(125, 120), bounds.size.w, 25));

	// Style the text
	text_layer_set_background_color(s_weather_layer, GColorClear);
	text_layer_set_text_color(s_weather_layer, GColorWhite);
	text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
	text_layer_set_text(s_weather_layer, "Loading...");
	// Create second custom font, apply it and add to Window
	s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELSCREEN_17));
	text_layer_set_font(s_weather_layer, s_weather_font);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
	//******************************************************************************
	//* End Weather Text Layer
	//******************************************************************************
	//******************************************************************************
	//* Day Sunday Text Layer
	//******************************************************************************
	// Create Sunday Day Layer
	s_day_layer = text_layer_create(
    GRect(0, 70, bounds.size.w, 20));

	// Style the text
	text_layer_set_background_color(s_day_layer, GColorClear);
	text_layer_set_text_color(s_day_layer, GColorWhite);
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
	text_layer_set_text(s_day_layer, "Su Mo Tu We Th Fr Sa");
	// Create second custom font, apply it and add to Window
	s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SMOOTH_PET_14));
	text_layer_set_font(s_day_layer, s_day_font);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
	//******************************************************************************
	//* End Day Sunday Text Layer
	//******************************************************************************

}

static void main_window_unload(Window *window) {
	// Unload GFont
	fonts_unload_custom_font(s_time_font);
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);
	// Destroy Date elements
	text_layer_destroy(s_date_layer);
	fonts_unload_custom_font(s_date_font);
	// Destroy Day SU elements
	text_layer_destroy(s_day_layer);
	fonts_unload_custom_font(s_day_font);
	// Destroy weather elements
	text_layer_destroy(s_weather_layer);
	fonts_unload_custom_font(s_weather_font);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

	// Set the background color
	window_set_background_color(s_main_window, GColorBlue);
	
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
	update_time();
	update_date();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	// Register callbacks
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	
	// Open AppMessage
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());


}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
