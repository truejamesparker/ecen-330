#include "clockControl.h"

// Timer values computed at a 10 ms tick rate
#define ticksPerSecond 100
#define adTimerExpired 5
#define autoTimerExpired 50
#define rateTimerExpired 10

// Bool values
#define TRUE 1
#define FALSE 0

// Clock states
enum TicTacToeStates {
	start_st,
	init_st,
	waiting_for_touch_st,
	ad_timer_running_st,
	auto_timer_running_st,
	rate_timer_running_st,
	rate_timer_expired_st
};

// Initialize clock state
ClockStates ClockState = start_st;

// CLock tick function
void clockControl_tick() {
	static uint16_t adTimer;
	static uint16_t autoTimer;
	static uint16_t rateTimer;
	static uint16_t periodCounter;
	static uint8_t firstTouch = FALSE;

	switch(ClockState) { // Transitions
		case start_st: // start state (transition to init)
			//printf("In start_st\n\r");
			ClockState = init_st;
			//printf("Entering init_st\n\r");
			break;

		case init_st: // init state
			//printf("In init_st\n\r");
			ClockState = waiting_for_touch_st;
			periodCounter = 0;
			//printf("Entering waiting_for_touch_st\n\r");
			break;

		case waiting_for_touch_st: // wait for user to touch screen
			if (display_isTouched()) {
				ClockState = ad_timer_running_st;
				periodCounter = 0;
				firstTouch = TRUE; // variable set on first touch
				display_clearOldTouchData();
				//printf("Entering ad_timer_running_st\n\r");
			}
			// If the user has touched the screen once already advance the clock
			// once per second if the user is not touching the LCD screen
			else if (periodCounter == ticksPerSecond && firstTouch) {
				clockDisplay_advanceTimeOneSecond();
				clockDisplay_updateTimeDisplay(FALSE);
				periodCounter = 0;
			}
			break;

		case ad_timer_running_st: 	// auto debounce state
			// Timer did not expire before screen was untouched
			// (i.e. glitch was detected and discarded)
			// NOTE: this transition was neglected on the state diagram
			if (adTimer<adTimerExpired && !display_isTouched()) {
				ClockState = waiting_for_touch_st;
				//printf("Entering waiting_for_touch_st\n\r");
			}
			// Timer expired but the screen is still being touched
			else if (adTimer==adTimerExpired && display_isTouched()) {
				ClockState = auto_timer_running_st;
				autoTimer = 0;
				//printf("Entering auto_timer_running_st\n\r");
			}
			// Timer expired and the screen is not being touched
			else if (adTimer==adTimerExpired && !display_isTouched()) {
				ClockState = waiting_for_touch_st;
				clockDisplay_performIncDec();
				//printf("Entering waiting_for_touch_st\n\r");
			}
			break;

		case auto_timer_running_st: 	// auto increment
			// Display is not being touched
			if (!display_isTouched()) {
				ClockState = waiting_for_touch_st;
				clockDisplay_performIncDec();
				//printf("Entering waiting_for_touch_st\n\r");
			}
			// Timer expired and display is still being touched
			else if (autoTimer==autoTimerExpired && display_isTouched()) {
				ClockState = rate_timer_running_st;
				clockDisplay_performIncDec();
				//printf("Entering rate_timer_running_st\n\r");
				rateTimer = 0;
			}
			break;

		case rate_timer_running_st: 	// High speed increment
			// Display is not touched
			if (!display_isTouched()) {
				ClockState = waiting_for_touch_st;
				//printf("Entering waiting_for_touch_st\n\r");
			}
			// Timer is expired and display is still being touched
			else if (rateTimer==rateTimerExpired && display_isTouched()) {
				ClockState = rate_timer_expired_st;
				//printf("Entering rate_timer_expired_st\n\r");
			}
			break;

		case rate_timer_expired_st: 	// Perform inc/dec 10x per sec
			if(display_isTouched()) {
				ClockState = rate_timer_running_st;
				clockDisplay_performIncDec();
				//printf("Entering rate_timer_running_st\n\r");
			}
			else {
				ClockState = waiting_for_touch_st;
				//printf("Entering waiting_for_touch_st\n\r");
			}
			break;

		default: 	// Default case
			ClockState = start_st;
			break;
	}

	// State actions
	switch(ClockState) {
		case init_st:
			//clockDisplay_init();
		case waiting_for_touch_st:
			periodCounter++;
			adTimer = 0;
		case ad_timer_running_st:
			adTimer++;
			break;
		case auto_timer_running_st:
			autoTimer++;
			break;
		case rate_timer_running_st:
			rateTimer++;
			break;
		case rate_timer_expired_st:
			rateTimer=0;
			break;
		default:
			break;
	}
}
