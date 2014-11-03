#include "ticTacToeDisplay.h"
#include "supportFiles/utils.h"
#define DISPLAY_WIDTH 320
#define DISPALY_HEIGHT 240
#define THIRD_HEIGHT 240/3
#define THIRD_WIDTH 320/3
#define LINE_THICKNESS 3 // in pixels
#define X0 320/6
#define X1 320/2
#define X2 5*320/6
#define Y0 240/6
#define Y1 240/2
#define Y2 5*240/6
#define RADIUS 30
#define SWITCH0 0
#define BUTTON0 0
#define BUTTON1 1
// Display processing time delay in ms
#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 60


// Array of character locations
static uint32_t XCENTER[] = {
		X0,
		X1,
		X2
};

static uint32_t YCENTER[] = {
		Y0,
		Y1,
		Y2,
};

void ticTacToeDisplay_drawBoardLines();

uint8_t GetBit(uint8_t n, uint8_t k) {
	return ((n & (01<<k)) != 0);
}

// Inits the tic-tac-toe display, draws the lines that form the board.
void ticTacToeDisplay_init() {
	display_init();
	switches_init();
	buttons_init();
	display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
	ticTacToeDisplay_drawBoardLines();

}

// Returns whether the display is being touched
bool display_isTouched(void);

// Returns the x-y coordinate point and the pressure (z).
void display_getTouchedPoint(int16_t *x, int16_t *y, uint8_t *z);

// Throws away all previous touch data.
void display_clearOldTouchData();


// Draws an X at the specified row and column.
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column) {
	display_drawLine(XCENTER[row]-30, YCENTER[column]-30, XCENTER[row]+30, YCENTER[column]+30, DISPLAY_GREEN);
	display_drawLine(XCENTER[row]-30, YCENTER[column]+30, XCENTER[row]+30, YCENTER[column]-30, DISPLAY_GREEN);
}

// Draws an O at the specified row and column.
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column) {
	display_drawCircle(XCENTER[row], YCENTER[column], RADIUS, DISPLAY_GREEN);
}


// Runs a test of the display. Does the following.
// Draws the board. Each time you touch one of the screen areas, the screen will paint
// an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
// When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.
void ticTacToeDisplay_runTest() {
	ticTacToeDisplay_init();
	uint8_t row;
	uint8_t column;
	printf("Beginning test..\n\r");
	while(!GetBit(buttons_read(), BUTTON1)){
		if (GetBit(buttons_read(), BUTTON0)) {
			ticTacToeDisplay_init();
		}
		else if (display_isTouched()) {
			display_clearOldTouchData();
			utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
			ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);
			if (GetBit(switches_read(), SWITCH0)) {
				ticTacToeDisplay_drawO(row, column);
			}
			else {
				ticTacToeDisplay_drawX(row, column);
			}
		}
	}
	display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
	printf("Test completed!\n\r");
}

// This will draw the four board lines.
void ticTacToeDisplay_drawBoardLines() {
	display_fillRect(0, THIRD_HEIGHT, DISPLAY_WIDTH, LINE_THICKNESS, DISPLAY_GREEN);
	display_fillRect(0, 2*THIRD_HEIGHT, DISPLAY_WIDTH, LINE_THICKNESS, DISPLAY_GREEN);
	display_fillRect(THIRD_WIDTH, 0, LINE_THICKNESS, DISPALY_HEIGHT, DISPLAY_GREEN);
	display_fillRect(2*THIRD_WIDTH, 0, LINE_THICKNESS, DISPALY_HEIGHT, DISPLAY_GREEN);
}


// After a touch has been detected and after the proper delay, this sets the row and column arguments
// according to where the user touched the board.
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column) {
	int16_t x, y;  // Use these to keep track of coordinates.
	uint8_t z; // pressure
	display_getTouchedPoint(&x, &y, &z);  // Get the touched point.
	if (x < 0 || y < 0) {
		*row = 3;
		*column = 3;
	}
	else {
		// Compute row
		if (x < THIRD_WIDTH) {
			*row = 0;
		}
		else if (x > THIRD_WIDTH && x < 2*THIRD_WIDTH) {
			*row = 1;
		}
		else {
			*row = 2;
		}
		// Compute column
		if (y < THIRD_HEIGHT) {
			*column = 0;
		}
		else if (y>THIRD_HEIGHT && y<2*THIRD_HEIGHT) {
			*column = 1;
		}
		else {
			*column = 2;
		}
	}
}


