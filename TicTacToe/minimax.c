#define MINIMAX_BOARD_ROWS
#define MINIMAX_BOARD_COLUMNS


typedef struct {
	uint8_t squares[MINIMAX_BOARD_ROWS][MINIMAX_BOARD_COLUMNS];  // State of game as passed in.
} minimax_board_t;


int minimax(board, player) {
  if (game is over)   // Recursion base case, there has been a win or a draw.
    // If game is over, you need to evaluate board based upon previous player/opponent.
    return score(board, !player);  
  // Otherwise, you need to recurse.
  // This while-loop will generate all possible boards.
  while (there are more moves possible) {  
  	move = (1,1)
    move <= next move  // A move is a row-column pair where you will place an X or an O.
    // Assign the square to the player or opponent.
    if (player) 
      board(r, c) <= PLAYER_SQUARE  
    else 
      board(r, c) <= OPPONENT_SQUARE


    score = minimax(board, !player)


    add score to move-score table
    add move to move-score table
    // Undo the change to the board (return the square to empty).
    board(r, c) <= EMPTY_SQUARE
  }
  // Once you get here, the while-loop has completed and so you have all of the scores computed
  // in the move-score table for boards at this level. 
  // Now you need to return the score depending upon whether you are computing min or max.
  if (player) {
    choice <= get the move with the highest score in the move-score table.
    score <= highest score in the move-score table.
  } else {
    choice <= get the move with the lowest score in the move-score table.
    score <= lowest score in the move-score table. 
  }
  return score;
}


//if no empyt squares and no win

int8_t isGameOver(score) {
	if player!
}
