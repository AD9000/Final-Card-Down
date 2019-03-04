#include <stdio.h>
#include <stdlib.h>

#include "card.c"
typedef struct _deck *Deck;
// Struct for deck:- used for the draw and discard piles
typedef struct _deck{
    // First card in the deck
    Card card;
    struct _deck *nextCard;
}deck;
typedef struct _game *Game;
typedef struct _game {

    //playerMove players[NUM_PLAYERS];
    //Deck hand[NUM_PLAYERS];
    Deck startDeck;
    Deck drawPile;
    Deck discardPile;
    int currentPlayer;
    int currentTurn;
    int playerActions;
    int activeDrawTwos;
    int winner;
}game;

int main (void)
{
    Game g = malloc (sizeof(struct _game));
    printf("%p\n", g -> drawPile);
    printf("%p\n", g -> drawPile -> card);
    free(g);
}
    
