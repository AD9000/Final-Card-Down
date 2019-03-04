#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Game.h"
#include "Card.h"

#define STARTING_HAND_SIZE 7
#define NUM_PLAYERS 4

#define NUM_VALUES 16
#define NUM_COLORS 5
#define NUM_SUITS 5

static void check_initial_PlayerHand(Game game, int player, value values[], color colors[], suit suits[]);
static void check_initial_Deck(Game game, int deck_size, value values[], color colors[], suit suits[]);
static void check_initial_DiscardPile(Game game, int deck_size, value values[], color colors[], suit suits[]);

static int cardIsInHand(Game game, int player, int alreadyMatched[], value v, color c, suit s);
static int cardMatchesComponents(Card card, value v, color c, suit s);
static void printCardByComponents(value v, color c, suit s);

static void test1_init_deck_hands(void);

int main (void){

     test1_init_deck_hands();

}

/*  ----------------------------------------
        ... Test functions below ...
*/

static void test1_init_deck_hands(void){
    int deck_size = 50;
    value values[] = {
        ZERO, ONE, DRAW_TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
        A, B, C, D, E, F, ZERO, ONE, DRAW_TWO, THREE,
        FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, A, B, C, D,
        E, F, ZERO, ONE, DRAW_TWO, THREE, FOUR, FIVE, SIX, SEVEN,
        EIGHT, NINE, A, B, C, D, E, F, ZERO, ONE
    };

    color colors[] = {
        RED, RED, RED, RED, RED, RED, RED, RED, RED, RED,
        BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE,
        GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN,
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS
    };

    Game game = newGame(deck_size, values, colors, suits);
    
    //printGame(game);

    printf("Checking if current player is 0\n\n");
    assert(currentPlayer(game) == 0);

    printf("Checking if current turn is 0\n\n");
    assert(currentTurn(game) == 0);

    printf("Checking if number of turns is 1\n\n");
    assert(numTurns(game) == 1);

    printf("Checking if current moves in turn is 0\n\n");
    assert(currentTurnMoves(game) == 0);

    printf("Checking if player from previous turn is -1\n\n");
    assert(getPreviousTurnPlayer(game) == -1);

    printf("Checking if top of discard pile was played in turn -1\n\n");
    assert(getTopDiscardTurnNumber(game) == -1);

    check_initial_Deck(game, deck_size, values, colors, suits);
    printf("\n");

    check_initial_DiscardPile(game, deck_size, values, colors, suits);
    printf("\n");

    check_initial_PlayerHand(game, 0, values, colors, suits);
    printf("\n");

    check_initial_PlayerHand(game, 1, values, colors, suits);
    printf("\n");

    check_initial_PlayerHand(game, 2, values, colors, suits);
    printf("\n");

    check_initial_PlayerHand(game, 3, values, colors, suits);
    printf("\n");


    printf("Trying to destroy the game\n\n");
    destroyGame(game);

    printf("All tests passed, nice work!\n");

}

/*  ----------------------------------------
        ... Helper functions below ...
*/


static void check_initial_Deck(Game game, int deck_size, value values[], color colors[], suit suits[]){

    int deckStart = NUM_PLAYERS * STARTING_HAND_SIZE + 1;
    int expectedCards = deck_size - NUM_PLAYERS * STARTING_HAND_SIZE - 1;

    int i = 0;
    Card card = getDeckCard(game, i);
    while (card != NULL && i < expectedCards){
        int originalDeckIndex = deckStart + i;

        int v = values[originalDeckIndex];
        int c = colors[originalDeckIndex];
        int s = suits[originalDeckIndex];

        printf("Checking card %d in deck is ", i);
        printCardByComponents(v, c, s);
        printf("\n");

        assert(cardMatchesComponents(card, v, c, s));

        i += 1;
        card = getDeckCard(game, i);
    }

    printf("Checking the deck has %d cards\n", expectedCards);
    assert(i == expectedCards);
}

static void check_initial_DiscardPile(Game game, int deck_size, value values[], color colors[], suit suits[]){
    Card card;
    int firstCard = NUM_PLAYERS * STARTING_HAND_SIZE;

    printf("Checking top of discard pile is ");
    printCardByComponents(values[firstCard], colors[firstCard], suits[firstCard]);
    printf("\n");

    card = getDiscardPileCard(game, 0);
    assert(cardMatchesComponents(card, values[firstCard], colors[firstCard], suits[firstCard]));

    printf("Checking there are no other cards in the discard pile\n");

    card = getDiscardPileCard(game, 1);
    assert(card == NULL);
}


static void check_initial_PlayerHand(Game game, int player, value values[], color colors[], suit suits[]){
    int originalDeck_index = player;

    // Array to keep track of the index of cards in a player's hand we've already
    // matched against
    int alreadyMatched[STARTING_HAND_SIZE];
    for (int i = 0; i < STARTING_HAND_SIZE; i++){
        alreadyMatched[i] = FALSE;
    }

    for(int k = 0; k < STARTING_HAND_SIZE; k++){

        int v = values[originalDeck_index];
        int c = colors[originalDeck_index];
        int s = suits [originalDeck_index];
        originalDeck_index = originalDeck_index + NUM_PLAYERS;

        printf("\n---------- ----------------- ------------ \n");
        printf("Checking card ");
        printCardByComponents(v, c, s);
        printf(" is in player %d's hand\n", player);

        assert(cardIsInHand(game, player, alreadyMatched, v, c, s));
    }

    Card card = getHandCard(game, player, STARTING_HAND_SIZE);
    assert( card == NULL );

    printf("Finish checking hand for player %d , OK. ", player);

}


static int cardIsInHand(Game game, int player, int alreadyMatched[], value v, color c, suit s){
    int i = 0;
    Card card = getHandCard(game, player, i);
    while (card != NULL){
        if (cardMatchesComponents(card, v, c, s) && !alreadyMatched[i]){
            alreadyMatched[i] = TRUE;
            return TRUE;
        }

        i++;
        card = getHandCard(game, player, i);
    }

    return FALSE;
}

static int cardMatchesComponents(Card card, value v, color c, suit s){
    return cardValue(card) == v && cardColor(card) == c && cardSuit(card) == s;
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

    printf("%s %s of %s", colorStrings[c], valueStrings[v], suitStrings[s]);
}


