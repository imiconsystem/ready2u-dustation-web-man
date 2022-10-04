#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

U8G2LOG u8g2log;

#define U8LOG_WIDTH 128
#define U8LOG_HEIGHT 32

uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];

void oledSetup(void) {
  u8g2.begin();
}

void oledLogSetup(void) {
  u8g2.setFont(u8g2_font_squeezed_b7_tr);
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);
  u8g2log.setRedrawMode(0);
}


void oledDrawPage2() {

  u8g2.setFont(u8g2_font_emoticons21_tr);
  u8g2.drawUTF8(100, 25, pm25lev.icon.c_str());  // :)
  u8g2.setFont(u8g2_font_helvR08_tf);
  u8g2.drawStr(5, 12, "Thai AQI :");
  u8g2.drawStr(60, 12, u8x8_u8toa(AQI, 3));
  u8g2.drawUTF8(5, 25, pm25lev.word.c_str());  // :)
}

void oledDrawPage3() {

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(0, 8, 0xE219);   //wifi
    u8g2.drawGlyph(0, 16, 0xE0F2);  //rssi

    u8g2.setFont(u8g2_font_squeezed_b7_tr);
    u8g2.drawStr(15, 8, storageGetString("WiFissid").c_str());
    u8g2.drawStr(15, 17, WiFiRSSI.c_str());
    u8g2.drawStr(50, 17, myIP.c_str());
    u8g2.drawStr(50, 26, storageGetString("deviceName").c_str());
  
}


void oledDrawMain(void) {
  u8g2.setFont(u8g2_font_squeezed_b7_tr);
  u8g2.drawStr(0, 8, "PM2.5");

  u8g2.drawStr(9, 23, u8x8_u8toa(TEMP>99?99:TEMP, 2));
  u8g2.drawStr(9, 32, u8x8_u8toa(HUMI>99?99:HUMI, 2));

  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.drawGlyph(-4, 23, 0xE01D);  //icon termo
  u8g2.drawGlyph(-3, 33, 0xE22B);  //icon humi

  u8g2.setFont(u8g2_font_6x12_t_symbols);
  u8g2.drawGlyph(22, 23, 0x2103);  //c
  u8g2.drawGlyph(22, 32, 0x0025);  //%

  u8g2.setFont(u8g2_font_squeezed_r7_tr);
  u8g2.drawStr(93, 8, "PM1:");
  u8g2.drawStr(115, 8, u8x8_u8toa(PM1>999?999:PM1, 3));
  u8g2.drawStr(93, 16, "PM10:");
  u8g2.drawStr(115, 16, u8x8_u8toa(PM10>999?999:PM10, 3));

  u8g2.drawStr(97, 31, "g/m^3");

  u8g2.setFont(u8g2_font_6x12_t_symbols);
  u8g2.drawGlyph(91, 30, 0X00B5);  //u

  u8g2.setFont(u8g2_font_7_Seg_33x19_mn);
  u8g2.drawStr(30, 0, u8x8_u8toa(PM2>999?999:PM2, 3));

}

uint8_t draw_state = 0;

void oledDrawState(void) {
  switch (draw_state) {
    case 0: oledDrawMain(); break;
    case 1: oledDrawPage2(); break;
    case 2:
      oledDrawPage3();
      break;

  }
}
void oledLogLoop() {
  u8g2log.print(logString);
  u8g2log.print("\n");
  delay(1000);
}
void oledLoop(void) {

  u8g2.firstPage();
  do {
    oledDrawState();
  } while (u8g2.nextPage());

  draw_state++;
  if (draw_state > 2)
    draw_state = 0;
  delay(3000);
}