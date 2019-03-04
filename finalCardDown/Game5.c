/*
Notes: Adding (and removing) cards to a deck
There are 4 decks or 4 variables that refer to a deck:
1. startDeck
2. drawPile
3. discardPile
4. hand[]

1. Since startDeck is never modified, there is no need to add
   or remove cards from this deck. Except when the discardPile is
   turned over. (discussed in discardPile section)
   
2. drawPile: Instead of making a separate linked list for
   drawPile, it is referenced using the startDeck. To add (which
   is not done during the game) or remove cards from the deck, one
   only needs to change the startIndex (and startCard if used).
   Thus, the add and remove functions are not called
   
3. discardPile: Cards are added to the front of the list using
   function addCardFront(). Cards are not removed from this linked
   list except when the discardPile is turned over to make the drawPile.
   In this case the startDeck is changed to the discardPile (reversed).
   Thus, it does not affect the functionality of the drawPile (Since the
   drawPile is empty when the discardPile is overturned)
   
   
4. hand[]: Since the order of cards in any players' hand does not
   matter, cards are added to the front of the linked list. 
   
Conclusion: Three functions are needed to add and remove cards
1. addCardFront()
2. removeHandCard()
3. removeDeckCard()
*/


// COULD IMPLEMENT newCard(Card card) function to malloc the first node

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    int startIndex;
    int deckSize;
    Deck startDeck;
    Deck drawPile;
    Deck discardPile;
    int currentPlayer;
    int currentTurn;
    int playerActions;
    int activeDrawTwos;
    value nextValue;
    color nextColor;
    suit nextSuit;
    int topDiscardTurnNumber;
}game;

static Card getCard(Deck pile, int n);
static int distribute(Game game, int totalPlayers, int handSize);
static Deck createDeck(int deckSize, value values[], color colors[], suit suits[]);
static Deck copyDeck(Deck head, int start, int end);
static int isValidCard(Card a, Card check);
static Deck addCardFront(Deck head, Card card);
//static Deck addCardBack(Deck head, Card card);
static void removeDeckCard(Game game);
static Deck removeHandCard(Deck head, Card card);
static Card getPointerToCard(Deck head, Card card);
static void destroyList(Deck deck);
static Deck addCardToHand(Deck head, Card card)


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
    game -> discardPile = malloc (sizeof(struct _deck));
    game -> discardPile -> card = newCard(cardValue(current -> card), cardColor(current -> card), cardSuit(current -> card));
    game -> discardPile -> nextCard = NULL;

    // The rest is part of the drawPile
    game -> drawPile = current -> nextCard;
    //game -> drawPile = copyDeck(game -> startDeck, distributedCards, deckSize);

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
    
    game -> startIndex = distributedCards;
    game -> currentPlayer = 0;
    game -> currentTurn = 0;
    game -> playerActions = 0;
    game -> topDiscardTurnNumber = -1;

    return game;
}


// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
void destroyGame(Game game)
{
    // Destroy each linkedList
	
	// destroying the startDeck also destroys the drawPile
    destroyList(game -> startDeck);
    
    destroyList(game -> discardPile);
    
    int i = 0;
    while (i < NUM_PLAYERS)
    {
    	destroyList(game -> hand[i]);
    	i++;
    }

    // Destroy the game
    free(game);
    game = NULL;
}

// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.
int numCards(Game game)
{
    assert(0 == 1);
    return 0;
}

// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit(Game game, suit suit)
{
    assert(0 == 1);
    return 0;
}

// Get the number of cards in the initial deck of a particular color.
int numOfColor(Game game, color color)
{
    assert(0 == 1);
    return 0;
}

// Get the number of cards in the initial deck of a particular value.
int numOfValue(Game game, value value)
{
    assert(0 == 1);
    return 0;
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
    return game -> topDiscardTurnNumber;
}

// Get the number of consecutive cards with value "2" at the top of the 
// discard pile.
// If the value of the top of the discard pile is not "2", the function will 
// return zero. 
int getNumberOfTwoCardsAtTop(Game game)
{
    assert(0 == 1);
    return 0;
}


// If the last player played a card with value "D" and declared a color, 
// the following function returns that declared color as the current color. 
// For all other cases, the function returns the color of the card at 
// the top of the discard pile. 
int getCurrentColor(Game game)
{
    assert(0 == 1);
    return 0;
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
    assert(0 == 1);
    return 0;
}

// View a card from the current player's own hand.
//
// The player should not need to free() this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.

// If no such card exists, the function returns NULL.
Card handCard(Game game, int card)
{
    assert(0 == 1);
    return NULL;
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
	Deck d = game -> discardPile;
	return getCard(d, n);
}

// Get nth card from the hand of a player p.
// If no such card exists, the function returns NULL.
Card getHandCard (Game game, int player, int n)
{
    //printf("%d   %d\n", player, n);
    Deck d = game -> hand[player];
    //printf("%p\n", d);
	return getCard(d, n);
}


// Play the given action for the current player
//
// If the player makes the END_TURN move, their turn ends,
// and it becomes the turn of the next player.
//
// This should _not_ be called by the player AI.

void playMove(Game game, playerMove move)
{
	if (move.action == DRAW_CARD)
	{
		// Add first card in drawPile to current player's hand
		game -> hand[currentPlayer(game)] = addCardFront(game -> hand[currentPlayer(game)], game -> drawPile -> card);

		// Remove first card from the drawPile
		removeDeckCard(game);
	}
	else if (move.action == PLAY_CARD)
	{
		//Get the pointer to the card first
		Card removed = getPointerToCard(game -> hand[currentPlayer(game)], move.card);
		
		
		// Then remove the card and update the list
	    // Remove card from players hand
		game -> hand[currentPlayer(game)] = removeHandCard(game -> hand[currentPlayer(game)], move.card);
		
		// Add card to discardPile
		game -> discardPile = addCardFront(game -> discardPile, removed);
		
		game -> topDiscardTurnNumber = game -> currentTurn;
		
		// Special Cases
		if (cardValue(move.card) == DRAW_TWO)
		{
			game -> activeDrawTwos++;
		}
		else if(cardValue(move.card) == A)
		{
			game -> currentPlayer+=2;
		}
		else if(cardValue(move.card) == D)
		{
			game -> nextColor = move.nextColor;
		}
		else
		{		    
        	game -> nextColor = cardColor(game ->discardPile -> card);
        	game -> nextValue = cardValue(game ->discardPile -> card);
        	game -> nextSuit = cardSuit(game ->discardPile -> card);
        }

		// Update the game state
		game -> playerActions++;
		
	}
	else
	{
	    game -> currentPlayer++;
	    if (game -> currentPlayer > 3)
	    {
	        game -> currentPlayer = 0;
	    }
	    game -> currentTurn++;
	    game -> playerActions = 0;
	    
	}
	
	


}

// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner(Game game)
{
    assert(0 == 1);
    return 0;
}



// Get the number of cards in a given player's hand.
int playerCardCount(Game game, int player)
{
    assert(0 == 1);
    return 0;
}

// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// The number of points is always equal to
// the sum of the values for all the cards in a player's hand.
// This should return this number of points at any point in the game.
int playerPoints(Game game, int player)
{
    assert(0 == 1);
    return 0;
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
    
    //printf("%p\n", req);
    return req;
}

static int distribute(Game game, int totalPlayers, int handSize)
{
		
	// Extract one card from the startDeck and give it to
	// each player.
	// There is no need to remove the card from the startDeck
	
	// Total cards to be distributed
	int totalCards = totalPlayers * handSize;
	
	// Copy of the first card of the startDeck
	//Deck current = game -> startDeck;
	
	// Keep track of which player to distribute to
	// Start distributing from the first player (=0)
	int player = 0;
	
	value values[handSize];
	color colors[handSize];
	suit suits[handSize];
	
	
	// Distribute the cards. Keep track of the number of cards distributed
	while (player < totalPlayers)
	{
	    Deck current = game -> startDeck;
	    int k = 0;
	    while (k < player)
	    {
	        current = current -> nextCard;
	        k++;
	    }
	    int i = 0;
    	while (i < handSize)
	    {
	        values[i] = cardValue(current -> card);
	        colors[i] = cardColor(current -> card);
	        suits[i] = cardSuit(current -> card);
	        
	        int j = 0;
	        while (j < totalPlayers)
	        {
	            current = current -> nextCard;
	            j++;
	        }
	        
	        i++;
	    }
	    game -> hand[player] = createDeck(handSize, values, colors, suits);
	    player++;
	}
	return totalCards;
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


static int isValidCard(Card a, Card check)
{
	int isValid = 0;
	if (a == NULL || check == NULL)
	{
	    isValid = 0;
	}
	else
	{
        isValid = (cardSuit(a) == cardSuit(check) || cardValue(a) == cardValue(check) || cardColor(a) == cardColor(check));
    }
    
    return isValid;
}

static Deck addCardFront(Deck head, Card card)
{
    // Initialize
    Deck new = malloc(sizeof(struct _deck));
    new -> card = card;
    
    new -> nextCard = head;
    head = new;
    
    return head;
}
static void removeDeckCard(Game game)
{
    // Cannot remove a card from an empty deck
	if(game -> drawPile != NULL)
	{
	    game -> startIndex ++;
    }
}

// Find the card and remove it
static Deck removeHandCard(Deck head, Card card)
{
    int check = 0;
    Deck current = head;
    //printf("%p\n", current);
    // Cannot remove a card from an empty hand
    if (head != NULL)
    {
        // if the card is the first one,
        if (isValidCard(head -> card, card))
        {
            printf("Currently removing the first card\n");
            // Card found
            check = 1;
            
            // Remove from list
            Deck temp = head;
            head = head -> nextCard;
            
            //printf("First Card: %p\n Required Card: %p\n", temp -> card, card);
            
            // free the components
            //destroyCard(temp -> card);
            free(temp);
            temp = NULL;
            
        }
        else
        {
            while (check == 0 && current -> nextCard != NULL)
            {
                // Temporary copy of next card
                Deck temp = current -> nextCard;
                
                // If card to be removed is found
                if (isValidCard(temp -> card, card))
                {
                    // Card found
                    check = 1;
                    
                    // Remove from list
                    current -> nextCard = temp -> nextCard;
                    
                    // Free components
                    //destroyCard(temp -> card);
                    free(temp);
                    temp = NULL;
                }
                
                // Next card
                current = current -> nextCard;
            }
        }    
    }
    return head;
}

static Card getPointerToCard(Deck head, Card card)
{
    Card removed = NULL;
    Deck current = head;
    int check = 0;
    // Card does not exist in an empty hand
    if (head != NULL)
    {
        while (check == 0 && current -> card != NULL)
        {
            if (isValidCard(current -> card, card))
            {
                check = 1;
                removed = current -> card;
            }
            current = current -> nextCard;
        }
    }
    return removed;
}

static void destroyList(Deck head)
{
	Deck current = head;
	while (current != NULL)
	{
		Deck temp = current;
		current = current -> nextCard;
		free(temp);
	}
}


