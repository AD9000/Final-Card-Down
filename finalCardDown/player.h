// The player of Final Card-Down.
// This is the player ADT. It can be implemented in 
// different ways to create difficulty settings for the user.

#include "Game.h"

// This function is to be implemented by the AI.
// It will be called when the player can take an action on their turn.
// The function is called repeatedly until they make the action
// END_TURN.
// If the player's turn is skipped, this funciton is not called for that
// player.
playerMove decideMove(Game game);
