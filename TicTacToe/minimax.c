#include "minimax.h"
#define MINIMAX_BOARD_ROWS 3
#define MINIMAX_BOARD_COLUMNS 3


minimax_board_t gameBoard;
minimax_move_t choice;


// Init the board to all empty squares.
void minimax_initBoard(minimax_board_t* board) {
	for(int _row=0; _row<3; _row++) {
		for(int _col=0; _col<3; _col++) {
			board->squares[_row][_col] = MINIMAX_EMPTY_SQUARE;
    	}
    } 
}

void minimax_printBoard(minimax_board_t board) {
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			uint8_t mark = board.squares[i][j];
			if (mark == MINIMAX_PLAYER_SQUARE) {
				printf("[X]");
			}
			else if (mark == MINIMAX_OPPONENT_SQUARE) {
				printf("[O]");
			}
			else if (mark == MINIMAX_EMPTY_SQUARE){
				printf("[ ]");
			}
			else {
				printf("[U]");
			}
		}
		printf("\n\r");
	}
}


//if no empty squares and no win
bool minimax_isGameOver(minimax_score_t score) {
	if(score == MINIMAX_PLAYER_WINNING_SCORE || score == MINIMAX_OPPONENT_WINNING_SCORE || score == MINIMAX_DRAW_SCORE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Returns the score of the board, based upon the player.
int16_t minimax_computeBoardScore(minimax_board_t board, bool player) {
	minimax_score_t winning_score;
	uint8_t winning_product;
	uint8_t col_score[3];
	uint8_t row_score[3];
	int8_t row_product;
	int8_t col_product;

	if (player) {
		winning_product = MINIMAX_PLAYER_SQUARE * IN_ROW;
		winning_score = MINIMAX_PLAYER_WINNING_SCORE;
	}
	else {
		winning_product = MINIMAX_OPPONENT_SQUARE * IN_ROW;
		winning_score = MINIMAX_OPPONENT_WINNING_SCORE;
	}

	// Calculating row/col products
	for (int i=0; i<IN_ROW; i++) {
		row_product = (board.squares[i][0] * board.squares[i][1] * board.squares[i][2]);
		row_score[i] = row_product;
		col_product = (board.squares[0][i] * board.squares[1][i] * board.squares[2][i]);
		col_score[i] = col_product;
	}
	// Checking row/col products for win
	for(int i=0; i<IN_ROW; i++) {
		if (col_score[i] == winning_product || row_score[i] == winning_product) {
			return winning_score;
		}
	}
	// Check for diagonal wins
	if ((board.squares[0][0] * board.squares[1][1] * board.squares[2][2]) == winning_product) {
		return winning_score;
	}
	if ((board.squares[2][0] * board.squares[1][1] * board.squares[0][2]) == winning_product) {
		return winning_score;
	}
	// Checking for not end of game
	for(int i=0; i<IN_ROW; i++) {
		if (col_score[i] == 0 || row_score[i] == 0) {
			return MINIMAX_NOT_ENDGAME;
		}
	}
	return MINIMAX_DRAW_SCORE;
}


int minimax(minimax_board_t board, bool player) {
	minimax_score_t currentScore = minimax_computeBoardScore(board, !player);
	uint8_t gameOver = minimax_isGameOver(currentScore);
	if (gameOver) {
		return currentScore;
	}   // Recursion base case, there has been a win or a draw.
    // If game is over, you need to evaluate board based upon previous player/opponent.
     
  // Otherwise, you need to recurse.
  // This while-loop will generate all possible boards.
	uint8_t score_table[9];
	minimax_move_t move_table[9];
	minimax_move_t nextMove;
	minimax_score_t score;
	int8_t mark;
	uint8_t index = 0;
	for(int _row=0; _row<3; _row++) {
    	for(int _col=0; _col<3; _col++) {
    		if (board.squares[_row][_col] == MINIMAX_EMPTY_SQUARE) {
    				nextMove.row = _row;
    				nextMove.column = _col;
    				if (player) {
    					mark = MINIMAX_PLAYER_SQUARE;
    				}
    				else {
    					mark = MINIMAX_OPPONENT_SQUARE;
    				}
    				board.squares[nextMove.row][nextMove.column] = mark;
    				//minimax_printBoard(board);
				  	score = minimax(board, !player);
				  	score_table[index] = score;
				  	move_table[index] = nextMove;
					// Undo the change to the board (return the square to empty).
					board.squares[_row][_col] = MINIMAX_EMPTY_SQUARE;
					index += 1;
    		}
    	}
	}

  // Once you get here, the while-loop has completed and so you have all of the scores computed
  // in the move-score table for boards at this level. 
  // Now you need to return the score depending upon whether you are computing min or max.

	minimax_score_t scoreChoice;
	if (player) {
		int16_t high_score= MINIMAX_OPPONENT_WINNING_SCORE - 1;
		for (int i=0; i<index; i++) {
			printf("Score: %d", score_table[i]);
			if (score_table[i] > high_score) {
				high_score = score_table[i];
				choice = move_table[i];
			}
		}
		scoreChoice = high_score;
	}

	else {
		int16_t low_score= MINIMAX_PLAYER_WINNING_SCORE + 1;
		for(int i=0; i<index; i++) {
			printf("Score: %d", score_table[i]);
	  		if (score_table[i] < low_score) {
	  			low_score = score_table[i];
	  			choice = move_table[i];
	  		}
		}

		scoreChoice = low_score;
	}
  return scoreChoice;
}


void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column) {
	minimax_printBoard(*board);
	uint8_t mark;
	if (player) {
		mark = MINIMAX_PLAYER_SQUARE;
	}
	else {
		mark = MINIMAX_OPPONENT_SQUARE;
	}
	minimax(*board, player);
	*row = choice.row;
	*column = choice.column;
	board->squares[*row][*column] = mark;
}
