//Author: Atharv Damle (z5232949)

/*
// Technical issues/details: Notes to self

// Note: To remove an element from the set the check variable should be first
// This is because the next node has been freed.
// Thus one must exit the loop and not access the '-> next' of the next value

// Note 2: To add element to a null set, one must use 's -> start' and not
// 'current' as the command 'current = temp' only changes the value of the
// pointer and not the list which 's -> start' points to
*/
/*
Notes for self:
1. Add functionality to check for various NULL conditions
    eg: calling various getter functions on a game that has not been "created"
2. Check if decks have to be stacks or queues or if it was mentioned at all.
3. Discard pile is a stack thus remember to add the cards from the front
4. Divide functionalities into helper functions (preferably)
5. DISTRIBUTING CARD FUNCTIONALITY IS WRONG!!
*/

#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "Card.h"
#include "Card.c"



#define HAND_SIZE 7
#define ANY_COLOR -10
typedef struct _deck *Deck;
// Struct for deck:- used for the draw and discard piles
typedef struct _deck{
    // First card in the deck
    Card card;
    struct _deck *nextCard;
}deck;

// Struct for game - ties the whole game together
typedef struct _game {

    playerMove players[NUM_PLAYERS];
    Deck hand[NUM_PLAYERS];

    // deckSize refers to the size of the drawPile
    int deckSize;
    Deck startDeck;
    // Remove drawPile and use only one list for the deck
    // Store using startIndex
    Deck drawPile;
    Deck discardPile;
    int currentPlayer;
    int currentTurn;
    int playerActions;
    int activeDrawTwos;
    value nextValue;
    color nextColor;
    suit nextSuit;
}game;

static Card getCard(Deck pile, int n);
//static void addCardFront(Deck pile, Card card);
//static Deck newDeck();
static void removeFirstCard(Deck pile);
static int isValidCard(Card a, Card check);
static int distribute(Game game, int totalPlayers, int handSize);
static Deck createDeck(int deckSize, value values[], color colors[], suit suits[]);
static Deck copyDeck(Deck head, int start, int end);
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
Game newGame(int deckSize, value values[], color colors[], suit suits[])
{
    // Create game struct
    // Which also creates space for all the integer variables and pointers
    Game game = malloc(sizeof(struct _game));

    // Create a list - startDeck
    game -> startDeck = createDeck(deckSize, values, colors, suits);

    // Distribute cards to players
    int distributedCards = distribute(game, NUM_PLAYERS, HAND_SIZE);

    // Use startDeck to create the drawPile
    // Size of the drawPile is the length of startDeck - no.of cards distributed - 1
    // (-1 because the first card is the start of the discardPile)
    game -> deckSize = deckSize - distributedCards - 1;
    Deck current = game -> startDeck;

    // Skip the cards that have been distributed
    int i = 0;
    while (i < distributedCards)
    {
    	current = current -> nextCard;
    	i++;
    }

    // The next card of the startDeck is the firstCard of the discardPile
    game -> discardPile = current;

    // The rest is part of the drawPile
    game -> drawPile = copyDeck(game -> startDeck, distributedCards, deckSize);

    // The value of the first card dictates the starting parameters in the game
    game -> nextColor = cardColor(current -> card);
    game -> nextValue = cardValue(current -> card);
    game -> nextSuit = cardSuit(current -> card);

    // Handle special cases when the first card of the discardPile is
    // is a special card
    if (game -> nextValue == D)
    {
    	game -> nextColor = ANY_COLOR;
    }
    
    game -> currentPlayer = 0;
    game -> currentTurn = 0;
    game -> playerActions = 0;

    return game;
}

// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
void destroyGame(Game game)
{
	//
    Deck current = game -> startDeck;

    while (current != NULL)
    {
        Deck temp = current;
        current = current -> nextCard;
        free(temp);
    }

    free(game);
    game = NULL;

}

// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.
int numCards(Game game)
{
    int count = 0;
    Deck current = game -> startDeck;
    while (current != NULL)
    {
        count++;
        current = current -> nextCard;
    }

    return count;
}

// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit(Game game, suit suit)
{
    int count = 0;
    Deck current = game -> startDeck;

    while (current != NULL)
    {
        if (cardValue(current-> card) == suit)
        {
            count ++;
        }

        current = current -> nextCard;
    }

    return count;
}

// Get the number of cards in the initial deck of a particular color.
int numOfColor(Game game, color color)
{
    int count = 0;
    Deck current = game -> startDeck;

    while (current != NULL)
    {
        if (cardColor(current -> card) == color)
        {
            count ++;
        }

        current = current -> nextCard;
    }

    return count;
}

// Get the number of cards in the initial deck of a particular value.
int numOfValue(Game game, value value)
{
    int count = 0;
    Deck current = game -> startDeck;

    while (current != NULL)
    {
        if (cardValue(current -> card) == value)
        {
            count ++;
        }

        current = current -> nextCard;
    }

    return count;
}

// Get the number of the player whose turn it is.
int currentPlayer(Game game)
{
    return game -> currentPlayer;
}
// Get the current turn number.
//
// The turn number increases after a player ends their turn.
// The turn number should start at 0 once the game has started.
int currentTurn(Game game)
{
    return game -> currentTurn;
}


// Get the card that is on the top of the discard pile.
Card topDiscard(Game game)
{
    return game -> discardPile -> card;
}

// This function returns the number of turns that have occurred in the
// game so far including the current turn.
// (i.e. on turn 0 of the game, this should return 1, as there has been
//       1 turn so far including the current turn.)
int numTurns(Game game)
{
    return game -> currentTurn + 1;
}

// Get the number of moves (actions) that happened on the current turn.
//
// A turn may consist of multiple moves (actions) such as drawing cards,
// playing cards, and ending the turn.
int currentTurnMoves(Game game)
{
    return game -> playerActions;
}


// Get the number of consecutive cards with value "2" at the top of the
// discard pile.
// If the value of the top of the discard pile is not "2", the function will
// return zero.
int getNumberOfTwoCardsAtTop(Game game)
{
    deck *current = game -> discardPile;
    int count = 0;

    while (current != NULL && cardValue(current -> card) == DRAW_TWO)
    {
        count++;
        current = current -> nextCard;
    }

    return count;
}


// If the last player played a card with value "D" and declared a color,
// the following function returns that declared color as the current color.
// For all other cases, the function returns the color of the card at
// the top of the discard pile.
int getCurrentColor(Game game)
{
    deck *current = game -> discardPile;
    int currentColor = 0;

    if (cardValue(current -> card) == D)
    {
        currentColor = game -> players[currentPlayer(game)].nextColor;
    }
    else
    {
        currentColor = cardColor(current -> card);
    }

    return currentColor;
}

// The function returns the player who played the previous turn.
// For example, if player 1 plays "A" in turn 12, player 3 will
// play next turn 13. During turn 13, the following function will
// return 1 (for player 1 who played the previous turn 12).
// If the current turn is the first turn (turn 0), the function should
// return -1.
int getPreviousTurnPlayer(Game game)
{
    int playerNo = 0;
    if(currentTurn(game) == 0)
    {
        playerNo = -1;
    }
    else
    {
        deck *current = game -> discardPile;
        playerNo = currentPlayer(game);

        if (cardValue(current -> card) == A)
        {
            playerNo -= 2;
        }
        else
        {
            playerNo -= 1;
        }

        if (playerNo < 0)
        {
            playerNo = 4 - playerNo;
        }
    }

    return playerNo;
}


// Get the turn number that played the card that is on the top of
// the discard pile.
int getTopDiscardTurnNumber(Game game)
{
    return game -> currentTurn - 1;
}

// Get the number of "active" cards with value "2" at the top of the
// discard pile.
// For example, say in turn-5 player-1 plays "2", as a result of this,
// the function should return 1. Let's say in turn-6 player-2
// draws two cards and is unable to play a card.
// At the start of turn-7, the function should return zero.
// If the value of the top of the discard pile is not "2", the function
// should return zero.
int getActiveDrawTwos(Game game)
{
    return game -> activeDrawTwos;
}


// Get the number of cards in the current player's hand.
int handCardCount(Game game)
{
    int count = 0;
    deck *current = game -> hand[currentPlayer(game)];
    while (current != NULL)
    {
        count++;
        current = current -> nextCard;
    }

    return count;
}


// View a card from the current player's own hand.
//
// The player should not need to free() this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.

// If no such card exists, the function returns NULL.
Card handCard(Game game, int card)
{
	Deck d = game -> hand[currentPlayer(game)];
	return getCard(d, card);
}

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

int isValidMove(Game game, playerMove move)
{
	/*
	 * Conditions for validity
	 * If the last card was:
	 * 1. Draw two: Valid only if the card to be played is draw two, or action is to draw two (or more) cards
	 * 2. A (Skip): Same color
	 */
	// Move is valid
	int check = 1;
	Card topCard = getDiscardPileCard(game, 0);
	// If the top card is zero, validity of the move depends on the next card
	// in the discard pile
	if(cardValue(topCard) == ZERO)
	{
		int i = 1;
		while (cardValue(topCard) == ZERO)
		{
			topCard = getDiscardPileCard(game, i);
		}
	}
	// If top card is a draw two

	// If the player wants to end his turn, he must make at least one move.
	/*
	if (move.action == END_TURN)
	{
		if (currentTurnMoves == 0)
		{
			check = 0;
		}
		else
		{
			check = 1;
		}
	*/
	// Player can end a turn only if he/she has played at least one move
	// ie Draw card if he/she has no valid card or played a valid card
	if (move.action == END_TURN)
	{
		if (currentTurnMoves(game) == 0)
		{
			// Invalid move
			check = 0;
		}
		// else the move is valid if: the player has drawn 2*Number of active draw twos
		// i.e. there are no active draw twos left
		else if (getActiveDrawTwos(game) != 0)
		{
			// Not valid
			check = 0;
		}
	}
	else if (move.action == DRAW_CARD)
	{
		// Move is valid
	}
	else if (cardValue(move.card) == ZERO)
	{
		// Move is valid
	}
	else // Player is trying to play a card
	{
		// If topCard
		if (cardValue(topCard) == DRAW_TWO)

		{
			if (cardValue(move.card) != DRAW_TWO)
			{
				// Invalid
				check = 0;
			}
			// Otherwise move is valid

		}
		// Top card is something else
		else if (cardValue(topCard) == D)
		{
			// Valid only if the next card has the value of next color
			// Since there is no value to the card on top
			// And thus has no suit either

			if (cardColor(move.card) != getCurrentColor(game))
			{
				// Invalid
				check = 0;
			}
		}
		else if (!isValidCard(topCard, move.card))
		{
			check = 0;
		}
	}
	return check;
}


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
/*
void playMove(Game game, playerMove move)
{
	if (move.action == DRAW_CARD)
	{
		// Add first card in drawPile to current player's hand
		game -> hand[currentPlayer(game)] -> nextCard = game -> drawPile;

		// Remove first card from the drawPile
		removeFirstCard(game -> drawPile);
	}
	else if (move.action == PLAY_CARD)
	{
		// Special Cases
		if (cardValue(move.card) == DRAW_TWO)
		{
			game -> activeDrawTwos++;
		}
		else if(cardValue(move.card) == A)
		{
			game -> currentPlayer++;
		}
		else if(cardValue(move.card) == D)
		{
			game -> nextColor = move.nextColor;
		}

		// Add card to discardPile
		addCardFront(game -> discardPile, move.card);

		// Remove card from players hand
		removeFirstCard(game -> hand[currentPlayer(game)]);
	}

	game -> playerActions++;


}
*/
// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner(Game game)
{
	// Assume game is still in progress
	int winner = NOT_FINISHED;

	// To check if there is a winner and who it is
	int check = 0;
	int i = 0;

	// Find if there is a winner
	// If yes, who is it?
	while (i < NUM_PLAYERS && check == 0)
	{
		// If there are no cards in the players hand,
		// he/she has won the game
		if (playerCardCount(game, i) == 0)
		{
			// Winner found. The ith player is the winner
			winner = i;
			check = 1;
		}

		// the ith player is not the winner.
		// Check next one
		i++;
	}

	// If there are no cards in the drawPile, the
	// game ends with no winner
	if (getDeckCard(game, 0) == NULL)
	{
		winner = NO_WINNER;
	}

	return winner;

}


// Get nth card from the deck, n=0 means top card from deck
// If no such card exists, the function returns NULL.
Card getDeckCard (Game game, int n)
{
	Deck d = game -> drawPile;
	return getCard(d, n);
}

// Get nth card from the discard pile, n=0 means top card from discard pile
// If no such card exists, the function returns NULL.
Card getDiscardPileCard (Game game, int n)
{
	Deck discP = game -> discardPile;
	return getCard(discP, n);
}

// Get nth card from the hand of a player p.
// If no such card exists, the function returns NULL.
Card getHandCard (Game game, int player, int n)
{
    Deck handP = game -> hand[player];
    return getCard(handP, n);
}
// Get the number of cards in a given player's hand.
int playerCardCount(Game game, int player)
{
    int count = 0;
    Deck current = game -> hand[player];
    while (current != NULL)
    {
        count++;
        current = current -> nextCard;
    }

    return count;
}
// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// The number of points is always equal to
// the sum of the values for all the cards in a player's hand.
// This should return this number of points at any point in the game.
int playerPoints(Game game, int player)
{
    int count = 0;
    deck *current = game -> hand[player];
    while (current != NULL)
    {
        count += cardValue(current -> card);
        current = current -> nextCard;
    }

    return count;
}

//-------------------------------------------------
// Helper functions
//-------------------------------------------------

// Finds and returns card from deck
static Card getCard(Deck head, int n)
{
    Deck current = head;
    Card req = NULL;
    int i = 0;

    while (i < n && current != NULL)
    {
        current = current -> nextCard;
        i++;
    }

    if (current != NULL)
    {
        //printf("Current exists");
    	req = current -> card;
    }
    else
    {
        //printf("Current is null");
        req = NULL;
    }

    return req;
}

static int isValidCard(Card a, Card check)
{
	return (cardSuit(a) == cardSuit(check) || cardValue(a) == cardValue(check) || cardColor(a) == cardColor(check));
}

// Removes first card from a deck
static void removeFirstCard(Deck pile)
{
    // Cannot remove a card from an empty pile
	if(pile != NULL)
	{
	    Deck temp = pile;
    	pile = pile -> nextCard;

	    // Free card memory
	    destroyCard(temp -> card);

	    // Free memory for the deck/node storing the card
	    free(temp);
    }
}
/*
// Add card to the front of a deck of cards
static void addCardFront(Deck pile, Card card)
{
	Deck current = newDeck();
	current -> card = card;

	current -> nextCard = pile;
	pile = current;
}
*/
/*
static Deck newDeck(value value, color color, suit suit)
{
	Deck new = malloc(sizeof(struct _deck));
	new -> card = newCard(value, color, suit);
	new -> nextCard = NULL;
	Card current = new -> card;

    if (current -> value == ZERO)
    {
        printf("ZERO\n");
    }
    if (current -> color == RED)
    {
        printf("RED\n");
    }
    if (current -> suit == HEARTS)
    {
        printf("HEARTS\n");
    }
    
	return new;
}
*/
/*
void printGame(Game game)
{
    Deck current = game -> startDeck;

    while (current != NULL)
    {
        printf("%d   %d    %d\n", cardValue(current -> card) , cardColor(current -> card), cardSuit(current -> card));
        Card current1 = game -> startDeck -> card;

        if (current1 -> value == ZERO)
        {
            printf("ZERO\n");
        }
        if (current1 -> color == RED)
        {
            printf("RED\n");
        }
        if (current1 -> suit == HEARTS)
        {
            printf("HEARTS\n");
        }
        current = current -> nextCard;
    }
}
*/
//-------------------------------------------------
// Helper functions
//-------------------------------------------------


static int distribute(Game game, int totalPlayers, int handSize)
{
		
	// Extract one card from the startDeck and give it to
	// each player.
	// There is no need to remove the card from the startDeck
	
	// Total cards to be distributed
	int totalCards = totalPlayers * handSize;
	
	// Copy of the first card of the startDeck
	Deck current = game -> startDeck;
	
	// Keep track of which player to distribute to
	// Start distributing from the first player (=0)
	int player = 0;
	
	// Distribute the cards. Keep track of the number of cards distributed
	int distributedCards = 0;
	while (distributedCards < totalCards)
	{
		// Actually distribute the card
		game -> hand[player] = current;
		
		// Change player
		player++;
		if (player > 3)
		{
			player = 0;
		}
		
		// Next card in deck
		current = current -> nextCard;
		
		distributedCards++;
	}
	return distributedCards;
}

// Create deck is working. (with int data anyway... (check test2 in vnc finalCardDown))
// Creates a deck of size deckSize with cards with values, colors, suits
// according to the arrays supplied
static Deck createDeck(int deckSize, value values[], color colors[], suit suits[])
{
	// Allocate memory for the first element in list
	Deck head = malloc(sizeof(struct _deck));

	// Initialize first card
	head -> card = newCard(values[0], colors[0], suits[0]);
	
	head -> nextCard = NULL;

	// Temporary copy of first node
	Deck temp = head;

	// Make list of deckSize - 1 elements
	int i = 1;
	while (i < deckSize)
	{
		// Allocate memory for a node
		Deck current = malloc(sizeof(struct _deck));

		// Initialize contents of node
		current -> card = newCard(values[i], colors[i], suits[i]);
		current -> nextCard = NULL;

		// Add node to list
		head -> nextCard = current;

		// Go to next node in list
		head = head -> nextCard;

		i++;
	}

	// Restore value of first node
	head = temp;

	// Return first element in the list
	return head;
}

static Deck copyDeck(Deck head, int start, int end)
{
	// Initialize first element
	Deck newDeck = malloc (sizeof(struct _deck));
	Deck newTemp = newDeck;
	newDeck -> nextCard = NULL;
	
	// Traverse deck to be copied
	// Skip cards till start index
	Deck current = head;
	int i = 0;
	while (i < start - 1)
	{
		current = current -> nextCard;
		i++;
	}
	
	// Copy first card
	newDeck -> card = newCard(cardValue(current -> card), cardColor(current -> card), cardSuit(current -> card));
	i++;
	// Copy the deck
	while (i < end)
	{
		Deck temp = malloc(sizeof(struct _deck));
		current = current -> nextCard;
		
		temp -> card = newCard(cardValue(current -> card), cardColor(current -> card), cardSuit(current -> card));
		temp -> nextCard = NULL;
		
		newDeck -> nextCard = temp;
		newDeck = newDeck -> nextCard;
		
		i++;
	}
	
	newDeck = newTemp;
	return newDeck;
}

