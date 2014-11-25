#include "TFTLCD.h"
#include "TouchScreen.h"


//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontroller:

// Resolution: 240x320

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM  7 // can be a digital pin
#define XP  6  // can be a digital pin

#define TS_MINX 150 // TouchScreen min X
#define TS_MINY 120 // TouchScreen min Y
#define TS_MAXX 920 // TouchScreen max X
#define TS_MAXY 940 // TouchScreen max Y

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3 // Chip select
#define LCD_CD A2 // Command/display
#define LCD_WR A1 // Write
#define LCD_RD A0 // Read
// optional
#define LCD_RESET A4

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

boolean isBoard = false;

int choice = 0; // 1 = X, 2 = O
int selectedRow = -1;

// board matrix
int board[3][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

void initScreen() {
  tft.reset();
  tft.initDisplay();
  tft.fillScreen(WHITE);
}

void drawButtons() {
  // fillRect(x, y, w, h, c)
  tft.fillRect(10, (tft.height() / 3) + 100, 100, 75, BLUE);
  tft.drawString(45, (tft.height() / 3) + 100 + 15, "X", RED, 6);
  
  tft.fillRect(130, (tft.height() / 3) + 100, 100, 75, RED);
  tft.drawString(165, (tft.height() / 3) + 100 + 15, "O", BLUE, 6);
}

void drawTitle() {
  // drawString(x, y, text, color, fontSize)
  tft.drawString((tft.width() / 3) - 30, (tft.height() / 3) - 70, "Tic", GREEN, 4);
  tft.drawString((tft.width() / 3), (tft.height() / 3) + 30 - 70, "Tac", GREEN, 4);
  tft.drawString((tft.width() / 3) + 30, (tft.height() / 3) + 60 - 70, "Toe", GREEN, 4);
}

void drawGrid() {
  // Horizontal line
  tft.fillRect(10, (tft.height() / 3), tft.width() - 20, 5, YELLOW);
  // Vertical line
  tft.fillRect((tft.width() / 3), 10, 5, tft.height() - 20, YELLOW);
  
  // Horizontal line
  tft.fillRect(10, (tft.height() / 3) * 2, tft.width() - 20, 5, YELLOW);
  // Vertical line
  tft.fillRect((tft.width() / 3) * 2, 10, 5, tft.height() - 20, YELLOW);
}

void drawBoard() {
  initScreen();
  
  tft.setRotation(1); // landscape mode
  
  drawGrid();
}

void defineButtons(Point p) {
  if (320-p.y > (tft.height() / 3) + 75 && 320-p.y < ((tft.height() / 3) + 75 +75)) {
    if (p.x > 145 && p.x < 225) {
      Serial.println("X button");
      
      drawBoard();
      
      isBoard = true;
      choice = 1;
    }
    else if (p.x > 25 && p.x < 115) {
      Serial.println("O button");
      
      drawBoard();
      
      isBoard = true;
      choice = 2;
    }
  }
}

void drawToken(int x, int y) {
  tft.initDisplay();
  
  switch (choice) {
    case 1: // X
      tft.drawString(x, y, "X", RED, 6);
      Serial.println("X Token");
      break;
    case 2: // O
      tft.drawString(x, y, "O", BLUE, 6);
      Serial.println("O Token");
      break;
  }
  
  changeTurn();
}

boolean isCellAvailable(int row, int col) {
  boolean isAvailable = false;
  
  if (board[row][col] == 0) {
    isAvailable =  true;
  }
  
  return isAvailable;
}

void changeTurn() {
  choice++;
  
  if (choice > 2) {
    choice = 1;
  }
}

void defineCells(Point p) {
  int xcoord = 0;
  
  int y0 = 20;
  int y1 = (tft.height() / 3) + 20;
  int y2 = ((tft.height() / 3) * 2) + 20;
  
  if (p.y > 0 && p.y < (tft.width() / 3) - 20) {
    Serial.println("Cell 3");
    
    xcoord = (((tft.width()) / 3) * 2 ) + 40;
    
    switch (selectedRow) {
      case 0: // Row 1
        if (isCellAvailable(selectedRow, 2)) {
          board[selectedRow][2] = choice;
          drawToken(xcoord, y0);
        }
        break;
      case 1: // Row 2
        if (isCellAvailable(selectedRow, 2)) {
          board[selectedRow][2] = choice;
          drawToken(xcoord, y1);
        }
        break;
      case 2: // Row 3
        if (isCellAvailable(selectedRow, 2)) {
          board[selectedRow][2] = choice;
          drawToken(xcoord, y2);
        }
        break;
    }
  }
  else if (p.y > (tft.width() / 3) - 10 && p.y < ((tft.width() / 3) * 2) - 50) {
    Serial.println("Cell 2");
    
    xcoord = (tft.width() / 3) + 40;
    
    switch (selectedRow) {
      case 0: // Row 1
        if (isCellAvailable(selectedRow, 1)) {
          board[selectedRow][1] = choice;
          drawToken(xcoord, y0);
        }
        break;
      case 1: // Row 2
        if (isCellAvailable(selectedRow, 1)) {
          board[selectedRow][1] = choice;
          drawToken(xcoord, y1);
        }
        break;
      case 2: // Row 3
        if (isCellAvailable(selectedRow, 1)) {
          board[selectedRow][1] = choice;
          drawToken(xcoord, y2);
        }
        break;
    }
  }
  else if (p.y > ((tft.width() / 3) * 2) - 50 + 13 && p.y < tft.width()) {
    Serial.println("Cell 1");
    
    xcoord = 40;
    
    switch (selectedRow) {
      case 0: // Row 1
        if (isCellAvailable(selectedRow, 0)) {
          board[selectedRow][0] = choice;
          drawToken(xcoord, y0);
        }
        break;
      case 1: // Row 2
        if (isCellAvailable(selectedRow, 0)) {
          board[selectedRow][0] = choice;
          drawToken(xcoord, y1);
        }
        break;
      case 2: // Row 3
        if (isCellAvailable(selectedRow, 0)) {
          board[selectedRow][0] = choice;
          drawToken(xcoord, y2);
        }
        break;
    }
  }
  
  printMatrix();
}

void defineRows(Point p) {
  if (320-p.x > 0 + 10 && 320-p.x < (tft.height() / 3) - 5) {
    Serial.println("Row 3");
    
    selectedRow = 2;
    
    defineCells(p);
  }
  
  if (320-p.x > (tft.height() / 3) + 10 && 320-p.x < ((tft.height() / 3) * 2) + 10) {
    Serial.println("Row 2");
    
    selectedRow = 1;
    
    defineCells(p);
  }
  
  if (320-p.x > ((tft.height() / 3) * 2) + 60 && 320-p.x < tft.width() - 10) {
    Serial.println("Row 1");
    
    selectedRow = 0;
    
    defineCells(p);
  }
}

void printMatrix() {
  Serial.println("==BEGIN==");
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Serial.print(board[i][j]);
    }
  }
  
  Serial.println();
  Serial.println("==END==");
}

void setup() {
  Serial.begin(57600);
  Serial.print("Tic-Tac-Toe");
  Serial.println();
  
  initScreen();
  
  tft.setRotation(0); // portrait mode
  
  drawTitle();
  
  tft.drawString(tft.width() / 3, (tft.height() / 3) + 30, "Kibur", BLACK, 2);
  
  drawButtons();
  
  pinMode(13, OUTPUT);
  
  printMatrix();
}

void loop() {
  digitalWrite(13, HIGH);
  Point p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    
    if (isBoard) {
      defineRows(p);
    }
    else {
      defineButtons(p);
    }
  }
}

