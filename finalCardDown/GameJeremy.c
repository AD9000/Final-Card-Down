// Jeremy John George (zID)

#include <stdlib.h>
#include <stdio.h>

#include "Game.h"
#include "Card.h"
#include "Card.c"


#define NUM_VALUES 16
#define NUM_COLORS 5
#define NUM_SUITS 5

#define HAND_SIZE 7

typedef struct _node *Node;
// Struct for node:- used for the draw and discard piles
typedef struct _node{
    // First card in the node
    Card card;
    struct _node *next;
}node;

// Struct for game - ties the whole game together
typedef struct _game {

    node *hand[NUM_PLAYERS];
    node *startDeck;
    node *drawPile;
    node *discardPile;
    int currentPlayer;
    int currentTurn;
    int playerActions;
    int activeDrawTwos;
}game;

static node *newDeck(int deckSize, value values[], color colors[], suit suits[]);
static node *newNode(value v, color c, suit s);
static Card getCard(Node pile, int n);

static void printCardByComponents(value v, color c, suit s);
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
    // Create memory for game
    Game game = malloc(sizeof(struct _game));

    //Call function newDeck to create a new Deck with the specified parameters (deckSize, Values, Colors and Suits)
    game -> startDeck = newDeck(deckSize, values, colors, suits);
    int k = 0;
    node *m = game -> startDeck;
    
    while (k < 50)
    {
        printCardByComponents(cardValue(m -> card), cardColor(m -> card), cardSuit(m -> card));
        m = m -> next;
        k++;
    }
    //Create a node for each player.
       
       //Create an array of size 4 to store the hands of all the players    
    
       int i = 0; 
    int j = 0;
    node *n = game -> startDeck;
    int count = 0;
    
    // Major mistake 1 (or mistake 5)
    node *current[4];
    while (i < NUM_PLAYERS)
    {
        current[i] = newNode(values[count], colors[count], suits[count]);
        game -> hand[i] = current[i];
        n = n -> next;
        count++;
        i++;
    }
    i = 0;
    
    while (i < HAND_SIZE - 1) {
        // Stupid mistake 3
        j = 0;
        while (j < NUM_PLAYERS) {
         
             node *temp = newNode(values[count], colors[count], suits[count]);
             current[j] -> next = temp;
             current[j] = current[j] -> next;
             // skipping first 27 cards
             n = n -> next;
             j++;
             count++;
       }
       i++;
    }
    
    // The 29th card is the first card in discardPile
    game -> discardPile = malloc(sizeof(struct _node));
    game -> discardPile -> card = newCard(cardValue(n -> card), cardColor(n -> card), cardSuit(n -> card));
    game -> discardPile -> next = NULL;
    
   
    // Remaining is the drawPile
    int decks = deckSize - HAND_SIZE*NUM_PLAYERS - 1;
    
    value values2[decks];
    color colors2[decks];
    suit suits2[decks];
    
    i = 0;
    while (i < decks) {
       values2[i] = values[i + HAND_SIZE*NUM_PLAYERS + 1];
       colors2[i] = colors[i + HAND_SIZE*NUM_PLAYERS + 1];
       suits2[i] = suits[i + HAND_SIZE*NUM_PLAYERS + 1];
       i++;
    }

    game -> drawPile = newDeck(decks, values2, colors2, suits2);
    //Initialize all the fields in the game struct
    game -> currentPlayer = 0;
    game -> currentTurn = 0;
    game -> playerActions = 0;
    game -> activeDrawTwos = 0;
    
    game -> discardPile -> next = NULL;

    return game;
}

// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
void destroyGame(Game game) {
    free(game);
    game = NULL;
}

// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.
int numCards(Game game) {

    int countCards = 0;
    node *curr = game -> startDeck;
    while (curr != NULL) {
    
        countCards++;
        curr = curr -> next;
    
    }

    return countCards;
}

// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit(Game game, suit suit){

    int countSuit = 0;
    node *curr = game -> startDeck;

    while (curr != NULL) {
        if (cardSuit(curr-> card) == suit) {
        
            countSuit++;
        }

        curr = curr -> next;
        
    }
    
    return countSuit;
}

// Get the number of cards in the initial deck of a particular color.
int numOfColor(Game game, color color){
    return 0;
}

// Get the number of cards in the initial deck of a particular value.
int numOfValue(Game game, value value){
    return 0;
}

// Get the number of the player whose turn it is.
int currentPlayer(Game game){
    return 0;
}

// Get the current turn number.
//
// The turn number increases after a player ends their turn.
// The turn number should start at 0 once the game has started.
int currentTurn(Game game){
    return 0;
}


// Get the card that is on the top of the discard pile.
Card topDiscard(Game game){
    return NULL;
}

// This function returns the number of turns that have occurred in the
// game so far including the current turn.
// (i.e. on turn 0 of the game, this should return 1, as there has been
//       1 turn so far including the current turn.)
int numTurns(Game game){

    return game -> currentTurn + 1;
    
}

// Get the number of moves (actions) that happened on the current turn.
//
// A turn may consist of multiple moves (actions) such as drawing cards,
// playing cards, and ending the turn.
int currentTurnMoves(Game game){
    return 0;
}


// Get the number of consecutive cards with value "2" at the top of the 
// discard pile.
// If the value of the top of the discard pile is not "2", the function will 
// return zero. 
int getNumberOfTwoCardsAtTop(Game game){
    return 0;
}


// If the last player played a card with value "D" and declared a color, 
// the following function returns that declared color as the current color. 
// For all other cases, the function returns the color of the card at 
// the top of the discard pile. 
int getCurrentColor(Game game){
    return 0;
}

// The function returns the player who played the previous turn.
// For example, if player 1 plays "A" in turn 12, player 3 will 
// play next turn 13. During turn 13, the following function will 
// return 1 (for player 1 who played the previous turn 12).
// If the current turn is the first turn (turn 0), the function should 
// return -1.
int getPreviousTurnPlayer(Game game){

    int playerNo = 0;
    if(currentTurn(game) == 0)
    {
        playerNo = -1;
    }
    else
    {
        node *curr = game -> discardPile;
        playerNo = currentPlayer(game);

        if (cardValue(curr -> card) == A)
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
int getTopDiscardTurnNumber(Game game){
    
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
int getActiveDrawTwos(Game game){
    return 0;
}


// Get the number of cards in the current player's hand.
int handCardCount(Game game){
    return 0;
}

// View a card from the current player's own hand.
//
// The player should not need to free() this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.

// If no such card exists, the function returns NULL.
Card handCard(Game game, int card){
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

int isValidMove(Game game, playerMove move){
    return 0;
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
void playMove(Game game, playerMove move){
}

// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner(Game game){
    return 0;
}


// Get nth card from the deck, n=0 means top card from deck
// If no such card exists, the function returns NULL.
Card getDeckCard (Game game, int a){
    
    node *n = game -> drawPile;
       return getCard(n, a);

    
}

// Get nth card from the discard pile, n=0 means top card from discard pile
// If no such card exists, the function returns NULL.
Card getDiscardPileCard (Game game, int a){

    // Then we needed to implement this function
    // Mistake 2
    
    node *n = game -> discardPile;
       return getCard(n, a);
}

// Get nth card from the hand of a player p.
// If no such card exists, the function returns NULL.
Card getHandCard (Game game, int player, int a){
    // Mistake 4
    node *n = game -> hand[player];
       return getCard(n, a);
}

// Get the number of cards in a given player's hand.
int playerCardCount(Game game, int player){
    return 0;
}

// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// The number of points is always equal to
// the sum of the values for all the cards in a player's hand.
// This should return this number of points at any point in the game.
int playerPoints(Game game, int player){
    return 0;
}

static node *newNode(value v, color c, suit s) {
    node *head = malloc(sizeof(struct _node));
    head -> card = newCard(v, c, s);
    head -> next = NULL;
    
    return head;
}

static node *newDeck(int deckSize, value values[], color colors[], suit suits[]) {
    node *head = malloc(sizeof(struct _node));
    head -> card = newCard(values[0], colors[0], suits[0]);
    head -> next = NULL;
    
    node *curr = head;
    // UN-FUCKING-BELIEVABLE MISTAKE DUDE!!!! (we had i = 0) (first one)
    int i = 1;
    while (i < deckSize) {
       node *curr2 = malloc(sizeof(struct _node));
       curr2 -> card = newCard(values[i], colors[i], suits[i]);
       curr2 -> next = NULL;
       
       curr -> next = curr2;
       curr = curr -> next;
       
       i++;
    }

    return head;
}
static Card getCard(node *pile, int n) {
       node *curr = pile;
       Card c = NULL;
       int i = 0;
       while (i < n && curr != NULL)
       {
           curr = curr -> next;
           i++;
       }
       
       if (curr != NULL)
       {
        c = curr -> card;
    }
    
    return c;
}

static void printCardByComponents(value v, color c, suit s){
    char* valueStrings[NUM_VALUES] = {
        "ZERO", "ONE", "DRAW_TWO", "THREE", "FOUR",
        "FIVE", "SIX", "SEVEN", "EIGHT", "NINE",
        "A", "B", "C", "D", "E", "F"
    };

    char* colorStrings[NUM_COLORS] = {
        "RED", "BLUE", "GREEN", "YELLOW", "PURPLE"
    };

    char* suitStrings[NUM_SUITS] = {
        "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "QUESTIONS"
    };

    printf("%s %s of %s\n", colorStrings[c], valueStrings[v], suitStrings[s]);
}

