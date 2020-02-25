#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>

const int tftWidth = 240, tftHeight = 320;
const int SD_CS_PIN = 4, TFT_CS_PIN = 5;
const int plastifierId = 1, waterId = 2, sandId = 3;

enum DISPLAY_STATUS { START, SELECT, UPDATE };
enum COMPONENTS { NONE, PLASTIFIER, WATER, SAND };

int ColorPaletteHigh = 60;
int color = WHITE;  //Paint brush color
unsigned int colors[4] = {RED, BLUE, YELLOW, GRAY1};

int relayArray[] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37 };
//char * buttonText[] = { "bn drp", "bn ld" };

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM); //init TouchScreen port pins

// END GLOBAL VAR

/** Define Pins, set pins low let library handle SS/CS */
void setTFTPinDefinitions() {
  // TF_CS SD card select input
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);
  // TFT_CS TFT chip select
  pinMode(TFT_CS_PIN, OUTPUT);
  digitalWrite(TFT_CS_PIN, HIGH);
}

/** Initialize TFT and draw start screen */
void initTFTouchScreen() {
  //init TFT library
  Tft.TFTinit();
  drawRelayButtons();
}

void drawRelayButtons() {
  // max size of a byte is 255
  byte squareSize = 50;
  byte margin = 5;
  byte row = 0, col = 0;
  byte maxRow = 3, maxCol = 3;


  for (byte i = 0; i < (sizeof(relayArray) / sizeof(relayArray[0])); i++) {
    int x = margin;
    int y = 0;

    x += (col * squareSize) + (margin * col);

    if (row > 0) {
      y = (row * squareSize) + (margin * row);
    }

    drawSquare(x, y, i, squareSize);

    if (col == maxCol) {
      row++;
      col = 0;
      Serial.println("increased rowcount");
    }
    else {
      col++;
    }



  }
}

/** Draw select component info with temporary plus and minus buttons */
void drawSquare(int x, int y, byte index, byte size) {

  //byte Xmargin =

  Tft.fillRectangle(x, y, size, size, RED);
  drawRelayButtonText(x, y, index);
}

void drawRelayButtonText(int x, int y, byte index) {
  byte fontSize = 2;
  if (index < 2) {
    Tft.drawString("hoi", x, y, fontSize, WHITE);
  }
}


/** Process touch input if valid pressure detected */
void processTouch() {
  // a point object holds x y and z coordinates.
  Point p = ts.getPoint();
  //map the ADC value read to into pixel co-ordinates
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tftWidth);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tftHeight);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > __PRESURE) {
    readTouchInput(p.x, p.y);
  }
}

void readTouchInput(int x, int y) {
  // Detect component select change
  Serial.println("Got touch!");

  Serial.print("y = ");
  Serial.println(y);
  Serial.print("x = ");
  Serial.println(x);
}
