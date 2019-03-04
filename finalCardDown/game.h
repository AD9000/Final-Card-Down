// The game of Final Card-Down. v1.6 (updated 07:50am 09/Oct)
//
// !!! DO NOT CHANGE THIS FILE !!!


#include "Card.h"

#ifndef GAME_H
#define GAME_H

#define NUM_PLAYERS 4

#define NOT_FINISHED -1
#define NO_WINNER 4

#define FALSE 0
#define TRUE (!FALSE)

typedef struct _game *Game;


typedef enum {
    // Draw a single card from the deck.
    DRAW_CARD,
    // Play a single card onto the discard pile.
    PLAY_CARD,
    // End the player's turn.
    END_TURN
} action;

typedef struct _playerMove {
    // Which action to play.
    action action;
    // Declare which color must be played on the next turn.
    // This is only used when playing a "D". 
    color nextColor;
    // Which card to play (only valid for PLAY_CARD).
    Card card;
} playerMove;

// Create a new game engine.
//
// This creates a game with a deck of the given size
// and the value, color, and suit of each card to be taken from
// each of the arrays in order.
//
// Your game will always have 4 players. Each player starts with a hand
// of 7 cards. The first card from the deck is given to player 0, the
// second to player 1, the third to player 2, the fourth to player 3,
// the fifth to player 0, and so on until each player has 7 cards.
Game newGame(int deckSize, value values[], color colors[], suit suits[]);

// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
void destroyGame(Game game);

// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.
int numCards(Game game);

// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit(Game game, suit suit);

// Get the number of cards in the initial deck of a particular color.
int numOfColor(Game game, color color);

// Get the number of cards in the initial deck of a particular value.
int numOfValue(Game game, value value);

// Get the number of the player whose turn it is.
int currentPlayer(Game game);

// Get the current turn number.
//
// The turn number increases after a player ends their turn.
// The turn number should start at 0 once the game has started.
int currentTurn(Game game);


// Get the card that is on the top of the discard pile.
Card topDiscard(Game game);

// This function returns the number of turns that have occurred in the
// game so far including the current turn.
// (i.e. on turn 0 of the game, this should return 1, as there has been
//       1 turn so far including the current turn.)
int numTurns(Game game);

// Get the number of moves (actions) that happened on the current turn.
//
// A turn may consist of multiple moves (actions) such as drawing cards,
// playing cards, and ending the turn.
int currentTurnMoves(Game game);


// Get the number of consecutive cards with value "2" at the top of the 
// discard pile.
// If the value of the top of the discard pile is not "2", the function will 
// return zero. 
int getNumberOfTwoCardsAtTop(Game game);


// If the last player played a card with value "D" and declared a color, 
// the following function returns that declared color as the current color. 
// For all other cases, the function returns the color of the card at 
// the top of the discard pile. 
int getCurrentColor(Game game);

// The function returns the player who played the previous turn.
// For example, if player 1 plays "A" in turn 12, player 3 will 
// play next turn 13. During turn 13, the following function will 
// return 1 (for player 1 who played the previous turn 12).
// If the current turn is the first turn (turn 0), the function should 
// return -1.
int getPreviousTurnPlayer(Game game);


// Get the turn number that played the card that is on the top of 
// the discard pile.
int getTopDiscardTurnNumber(Game game);

// Get the number of "active" cards with value "2" at the top of the 
// discard pile. 
// For example, say in turn-5 player-1 plays "2", as a result of this, 
// the function should return 1. Let's say in turn-6 player-2 
// draws two cards and is unable to play a card. 
// At the start of turn-7, the function should return zero.
// If the value of the top of the discard pile is not "2", the function
// should return zero.
int getActiveDrawTwos(Game game);


// Get the number of cards in the current player's hand.
int handCardCount(Game game);

// View a card from the current player's own hand.
//
// The player should not need to free() this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.

// If no such card exists, the function returns NULL.
Card handCard(Game game, int card);

// Check if a given move is valid.
//
// If the last player played a 2 (DRAW_TWO),
// the next player must either play a 2
// or draw 2 cards.

// If the last player has NOT played a 2, 
// the player must play a card that is either a ZERO
// or that has the same color, value, or suit as the card on the top
// of the discard pile (adjust color for a case where the 
//   previous player has played a "D").
//
// If the player plays an "A", the next player's turn is skipped.
// If the player plays a "D", they must also state which color
// the next player's discarded card should be.
//
// A player can only play cards from their hand.
// A player may choose to draw a card instead of discarding a card.
// A player must draw a card if they are unable to discard a card.
//
// This check should verify that:
// * If a card is played, that card is from the player's hand, and matches 
//   the top card on the discard pile (adjust color for a case where the 
//   previous player has played a "D"), AND the player has not already
//   played a card during their turn.
// * If a DRAW_TWO was played, the player drew the appropriate number of cards.
// * If the player draws a card, either they have not yet drawn a card
//   during this turn OR a DRAW_TWO was played in the previous turn,
//   AND the player has not yet drawn the required number of cards.
//
// * The player has either played at least one card, or drawn at least
//   one card, before playing the END_TURN action.   

int isValidMove(Game game, playerMove move);


// ---------------------------------------------------------
// The following functions must NOT be called by a player
// You can use the following functions to test your Game ADT 
// ---------------------------------------------------------

// Play the given action for the current player
//
// If the player makes the END_TURN move, their turn ends,
// and it becomes the turn of the next player.
//
// This should _not_ be called by the player AI.
void playMove(Game game, playerMove move);

// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner(Game game);


// Get nth card from the deck, n=0 means top card from deck
// If no such card exists, the function returns NULL.
Card getDeckCard (Game game, int n);

// Get nth card from the discard pile, n=0 means top card from discard pile
// If no such card exists, the function returns NULL.
Card getDiscardPileCard (Game game, int n);

// Get nth card from the hand of a player p.
// If no such card exists, the function returns NULL.
Card getHandCard (Game game, int player, int n);

// Get the number of cards in a given player's hand.
int playerCardCount(Game game, int player);

// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// The number of points is always equal to
// the sum of the values for all the cards in a player's hand.
// This should return this number of points at any point in the game.
int playerPoints(Game game, int player);

void printGa(


#endif // GAME_H

