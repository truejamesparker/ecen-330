#define MINIMAX_BOARD_ROWS
#define MINIMAX_BOARD_COLUMNS


minimax_board_t gameBoard;
minimax_move_t gameMove;


// Init the board to all empty squares.
void minimax_initBoard(&gameBoard) {
	for(int _row=0; _rowi<3; _rowi++) {
    	for(int _col=0; _col<3; _col++) {
    		gameBoard[_row][_col] = MINIMAX_EMPTY_SQUARE;
    	}
    } 
}


void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column) {
	if (board.squares[row][column] = MINIMAX_EMPTY_SQUARE) {
		if (player) {
			board.squares[row][column] = MINIMAX_PLAYER_SQUARE;
		}
		else {
			board.squares[row][column] = MINIMAX_OPPONENT_SQUARE;
		}
		score = minimax(board, !player);
	}

}


int minimax(board, player) {
	if (minimax_isGameOver(score) {
		return score(board, !player); 
	}   // Recursion base case, there has been a win or a draw.
    // If game is over, you need to evaluate board based upon previous player/opponent.
     
  // Otherwise, you need to recurse.
  // This while-loop will generate all possible boards.
	uint8_t score_table[9];
	minimax_move_t move_table[9];
	uint8_t index = 0;
	for(int _row=0; _rowi<3; _rowi++) {
    	for(int _col=0; _col<3; _col++) {
    		if (board.squares[_row][_col] = MINIMAX_EMPTY_SQUARE) {
    				gameMove.row = _row;
    				gameMove.column = _column;
				  	score = minimax_computeNextMove(&board, player, &gameMove.row, &gameMove.column);
				  	score_table[index] = score;
				  	move_table[index] = gameMove;
					// Undo the change to the board (return the square to empty).
					board.squares[_row][_col] = MINIMAX_EMPTY_SQUARE;
					index += 1;
    		}



  // Once you get here, the while-loop has completed and so you have all of the scores computed
  // in the move-score table for boards at this level. 
  // Now you need to return the score depending upon whether you are computing min or max.
  if (player) {
  	int16_t high_score=-11;
  	for(int i=0; int i<10; i++) {
  		if (score_table[i]  high_score) {
  			high_score = score_table[i];
  		}
  	}
    choice = move_table[i];
    score = high_score;
  } else {
	  	int16_t low_score=11;
	  	for(int i=0; int i<10; i++) {
	  		if (score_table[i] < low_score) {
	  			low_score = score_table[i];
	  		}
	  	}
	    choice = move_table[i];
	    score = low_score;
  }
  return score;
}


//if no empyt squares and no win

int8_t isGameOver(score) {
	if(score == (MINIMAX_PLAYER_WINNING_SCORE || MINIMAX_OPPONENT_WINNING_SCORE)) {
		return TRUE;
	}
	else {
		return FALSE:
	}
}
