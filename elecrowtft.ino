#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>

const int squareSize = 50;
const int tftWidth = 240, tftHeight = 320;
const int SD_CS_PIN = 4, TFT_CS_PIN = 5;

int ColorPaletteHigh = 60;
int color = WHITE;  //Paint brush color
unsigned int colors[8] = {BLACK, RED, GREEN, BLUE, CYAN, YELLOW, WHITE, GRAY1};

const int relayArray[] = { 22, 24, 26, 28, 30, 32, 34, 36, 23, 25, 27, 29, 31, 33, 35, 37 };
const char * buttonText[] = { "L up", "Ldwn", "drop", "load", "1FWD", "1REV", "2FWD", "2REV", "VSOP", "VSCL", "MXUP", "MXDW", "HYD", "VAC", "BFWD", "BREV" };

unsigned long prevTouchMilis = 0;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
const int touchIntervalMs = 350;
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


void initRelayArray() {
  for (byte i = 0; i < (sizeof(relayArray) / sizeof(relayArray[0])); i++) {
    pinMode(relayArray[i], OUTPUT);
    digitalWrite(relayArray[i], HIGH);
  }
}

/** Initialize TFT and draw start screen */
void initTFTouchScreen() {
  setTFTPinDefinitions();
  initRelayArray();  
  //init TFT library
  Tft.TFTinit();
  drawRelayButtons();
}

void drawRelayButtons() {
  byte margin = 5;
  byte row = 0, col = 0;
  byte maxRow = 3, maxCol = 3;


  for (int i = 0; i < (sizeof(relayArray) / sizeof(relayArray[0])); i++) {
    int x = margin;
    int y = 0;

    x += (col * squareSize) + (margin * col);

    if (row > 0) {
      y = (row * squareSize) + (margin * row);
    }

    drawSquare(x, y, i);

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
void drawSquare(int x, int y, byte index) {
  if(isRelayLOW(index)) {
    Serial.println("draw green");
    Tft.fillRectangle(x, y, squareSize, squareSize, GREEN);
  }
  else {
    Serial.println("draw red");
    Tft.fillRectangle(x, y, squareSize, squareSize, RED);
  }

  drawRelayButtonText(x, y, index);
}

void drawRelayButtonText(int x, int y, byte index) {
  byte fontSize = 2;
  byte margin = 15;    
  Tft.drawString(buttonText[index], x, y + margin, fontSize, WHITE);  
}


/** Process touch input if valid pressure detected */
void processTouch() {

  if ((long)millis() - prevTouchMilis > touchIntervalMs) {
    // a point object holds x y and z coordinates.
    Point p = ts.getPoint();
    //map the ADC value read to into pixel co-ordinates
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tftWidth);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tftHeight);

    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    if (p.z > __PRESURE) {
      prevTouchMilis = millis();
      readTouchInput(p.x, p.y);
    }
  }
}

void readTouchInput(int x, int y) {
  // Detect component select change
  const int margin = 5;
  const int cel0max = 55, cel1max = 110, cel2max = 165, cel3max = 220;

  Serial.println("Got touch!");
  Serial.print("y = ");
  Serial.println(y);
  Serial.print("x = ");
  Serial.println(x);

  if(y < cel0max) {
    if(x < cel0max) {
      Serial.println("row 0 cel 0");
      toggleRelay(0);
      drawSquare(5, 0, 0);
    }
    else if(x > cel0max && x < cel1max) {
      Serial.println("row 0 cel 1");
      toggleRelay(1);
      drawSquare(cel0max + margin, 0, 1);
    } 
    else if(x > cel1max && x < cel2max) {
      Serial.println("row 0 cel 2");
      toggleRelay(2);
      drawSquare(cel1max  + margin, 0, 2);
    } 
    else if(x > cel2max && x < cel3max) {
      Serial.println("row 0 cel 3");
      toggleRelay(3);
      drawSquare(cel2max + margin, 0, 3);
    } 
  }
  else if(y < cel1max) {
    if(x < cel0max) {
      Serial.println("row 1 cel 0");
      toggleRelay(4);
      drawSquare(5, cel0max, 4);
    }
    else if(x > cel0max && x < cel1max) {
      Serial.println("row 1 cel 1");
      toggleRelay(5);
      drawSquare(cel0max + margin, cel0max, 5);
    } 
    else if(x > cel1max && x < cel2max) {
      Serial.println("row 1 cel 2");
      toggleRelay(6);
      drawSquare(cel1max + margin, cel0max, 6);
    } 
    else if(x > cel2max && x < cel3max) {
      Serial.println("row 1 cel 3");
      toggleRelay(7);
      drawSquare(cel2max + margin, cel0max, 7);
    } 
  }
  else if(y < cel2max) {
    if(x < cel0max) {
      Serial.println("row 2 cel 0");
      toggleRelay(8);
      drawSquare(5, cel1max, 8);
    }
    else if(x > cel0max && x < cel1max) {
      Serial.println("row 2 cel 1");
      toggleRelay(9);
      drawSquare(cel0max + margin, cel1max, 9);
    } 
    else if(x > cel1max && x < cel2max) {
      Serial.println("row 2 cel 2");
      toggleRelay(10);
      drawSquare(cel1max + margin, cel1max, 10);
    } 
    else if(x > cel2max && x < cel3max) {
      Serial.println("row 2 cel 3");
      toggleRelay(11);
      drawSquare(cel2max + margin, cel1max, 11);
    } 
  }
   else if(y < cel3max) {
    if(x < cel0max) {
      Serial.println("row 3 cel 0");
      toggleRelay(12);
      drawSquare(5, cel2max, 12);
    }
    else if(x > cel0max && x < cel1max) {
      Serial.println("row 3 cel 1");
      toggleRelay(13);
      drawSquare(cel0max + margin, cel2max, 13);
    } 
    else if(x > cel1max && x < cel2max) {
      Serial.println("row 3 cel 2");
      toggleRelay(14);
      drawSquare(cel1max + margin, cel2max, 14);
    } 
    else if(x > cel2max && x < cel3max) {
      Serial.println("row 3 cel 2");
      toggleRelay(15);
      drawSquare(cel2max + margin, cel2max, 15);
    }  
  }
}

void toggleRelay(int index) {
  digitalWrite(relayArray[index], !digitalRead(relayArray[index]));
}

bool isRelayLOW(int index) {
  return digitalRead(relayArray[index]) == LOW ? true : false;
}
