# 1 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino"

# 3 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino" 2
# 4 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino" 2
# 5 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino" 2
# 6 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino" 2


Arduino_DataBus *bus = new Arduino_SWSPI(
    -1 /* DC */, 21 /* CS */,
    47 /* SCK */, 41 /* MOSI */, -1 /* MISO */);

//Use pinouts from Pinout.h
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    39 /*Data Enable*/ /* DE */, 48 /*Vertical Sync*/ /* VSYNC */, 40 /*Horizontal Sync*/ /* HSYNC */, 45 /*Pixel Clock*/ /* PCLK */,
    10 /* R0 */, 16 /* R1 */, 9 /* R2 */, 15 /* R3 */, 46 /* R4 */,
    8 /* G0 */, 13 /* G1 */, 18 /* G2 */, 12 /* G3 */, 11 /* G4 */, 17 /* G5 */,
    47 /* B0 */, 41 /* B1 */, 0 /* B2 */, 42 /* B3 */, 14 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 10 /* hsync_back_porch */,
    1 /* vsync_polarity */, 14 /* vsync_front_porch */, 2 /* vsync_pulse_width */, 12 /* vsync_back_porch */
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 272 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, -1 /* RST */, st7701_type7_init_operations, sizeof(st7701_type7_init_operations)
);

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t *disp_draw_buf;

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    int16_t w = ( area->x2 - area->x1 + 1 );
    int16_t h = ( area->y2 - area->y1 + 1 );

    gfx->draw16bitRGBBitmap( area->x1, area->y1, ( uint16_t * )color_p->full ,w, h );

    lv_disp_flush_ready( disp );
}


void setup()
{
delay(2000);
Serial.begin(115200);
Serial.println("Rotary Short Demo");
delay(1000);
Serial.println("gfx beguin");
gfx->begin();
delay(1000);
Serial.println("BACK LED ON?");
pinMode(7 /*Backlight*/, 0x03);
digitalWrite(7 /*Backlight*/, 0x1);

Serial.println("lvgl init");
lv_init();

disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * 480 * 40, (1<<11) /*|< Memory must be internal; specifically it should not disappear when flash/spiram cache is switched off*/ | (1<<2) /*|< Memory must allow for 8/16/...-bit data accesses*/);
if (!disp_draw_buf)
{
Serial.println("LVGL disp_draw_buf allocate failed!");
}
else{
    Serial.println("LVGL disp_draw_buf allocate OK!");
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, 
# 66 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino" 3 4
                                                   __null
# 66 "C:\\Users\\Ariel\\Documents\\Dev\\HudV2\\HudV2.ino"
                                                       , 480 * 40);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 272;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    Serial.println("Setup done");
}




}

void loop()
{
  lv_timer_handler();
  delay(5);
}
