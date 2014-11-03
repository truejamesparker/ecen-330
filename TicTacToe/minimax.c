#include "minimax.h"

// Global variable stores the next optimal move
minimax_move_t choice;


// Init the board to all empty squares
void minimax_initBoard(minimax_board_t* board) {
	for(int _row=0; _row<IN_ROW; _row++) {
		for(int _col=0; _col<IN_ROW; _col++) {
			board->squares[_row][_col] = MINIMAX_EMPTY_SQUARE;
    	}
    } 
}

// Print the board
void minimax_printBoard(minimax_board_t board) {
	for (int i=0; i<IN_ROW; i++) {
		for (int j=0; j<IN_ROW; j++) {
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
				printf("[?]"); // Uninitialized/corrupted square
			}
		}
		printf("\n\r");
	}
	printf("\n\r");
}


// Check if game is over
bool minimax_isGameOver(minimax_score_t score) {
	if(score != MINIMAX_NOT_ENDGAME) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Returns the score of the board, based upon the player.
int16_t minimax_computeBoardScore(minimax_board_t* _board, bool player) {
	minimax_board_t board = *_board;
	minimax_score_t winning_score;
	uint8_t mark;

	if (player) {
		mark = MINIMAX_PLAYER_SQUARE;
		winning_score = MINIMAX_PLAYER_WINNING_SCORE;
	}
	else {
		mark = MINIMAX_OPPONENT_SQUARE;
		winning_score = MINIMAX_OPPONENT_WINNING_SCORE;
	}

	// Check for row/column wins
	for (int i=0; i<IN_ROW; i++) {
		if (board.squares[i][0] == mark && board.squares[i][1] == mark && board.squares[i][2] == mark) {
			return winning_score;
		}
		if (board.squares[0][i] == mark && board.squares[1][i] == mark && board.squares[2][i] == mark) {
			return winning_score;
		}
	}
	// Check for diagonal wins
	if (board.squares[0][0] == mark && board.squares[1][1] == mark && board.squares[2][2] == mark) {
		return winning_score;
	}
	if (board.squares[2][0] == mark && board.squares[1][1] == mark && board.squares[0][2] == mark) {
		return winning_score;
	}
	// Check for not end of game
	for(int _row=0; _row<IN_ROW; _row++) {
		for (int _col=0; _col<IN_ROW; _col++) {
			if (board.squares[_row][_col] == MINIMAX_EMPTY_SQUARE) {
				return MINIMAX_NOT_ENDGAME;
			}
		}
	}
	// It's a draw!
	return MINIMAX_DRAW_SCORE;
}


int minimax(minimax_board_t board, bool player) {
	minimax_score_t currentScore = minimax_computeBoardScore(&board, !player);
	uint8_t gameOver = minimax_isGameOver(currentScore);
	if (gameOver) {
		return currentScore;
	}   // Recursion base case, there has been a win or a draw.
    // If game is over, you need to evaluate board based upon previous player/opponent.
     
  // Otherwise, you need to recurse.
  // This while-loop will generate all possible boards.
	minimax_score_t score_table[9];
	minimax_move_t move_table[9];
	minimax_move_t nextMove;
	minimax_score_t score;
	int8_t mark;
	uint8_t index = 0;
	for(int _row=0; _row<IN_ROW; _row++) {
    	for(int _col=0; _col<IN_ROW; _col++) {
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

	minimax_score_t scoreChoice;

	// Choose greatest value if player (max)
	if (player) {
		int16_t high_score= MINIMAX_OPPONENT_WINNING_SCORE - 1;
		for (int i=0; i<index; i++) {
			//printf("Score: %d\n\r", score_table[i]);
			if (score_table[i] > high_score) {
				high_score = score_table[i];
				choice = move_table[i];
			}
		}
		scoreChoice = high_score;
	}

	// Choose least value if !player (min)
	else {
		int16_t low_score= MINIMAX_PLAYER_WINNING_SCORE + 1;
		for(int i=0; i<index; i++) {
			//printf("Score: %d\n\r", score_table[i]);
	  		if (score_table[i] < low_score) {
	  			low_score = score_table[i];
	  			choice = move_table[i];
	  		}
		}

		scoreChoice = low_score;
	}

  return scoreChoice;
}

// Compute the next optimal move
void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column) {
	//minimax_printBoard(*board); // Debug print
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
	board->squares[*row][*column] = mark; // Debug function for self-play
}
