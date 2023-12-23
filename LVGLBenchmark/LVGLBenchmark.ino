#include "Arduino.h"
#include "GFX_Pins.h"
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <lv_demo.h>

// Unsure why this databus works as we have no SPI but alas
Arduino_DataBus *bus = new Arduino_SWSPI(
  GFX_NOT_DEFINED /* DC */, 21 /* CS */,
  47 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

// Pins are set based on datasheet found here https://files.seeedstudio.com/products/ZX4D30NE01S-UR-4827.pdf
// Also just tested and figured out
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  LCD_DE, LCD_VSYNC, LCD_HSYNC, LCD_PCLK,
  RGB_pins[0], RGB_pins[1], RGB_pins[2], RGB_pins[3], RGB_pins[4],                // R0, R1, R2, R3
  RGB_pins[5], RGB_pins[6], RGB_pins[7], RGB_pins[8], RGB_pins[9], RGB_pins[10],  // G0, G1, G2, G3, G4, G5
  RGB_pins[11], RGB_pins[12], RGB_pins[13], RGB_pins[14], RGB_pins[15],           // B0, B1, B2, B3, B4
  1 /* hsync_polarity */, 10 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 10 /* hsync_back_porch */,
  1 /* vsync_polarity */, 14 /* vsync_front_porch */, 2 /* vsync_pulse_width */, 12 /* vsync_back_porch */);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  Screen_Width /* width */, Screen_Height /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
  bus, GFX_NOT_DEFINED /* RST */, st7701_type7_init_operations, sizeof(st7701_type7_init_operations));

// Logging callback function
void my_log_cb( const char * dsc) {
  Serial.print(dsc);
  Serial.print("\n");
}

// Buffer is a tenth of the screen size
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ Screen_Width * Screen_Height / 10 ];

// This function draws the buffer to the screen repeatedly
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p ) {

    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    gfx->draw16bitRGBBitmap(area->x1, area->y1, ( uint16_t * )&color_p->full, w, h);

    lv_disp_flush_ready( disp_drv );
}

void setup() {

  Serial.begin( 115200 ); 

  while(!Serial){}

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println( LVGL_Arduino );
  Serial.println( "I am LVGL_Arduino" );

  lv_init();

  // Runs logging
  lv_log_register_print_cb(my_log_cb);

  // Begin GFX and backlight
  gfx->begin();
  gfx->fillScreen(BLACK);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  // Initialize buffer
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, Screen_Width * Screen_Height / 10 );

  // Initialize the display with LVGL
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = Screen_Width;
  disp_drv.ver_res = Screen_Height;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  // Run the benchmark
  lv_demo_benchmark();

  Serial.println( "Setup done" );
}

void loop() {
    lv_timer_handler(); // let the GUI do its work 
    delay( 5 );
}

