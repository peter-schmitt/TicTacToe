#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define X_BUTTON 5
#define O_BUTTON 3
#define SCROLL_BUTTON 6

#define XPOS_OFFSET 4
#define YPOS_OFFSET 5

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int theRound = 0;
unsigned long lastDebounceTime;
int debounceDelay = 50;

String board[9];

int coordinates[9][2] = {
  {((display.width() / 2) / 6) - XPOS_OFFSET, ((display.width() / 2) / 6) - YPOS_OFFSET},
  {((display.width() / 2) / 6) - XPOS_OFFSET, ((display.width() / 2) / 6) * 3 - YPOS_OFFSET},
  {((display.width() / 2) / 6) - XPOS_OFFSET, ((display.width() / 2) / 6) * 5 - YPOS_OFFSET},

  {((display.width() / 2) / 6) * 3 - XPOS_OFFSET, ((display.width() / 2) / 6) - YPOS_OFFSET},
  {((display.width() / 2) / 6) * 3 - XPOS_OFFSET, ((display.width() / 2) / 6) * 3 - YPOS_OFFSET},
  {((display.width() / 2) / 6) * 3 - XPOS_OFFSET, ((display.width() / 2) / 6) * 5 - YPOS_OFFSET},

  {((display.width() / 2) / 6) * 5 - XPOS_OFFSET, ((display.width() / 2) / 6) - YPOS_OFFSET},
  {((display.width() / 2) / 6) * 5 - XPOS_OFFSET, ((display.width() / 2) / 6) * 3 - YPOS_OFFSET},
  {((display.width() / 2) / 6) * 5 - XPOS_OFFSET, ((display.width() / 2) / 6) * 5 - YPOS_OFFSET}
};
int coStep = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64);

  pinMode(X_BUTTON, INPUT_PULLUP);
  pinMode(O_BUTTON, INPUT_PULLUP);
  pinMode(SCROLL_BUTTON, INPUT_PULLUP);

  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("Display Init");
  display.display();
  delay(2000);

  display.clearDisplay();

}

void loop() {
  // put your main code here, to run repeatedly:

  display.clearDisplay();

  drawGrid();
  drawRound();
  drawField();

  display.setCursor(120, 58);
  display.println(coStep);

  button_press_x();
  button_press_o();
  button_press_scroll();

  display.display();

  if (win_check() >= 0){
    draw_win();
  }
}

void draw_win(){
  
  display.clearDisplay();
  display.setCursor(20, 20);
  display.setTextSize(2);
  if (win_check() == 0) {
    display.println("O wins");  
  } else {
    display.println("X wins");
  }
  
  display.display();

  delay(10000);
}

// result -1: no winner
// result 0: o wins
// result 1: x wins
int win_check() {
  int winner = -1;
  // rows
  for (int i = 0; i < 8; i = i + 3) {
    if (board[i] == "x" and board[i + 1] == "x" and board[i + 2] == "x") {
      return 1;
    }

    if (board[i] == "o" and board[i + 1] == "o" and board[i + 2] == "o") {
      return 0;
    }
  }

  // columns
  for (int i = 0; i < 3; i++) {
    if (board[i] == "x" and board[i + 3] == "x" and board[i + 6] == "x") {
      return 1;
    }

    if (board[i] == "o" and board[i + 3] == "o" and board[i + 6] == "o") {
      return 0;
    }
  }

  // diag 1
  if (board[0] == "x" and board[4] == "x" and board[8] == "x") {
    return 1;
  }
  if (board[0] == "o" and board[4] == "o" and board[8] == "o") {
    return 0;
  }

  // diag 2
  if (board[2] == "x" and board[4] == "x" and board[6] == "x") {
    return 1;
  }
  if (board[2] == "o" and board[4] == "o" and board[6] == "o") {
    return 0;
  }

  return -1;
}

void button_press_x() {
  if (digitalRead(X_BUTTON) == LOW) {
    lastDebounceTime = millis();

    // ------------
    board[coStep] = "x";
    theRound++;

    while (digitalRead(X_BUTTON) == LOW or millis() - lastDebounceTime < debounceDelay);
  }
}

void button_press_o() {
  if (digitalRead(O_BUTTON) == LOW) {
    lastDebounceTime = millis();

    // ------------
    board[coStep] = "o";
    theRound++;

    while (digitalRead(O_BUTTON) == LOW or millis() - lastDebounceTime < debounceDelay);
  }
}

void button_press_scroll() {
  if (digitalRead(SCROLL_BUTTON) == LOW) {
    lastDebounceTime = millis();

    // ------------
    if (coStep == 8) {
      coStep = 0;
    } else {
      coStep++;
    }

    while (digitalRead(SCROLL_BUTTON) == LOW or millis() - lastDebounceTime < debounceDelay);
  }
}

void drawRound() {
  display.setCursor(display.height() + 5, 10);
  display.setTextSize(1);
  display.println("Runde:");

  display.setCursor(display.height() + 5, display.height() / 2);
  display.setTextSize(2);
  display.println(theRound);
}

void drawGrid() {
  display.drawLine((display.width() / 6) * 2, 0, (display.width() / 6) * 2, display.height(), WHITE);
  display.drawLine((display.width() / 6), 0, (display.width() / 6), display.height(), WHITE);

  display.drawLine(0, (display.height() / 3) * 2, display.height(), (display.height() / 3) * 2, WHITE);
  display.drawLine(0, display.height() / 3, display.height(), display.height() / 3, WHITE);

  display.drawLine(display.height() + 1, 0, display.height() + 1, display.height(), WHITE);
  display.drawLine(display.height() + 3, 0, display.height() + 3, display.height(), WHITE);
}

void drawField() {
  for (int i = 0; i < 9; i++) {
    display.setCursor(coordinates[i][0], coordinates[i][1]);

    display.setTextSize(2);
    display.println(board[i]);
    display.setTextSize(1);


    if (i == coStep) {
      display.drawCircle(coordinates[i][0] + XPOS_OFFSET, coordinates[i][1] + YPOS_OFFSET, 8, WHITE);
    }
  }
}

