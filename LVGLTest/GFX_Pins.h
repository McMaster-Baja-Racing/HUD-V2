// Screen dimensions
#define Screen_Width 480
#define Screen_Height 272

#define LCD_BL 45 //Backlight
#define LCD_DE 40 //Data Enable
#define LCD_VSYNC 48 //Vertical Sync
#define LCD_HSYNC 47 //Horizontal Sync
#define LCD_PCLK 14 //Pixel Clock

// Red pins
#define R0_PIN 4
#define R1_PIN 5
#define R2_PIN 6
#define R3_PIN 7
#define R4_PIN 15

// Green pins
#define G0_PIN 16
#define G1_PIN 17
#define G2_PIN 18
#define G3_PIN 8
#define G4_PIN 3
#define G5_PIN 46

// Blue pins
#define B0_PIN 9
#define B1_PIN 10
#define B2_PIN 11
#define B3_PIN 12
#define B4_PIN 13

const int RGB_pins[] = {
    R0_PIN, R1_PIN, R2_PIN, R3_PIN, R4_PIN,
    G0_PIN, G1_PIN, G2_PIN, G3_PIN, G4_PIN, G5_PIN,
    B0_PIN, B1_PIN, B2_PIN, B3_PIN, B4_PIN
};