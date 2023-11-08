#include "font.h"


uint8_t char_width = 8;
uint8_t char_height = 13;
uint8_t characters[95][13] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// space :32
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// ! :33
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
	{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
	{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
	{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
	{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
	{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
	{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
	{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
	{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
	{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
	{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
	{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
	{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
	{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
	{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
	{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
	{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
	{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
	{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
	{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}  // :126
};

// Header File for SSD1306 characters
// Generated with TTF2BMH
// Font Ubuntu Mono
// Font Size: 24
//uint8_t char_height = 24;
uint8_t bitmap_33[] = {0,248,248,0,0,159,159,0,3,7,7,3};
uint8_t bitmap_34[] = {126,126,0,0,126,126,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t bitmap_35[] = {128,128,128,248,248,128,128,128,248,248,128,97,225,255,127,97,97,225,255,127,97,97,0,7,7,0,0,0,7,7,0,0,0};
uint8_t bitmap_36[] = {192,224,112,48,62,62,48,112,96,0,3,7,14,12,12,28,24,56,240,224,3,7,6,6,62,62,6,7,3,1};
uint8_t bitmap_37[] = {240,248,8,8,248,224,0,128,224,112,24,8,3,7,132,228,119,29,254,251,9,8,248,224,4,6,3,1,0,0,3,7,4,4,7,3};
uint8_t bitmap_38[] = {0,224,240,24,24,24,248,240,0,0,0,224,248,31,7,14,27,113,224,252,28,0,1,3,7,6,6,6,3,1,3,7,4};
uint8_t bitmap_39[] = {254,254,0,0,0,0};
uint8_t bitmap_40[] = {0,128,192,224,112,56,16,252,255,3,0,0,0,0,0,7,15,28,56,112,32};
uint8_t bitmap_41[] = {16,56,112,224,192,128,0,0,0,0,0,3,255,252,32,112,56,28,15,7,0};
uint8_t bitmap_42[] = {64,224,192,192,248,184,192,192,224,64,0,8,30,15,3,3,15,30,8,0,0,0,0,0,0,0,0,0,0,0};
uint8_t bitmap_43[] = {0,0,0,0,192,192,0,0,0,0,24,24,24,24,255,255,24,24,24,24,0,0,0,0,3,3,0,0,0,0};
uint8_t bitmap_44[] = {0,0,0,0,0,0,0,128,128,0,96,115,63,31,15};
uint8_t bitmap_45[] = {0,0,0,0,0,0,24,24,24,24,24,24,0,0,0,0,0,0};
uint8_t bitmap_46[] = {0,0,0,0,0,128,128,0,3,7,7,3};
uint8_t bitmap_47[] = {0,0,0,0,0,192,252,62,6,0,0,128,240,126,15,1,0,0,96,124,63,3,0,0,0,0,0};
uint8_t bitmap_48[] = {128,224,112,56,24,24,24,56,112,224,128,127,255,128,0,14,14,14,0,128,255,127,0,1,3,7,6,6,6,7,3,1,0};
uint8_t bitmap_49[] = {192,224,96,48,248,248,0,0,0,0,0,0,0,255,255,0,0,0,0,6,6,6,7,7,6,6,6};
uint8_t bitmap_50[] = {32,112,56,24,24,24,56,240,224,0,0,128,224,112,56,28,14,7,1,0,7,7,6,6,6,6,6,6,6,6};
uint8_t bitmap_51[] = {16,56,24,24,24,56,240,224,0,0,0,0,12,12,14,27,241,224,3,6,6,6,6,6,3,3,1};
uint8_t bitmap_52[] = {0,0,0,128,224,112,56,248,248,0,0,112,124,111,103,97,96,96,255,255,96,96,0,0,0,0,0,0,0,7,7,0,0};
uint8_t bitmap_53[] = {0,240,248,24,24,24,24,24,24,0,7,7,6,6,14,28,252,240,3,6,6,6,6,6,3,3,1};
uint8_t bitmap_54[] = {0,128,192,96,48,48,24,24,24,24,0,254,255,133,6,6,6,6,6,12,252,240,0,1,3,7,6,6,6,6,3,3,0};
uint8_t bitmap_55[] = {24,24,24,24,24,24,152,216,120,56,0,0,0,224,252,31,3,0,0,0,0,0,7,7,0,0,0,0,0,0};
uint8_t bitmap_56[] = {224,240,48,24,24,24,24,24,48,240,224,225,243,31,14,12,12,12,30,27,243,225,1,3,3,6,6,6,6,6,3,3,1};
uint8_t bitmap_57[] = {192,240,48,24,24,24,24,56,112,224,128,3,15,12,24,24,24,24,152,232,127,31,0,6,6,6,6,3,3,1,1,0,0};
uint8_t bitmap_58[] = {0,128,128,0,3,135,135,3,3,7,7,3};
uint8_t bitmap_59[] = {0,0,0,128,128,0,0,0,131,135,7,3,96,115,63,31,15,0};
uint8_t bitmap_60[] = {0,0,0,0,0,0,0,0,0,128,128,56,56,108,108,108,198,198,131,131,131,1,0,0,0,0,0,0,0,1,1,3,3};
uint8_t bitmap_61[] = {0,0,0,0,0,0,0,0,0,0,0,198,198,198,198,198,198,198,198,198,198,198,0,0,0,0,0,0,0,0,0,0,0};
uint8_t bitmap_62[] = {128,128,0,0,0,0,0,0,0,0,0,1,131,131,131,198,198,108,108,108,56,56,3,3,1,1,0,0,0,0,0,0,0};
uint8_t bitmap_63[] = {48,24,24,24,24,56,240,224,0,0,152,156,6,3,1,0,0,3,7,7,3,0,0,0};
uint8_t bitmap_64[] = {0,192,224,48,24,24,24,24,48,240,192,255,255,1,0,248,252,142,6,6,255,255,1,7,30,56,48,97,99,99,99,99,3};
uint8_t bitmap_65[] = {0,0,0,192,248,56,56,248,192,0,0,0,0,224,254,127,99,96,96,99,127,254,224,0,6,7,3,0,0,0,0,0,0,1,7,6};
uint8_t bitmap_66[] = {248,248,24,24,24,24,24,48,240,224,0,255,255,12,12,12,12,12,14,27,241,224,7,7,6,6,6,6,6,7,3,3,1};
uint8_t bitmap_67[] = {0,192,224,48,56,24,24,24,24,56,48,127,255,192,0,0,0,0,0,0,0,0,0,1,3,3,7,6,6,6,6,7,3};
uint8_t bitmap_68[] = {248,248,24,24,24,24,56,48,224,192,0,255,255,0,0,0,0,0,0,192,255,63,7,7,6,6,6,6,7,3,1,0,0};
uint8_t bitmap_69[] = {248,248,24,24,24,24,24,24,24,0,255,255,12,12,12,12,12,12,12,0,7,7,6,6,6,6,6,6,6,6};
uint8_t bitmap_70[] = {248,248,24,24,24,24,24,24,24,255,255,12,12,12,12,12,12,0,7,7,0,0,0,0,0,0,0};
uint8_t bitmap_71[] = {0,192,224,112,56,24,24,24,24,56,16,63,255,192,0,0,0,0,0,0,248,248,0,0,3,3,7,6,6,6,6,7,3};
uint8_t bitmap_72[] = {248,248,0,0,0,0,0,0,0,248,248,255,255,12,12,12,12,12,12,12,255,255,7,7,0,0,0,0,0,0,0,7,7};
uint8_t bitmap_73[] = {24,24,24,248,248,24,24,24,0,0,0,255,255,0,0,0,6,6,6,7,7,6,6,6};
uint8_t bitmap_74[] = {0,0,24,24,24,24,24,24,248,248,0,0,0,0,0,0,0,0,255,255,2,7,6,6,6,6,6,3,3,0};
uint8_t bitmap_75[] = {248,248,0,0,128,224,120,56,8,0,255,255,14,31,59,113,192,128,0,0,7,7,0,0,0,0,1,7,7,4};
uint8_t bitmap_76[] = {248,248,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,7,7,6,6,6,6,6,6,6,6};
uint8_t bitmap_77[] = {192,248,56,248,128,0,128,248,120,248,128,255,255,0,3,31,24,31,7,0,255,255,7,7,0,0,0,0,0,0,0,7,7};
uint8_t bitmap_78[] = {248,248,120,224,128,0,0,0,0,248,248,255,255,0,1,3,15,60,240,192,255,255,7,7,0,0,0,0,0,0,3,7,7};
uint8_t bitmap_79[] = {128,224,112,56,24,24,24,56,112,224,128,127,255,128,0,0,0,0,0,128,255,127,0,1,3,7,6,6,6,7,3,1,0};
uint8_t bitmap_80[] = {248,248,24,24,24,24,24,48,240,192,255,255,24,24,24,24,24,12,15,3,7,7,0,0,0,0,0,0,0,0};
uint8_t bitmap_81[] = {128,224,112,56,24,24,24,56,112,224,128,127,255,128,0,0,0,0,0,128,255,127,0,1,3,7,14,30,54,55,99,97,0};
uint8_t bitmap_82[] = {248,248,24,24,24,24,24,48,240,192,0,255,255,24,24,24,56,120,236,135,3,0,7,7,0,0,0,0,0,1,3,7,4};
uint8_t bitmap_83[] = {224,240,56,24,24,24,24,56,48,0,1,3,7,6,12,12,28,24,248,224,3,3,6,6,6,6,6,7,3,1};
uint8_t bitmap_84[] = {24,24,24,24,248,248,24,24,24,24,0,0,0,0,255,255,0,0,0,0,0,0,0,0,7,7,0,0,0,0};
uint8_t bitmap_85[] = {248,248,0,0,0,0,0,0,0,248,248,255,255,128,0,0,0,0,0,128,255,255,0,3,3,6,6,6,6,6,3,3,0};
uint8_t bitmap_86[] = {24,248,224,0,0,0,0,0,0,224,248,24,0,1,15,127,248,192,0,224,126,15,1,0,0,0,0,0,7,7,7,3,0,0,0,0};
uint8_t bitmap_87[] = {248,248,0,0,0,0,0,0,0,248,248,255,255,128,248,126,6,126,248,128,255,255,7,7,7,7,0,0,0,3,7,7,7};
uint8_t bitmap_88[] = {8,56,240,224,128,0,128,224,240,56,8,0,0,192,241,127,30,63,225,192,0,0,4,7,3,0,0,0,0,0,3,7,4};
uint8_t bitmap_89[] = {8,120,240,192,0,0,0,0,192,240,56,8,0,0,1,3,15,252,252,15,3,0,0,0,0,0,0,0,0,7,7,0,0,0,0,0};
uint8_t bitmap_90[] = {24,24,24,24,24,24,24,216,248,120,24,0,128,224,112,60,14,7,1,0,0,0,6,7,7,6,6,6,6,6,6,6,6};
uint8_t bitmap_91[] = {248,248,24,24,24,24,255,255,0,0,0,0,127,127,96,96,96,96};
uint8_t bitmap_92[] = {0,0,0,0,0,192,252,62,6,0,0,128,240,126,15,1,0,0,96,124,63,3,0,0,0,0,0};
uint8_t bitmap_93[] = {24,24,24,24,248,248,0,0,0,0,255,255,96,96,96,96,127,127};
uint8_t bitmap_94[] = {0,0,128,224,120,24,120,224,128,0,0,4,15,7,1,0,0,0,1,7,15,4,0,0,0,0,0,0,0,0,0,0,0};
uint8_t bitmap_95[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,96,96,96,96,96,96,96,96,96,96,96,96};
uint8_t bitmap_96[] = {4,14,56,16,0,0,0,0,0,0,0,0};
uint8_t bitmap_97[] = {0,128,128,128,128,128,128,0,0,192,225,113,49,49,49,51,255,254,1,3,7,6,6,6,6,7,7};
uint8_t bitmap_98[] = {254,254,0,128,128,128,128,0,0,0,255,255,3,1,1,1,3,135,254,252,7,7,6,6,6,6,7,3,1,0};
uint8_t bitmap_99[] = {0,0,0,0,128,128,128,128,128,128,128,252,254,135,3,1,1,1,1,1,1,1,0,1,3,3,6,6,6,6,6,6,6};
uint8_t bitmap_100[] = {0,0,0,128,128,128,128,128,254,254,252,254,135,3,1,1,1,3,255,255,0,1,3,7,6,6,6,6,7,7};
uint8_t bitmap_101[] = {0,0,0,128,128,128,128,128,0,0,0,252,254,55,51,49,49,49,49,51,63,60,0,1,3,7,6,6,6,6,6,3,0};
uint8_t bitmap_102[] = {128,128,128,248,252,142,134,134,134,12,1,1,1,255,255,1,1,1,1,0,0,0,0,7,7,0,0,0,0,0};
uint8_t bitmap_103[] = {0,0,0,128,128,128,128,128,128,0,252,254,135,3,1,1,1,1,255,255,0,51,99,102,102,102,102,55,63,31};
uint8_t bitmap_104[] = {254,254,128,128,128,128,128,0,0,255,255,1,1,1,1,3,255,254,7,7,0,0,0,0,0,7,7};
uint8_t bitmap_105[] = {128,128,128,156,156,156,0,0,0,0,0,1,1,1,1,255,255,0,0,0,0,0,0,0,0,0,1,3,7,6,6,6,3};
uint8_t bitmap_106[] = {0,128,128,128,156,156,156,128,0,1,1,1,1,1,255,255,32,96,96,96,96,112,63,31};
uint8_t bitmap_107[] = {254,254,0,0,0,0,0,128,128,128,255,255,48,56,108,198,131,3,1,0,7,7,0,0,0,0,3,7,6,4};
uint8_t bitmap_108[] = {6,6,6,6,254,254,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,1,3,7,6,6,6,3};
uint8_t bitmap_109[] = {128,128,128,128,128,0,0,128,128,128,0,255,255,1,1,1,63,63,1,1,255,255,7,7,0,0,0,0,0,0,0,7,7};
uint8_t bitmap_110[] = {128,128,128,128,128,128,128,0,0,255,255,1,1,1,1,3,255,254,7,7,0,0,0,0,0,7,7};
uint8_t bitmap_111[] = {0,0,0,128,128,128,128,128,0,0,0,252,254,135,3,1,1,1,3,135,254,252,0,1,3,7,6,6,6,7,3,1,0};
uint8_t bitmap_112[] = {128,128,128,128,128,128,128,0,0,0,255,255,1,1,1,1,3,135,254,252,127,127,6,6,6,6,7,3,1,0};
uint8_t bitmap_113[] = {0,0,0,128,128,128,128,128,128,0,252,254,135,3,1,1,1,1,255,255,0,1,3,7,6,6,6,7,127,127};
uint8_t bitmap_114[] = {0,128,128,128,128,128,128,128,255,255,1,1,1,1,1,1,7,7,0,0,0,0,0,0};
uint8_t bitmap_115[] = {0,0,128,128,128,128,128,128,0,14,31,25,49,49,49,97,227,192,3,7,6,6,6,6,6,3,1};
uint8_t bitmap_116[] = {128,128,248,248,128,128,128,128,128,1,1,255,255,1,1,1,1,1,0,0,1,3,7,6,6,6,3};
uint8_t bitmap_117[] = {128,128,0,0,0,0,0,128,128,255,255,0,0,0,0,0,255,255,1,3,7,6,6,6,6,7,7};
uint8_t bitmap_118[] = {128,128,0,0,0,0,0,0,0,128,128,1,15,127,248,192,0,192,248,127,15,1,0,0,0,1,7,7,7,1,0,0,0};
uint8_t bitmap_119[] = {128,128,0,0,0,0,0,0,0,128,128,7,255,240,128,248,28,248,128,240,255,7,0,1,7,7,1,0,1,7,7,1,0};
uint8_t bitmap_120[] = {128,128,128,0,0,0,0,0,128,128,128,0,1,131,206,252,120,124,206,131,1,0,4,6,7,1,0,0,0,1,7,6,4};
uint8_t bitmap_121[] = {128,128,0,0,0,0,0,0,0,128,128,1,15,63,248,192,0,128,240,254,31,1,96,96,96,96,51,63,31,3,0,0,0};
uint8_t bitmap_122[] = {128,128,128,128,128,128,128,128,128,1,129,193,225,57,29,15,7,1,6,7,7,6,6,6,6,6,6};
uint8_t bitmap_123[] = {0,0,0,252,254,6,6,6,6,24,24,60,255,231,0,0,0,0,0,0,0,63,127,96,96,96,96};
uint8_t bitmap_124[] = {254,254,255,255,127,127};
uint8_t bitmap_125[] = {6,6,6,6,252,252,0,0,0,0,0,0,0,231,255,60,24,24,96,96,96,96,127,63,0,0,0};
uint8_t bitmap_126[] = {0,0,0,0,0,0,0,0,0,0,0,48,24,12,12,28,24,56,48,48,24,12,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t char_width[] = {4,6,11,10,12,11,2,7,7,10,10,5,6,4,9,11,9,10,9,11,9,11,10,11,11,4,6,11,11,11,8,11,12,11,11,11,10,9,11,11,8,10,10,10,11,11,11,10,11,11,10,10,11,12,11,11,12,11,6,9,6,11,13,4,9,10,11,10,11,10,10,9,11,8,10,11,11,9,11,10,10,8,9,9,9,11,11,11,11,9,9,2,9,11};
//uint8_t* char_addr[] = {&bitmap_33[0],&bitmap_34[0],&bitmap_35[0],&bitmap_36[0],&bitmap_37[0],&bitmap_38[0],&bitmap_39[0],&bitmap_40[0],&bitmap_41[0],&bitmap_42[0],&bitmap_43[0],&bitmap_44[0],&bitmap_45[0],&bitmap_46[0],&bitmap_47[0],&bitmap_48[0],&bitmap_49[0],&bitmap_50[0],&bitmap_51[0],&bitmap_52[0],&bitmap_53[0],&bitmap_54[0],&bitmap_55[0],&bitmap_56[0],&bitmap_57[0],&bitmap_58[0],&bitmap_59[0],&bitmap_60[0],&bitmap_61[0],&bitmap_62[0],&bitmap_63[0],&bitmap_64[0],&bitmap_65[0],&bitmap_66[0],&bitmap_67[0],&bitmap_68[0],&bitmap_69[0],&bitmap_70[0],&bitmap_71[0],&bitmap_72[0],&bitmap_73[0],&bitmap_74[0],&bitmap_75[0],&bitmap_76[0],&bitmap_77[0],&bitmap_78[0],&bitmap_79[0],&bitmap_80[0],&bitmap_81[0],&bitmap_82[0],&bitmap_83[0],&bitmap_84[0],&bitmap_85[0],&bitmap_86[0],&bitmap_87[0],&bitmap_88[0],&bitmap_89[0],&bitmap_90[0],&bitmap_91[0],&bitmap_47[0],&bitmap_93[0],&bitmap_94[0],&bitmap_95[0],&bitmap_96[0],&bitmap_97[0],&bitmap_98[0],&bitmap_99[0],&bitmap_100[0],&bitmap_101[0],&bitmap_102[0],&bitmap_103[0],&bitmap_104[0],&bitmap_105[0],&bitmap_106[0],&bitmap_107[0],&bitmap_108[0],&bitmap_109[0],&bitmap_110[0],&bitmap_111[0],&bitmap_112[0],&bitmap_113[0],&bitmap_114[0],&bitmap_115[0],&bitmap_116[0],&bitmap_117[0],&bitmap_118[0],&bitmap_119[0],&bitmap_120[0],&bitmap_121[0],&bitmap_122[0],&bitmap_123[0],&bitmap_124[0],&bitmap_125[0],&bitmap_126};