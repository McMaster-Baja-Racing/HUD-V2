
#include "Arduino.h"
#include <lvgl.h>
#include "Pinout.h"
#include "TPMS.h"
#include <Arduino_GFX_Library.h>







Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 21 /* CS */,
    47 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

//Use pinouts from Pinout.h
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    LCD_DE /* DE */, LCD_VSYNC /* VSYNC */, LCD_HSYNC /* HSYNC */, LCD_PCLK /* PCLK */,
    R_0 /* R0 */, R_1 /* R1 */, R_2 /* R2 */, R_3 /* R3 */, R_4 /* R4 */,
    G_0 /* G0 */, G_1 /* G1 */, G_2 /* G2 */, G_3 /* G3 */, G_4 /* G4 */, G_5 /* G5 */,
    B_0 /* B0 */, B_1 /* B1 */, B_2 /* B2 */, B_3 /* B3 */, B_4 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 10 /* hsync_back_porch */,
    1 /* vsync_polarity */, 14 /* vsync_front_porch */, 2 /* vsync_pulse_width */, 12 /* vsync_back_porch */
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    TFT_Width /* width */, TFT_Height /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type7_init_operations, sizeof(st7701_type7_init_operations)
);

static lv_disp_draw_buf_t  draw_buf;
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

String LVGL_Arduino = "Hello Arduino! ";
LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();


BLEDevice::init("");
pBLEScan = BLEDevice::getScan(); //create new scan
pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
pBLEScan->setInterval(100);
pBLEScan->setWindow(99);  // less or equal setInterval value

gfx->begin();
gfx->fillScreen(BLACK);
pinMode(LCD_BL, OUTPUT);
digitalWrite(LCD_BL, HIGH);

Serial.println("lvgl init");
lv_init();

disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * TFT_Width * 40, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
if (!disp_draw_buf){
    Serial.println("LVGL disp_draw_buf allocate failed!");
}
else{
    Serial.println("LVGL disp_draw_buf allocate OK!");
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, TFT_Width * 40);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = TFT_Width;
    disp_drv.ver_res = TFT_Height;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    //Create a basic object to hold the tpms labels
    lv_obj_t *tpmsLabels = lv_obj_create(lv_scr_act());
    lv_obj_set_size(tpmsLabels, 128, 64);
    lv_obj_align(tpmsLabels, LV_ALIGN_RIGHT_MID, 0, 0);

    //Use the tpms label array to create labels for each tire
    for (int i = 0; i < 4; i++)
    {
        tpmsLabel[i] = lv_label_create(tpmsLabels);
        lv_label_set_text(tpmsLabel[i],  "N/A");
        //align the labels to each cornor of the tpmslabels object
        switch (i)
        {
        case 0:
            lv_obj_align(tpmsLabel[i], LV_ALIGN_OUT_TOP_LEFT, 0, 0);
            break;
        case 1:
            lv_obj_align(tpmsLabel[i], LV_ALIGN_OUT_TOP_RIGHT, 0, 0);
            break;
        case 2:
            lv_obj_align(tpmsLabel[i], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
            break;
        case 3:
            lv_obj_align(tpmsLabel[i], LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
            break;
        default:
            break;
        }
    }

    Serial.println("Setup done");
}




}

void loop()
{
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
    lv_timer_handler();
    delay(5);
}