// stage2.c (version v2, updated 11pm 24/Oct/2018)
//
// Stage 2 tests for assignment 2, 
// Covering only basic actions.
// The program checks some (and not all) of the game state.
//
// Jacob Mikkelsen
// 24/10/2018

#include <stdio.h>
#include <assert.h>

#include "Game.h"
#include "Card.h"

#define STARTING_HAND_SIZE 7

#define NUM_VALUES 16
#define NUM_COLORS 5
#define NUM_SUITS 5

static void checkPlayer0Turn1(Game game);
static void checkPlayer1Turn1(Game game);
static void checkPlayer2Turn1(Game game);
static void checkPlayer3Turn1(Game game);
static void checkPlayer0Turn2(Game game);
static void checkPlayer1Turn2(Game game);

static Card findCardInHand(Game game, int player, value v, color c, suit s);
static Card findCardInDeck(Game game, value v, color c, suit s);
static int cardMatchesComponents(Card card, value v, color c, suit s);
static void printCard(Card card);
static void printCardByComponents(value v, color c, suit s);
static void printInitialCards(int deck_size, value values[], color colors[], suit suits[]);
static void checkGameState(Game game, int expectedPlayer, int expectedTurn,
    int expectedMoves, int expectedPreviousPlayer, int expectedTopDiscardTurn);
static void checkTopDiscard(Game game, Card card);
static void test1_play_basic_hands(void);

int main (void){
    
    test1_play_basic_hands();
    
    // you should yout own tests here ... 
    
}

/*  ----------------------------------------
        ... Test functions below ...
*/

static void test1_play_basic_hands(void){
    int deck_size = 36;

    value values[] = {
        THREE, THREE, C, A,
        E, ZERO, F, B,
        A, ZERO, E, DRAW_TWO,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        A, THREE, FOUR, ZERO,
        THREE, NINE, D, B,
        SIX, SIX, FOUR, FIVE
    };

    color colors[] = {
        RED, BLUE, YELLOW, PURPLE,
        YELLOW, BLUE, YELLOW, GREEN,
        GREEN, BLUE, BLUE, BLUE,
        PURPLE, PURPLE, RED, BLUE,
        GREEN, GREEN, PURPLE, RED,
        GREEN, GREEN, BLUE, RED,
        PURPLE, YELLOW, BLUE, BLUE,
        RED, PURPLE, YELLOW, YELLOW,
        RED, YELLOW, RED, PURPLE
    };

    suit suits[] = {
        SPADES, DIAMONDS, DIAMONDS, CLUBS,
        CLUBS, SPADES, SPADES, CLUBS,
        SPADES, CLUBS, DIAMONDS, HEARTS,
        DIAMONDS, SPADES, QUESTIONS, SPADES,
        CLUBS, DIAMONDS, QUESTIONS, QUESTIONS,
        QUESTIONS, CLUBS, HEARTS, HEARTS,
        CLUBS, CLUBS, DIAMONDS, SPADES,
        HEARTS, DIAMONDS, CLUBS, HEARTS,
        DIAMONDS, HEARTS, HEARTS, DIAMONDS
    };

    // Important: 
    // We can derive most of the required information for testing 
    // from these three arrays provided to "newGame". 
    
    printInitialCards(deck_size, values, colors, suits);
    Game game = newGame(deck_size, values, colors, suits);

    checkPlayer0Turn1(game);

    checkPlayer1Turn1(game);

    checkPlayer2Turn1(game);

    checkPlayer3Turn1(game);

    checkPlayer0Turn2(game);

    checkPlayer1Turn2(game);

    printf("Trying to destroy the game\n");
    destroyGame(game);

    printf("All tests passed, awesome work!\n");
}

/*  ----------------------------------------
        ... Helper functions below ...
*/

static void checkPlayer0Turn1(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 0's Turn =======\n");

    // Let's check the initial state.
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);

    // We know player 0 should have the card (Three, RED, SPADES).
    // We can derive this from the initial three arrays provided to "newGame". 
    printf("Checking player 0 has ");
    printCardByComponents(THREE, RED, SPADES);
    printf("\n");
    card = findCardInHand(game, 0, THREE, RED, SPADES);
    assert(card != NULL);

    // We know the card (Three, RED, SPADES) is valid to play at 
    // this point, so let's check it.
    printf("Checking it is valid for player 0 to play this card\n");
    move.action = PLAY_CARD;
    move.card = card;  // card (Three, RED, SPADES) 
    assert(isValidMove(game, move) == TRUE);  // isValidMove should return TRUE 

    // Let's make sure that "isValidMove" above has not changed the game state.
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);

    // Let's play the card (Three, RED, SPADES). 
    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    // Let's check that the hand of player 0 is properly updated.
    // We know that there was only one card (THREE, RED, SPADES) in the hand.    
    // So, if we look for the card (THREE, RED, SPADES), it should return NULL.
    // If there were multiple cards with same three values, we need 
    // different test!
    printf("Checking player 0 no longer has ");
    printCardByComponents(THREE, RED, SPADES);
    printf("\n");
    card = findCardInHand(game, 0, THREE, RED, SPADES);
    assert(card == NULL);

    // After the move, the game should be in the following state. 
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 1, -1, 0);

    // Now player 0 should be able to END_TURN.
    // Let's check "isValidMove", it should return TRUE.
    printf("Checking player 0 can end their turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    // Let's end the turn.
    printf("Ending player 0's turn\n");
    playMove(game, move);
}

static void checkPlayer1Turn1(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 1's Turn =======\n");

    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 0, 0, 0);

    // At this point, player 1 should be able to draw a card, let's check it.
    printf("Checking player 1 could draw a card\n");
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);

    // The above "isValidMove" should not change the state.
    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 0, 0, 0);

    // From the initial three arrays, we know that player 1 should have 
    // the card (THREE, BLUE, DIAMONDS). Let's check it.
    printf("Checking player 1 has ");
    printCardByComponents(THREE, BLUE, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 1, THREE, BLUE, DIAMONDS);
    assert(card != NULL);

    // The card (THREE, BLUE, DIAMONDS) satisfies the matching criteria, 
    // so player 1 should be able to play it, and "isValidMove" 
    // should return TRUE
    printf("Checking player 1 can play this card\n");
    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == TRUE);

    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 0, 0, 0);

    printf("Playing the card\n");
    playMove(game, move);
    
    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);
    
    // Let's check that the hand of player 1 is properly updated.
    // We know that there was only one card (THREE, BLUE, DIAMONDS) in the hand.    
    // So, if we look for the card (THREE, BLUE, DIAMONDS), it should return NULL.
    // If there were multiple cards with same three values, we need 
    // different test!    
    printf("Checking player 1 no longer has ");
    printCardByComponents(THREE, BLUE, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 1, THREE, BLUE, DIAMONDS);
    assert(card == NULL);

    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 1, 0, 1);

    printf("Checking player 1 can end their turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending player 1's turn\n");
    playMove(game, move);
}

static void checkPlayer2Turn1(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 2's Turn =======\n");

    printf("Checking the game state\n");
    checkGameState(game, 2, 2, 0, 1, 1);

    printf("Checking player 2 has ");
    printCardByComponents(C, YELLOW, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 2, C, YELLOW, DIAMONDS);
    assert(card != NULL);

    printf("Checking player 2 can play this card\n");
    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == TRUE);

    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    printf("Checking player 2 no longer has ");
    printCardByComponents(C, YELLOW, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 2, C, YELLOW, DIAMONDS);
    assert(card == NULL);

    printf("Checking the game state\n");
    checkGameState(game, 2, 2, 1, 1, 2);

    printf("Checking player 2 has ");
    printCardByComponents(F, YELLOW, SPADES);
    printf("\n");
    card = findCardInHand(game, 2, F, YELLOW, SPADES);
    assert(card != NULL);

    // They can play only one card in one turn, 
    // so the next move with PLAY_CARD should fail!
    printf("Checking they cannot play the card\n");
    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == FALSE);

    printf("Checking player 2 can end their turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending player 2's turn\n");
    playMove(game, move);
}

static void checkPlayer3Turn1(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 3's Turn =======\n");
    checkGameState(game, 3, 3, 0, 2, 2);

    printf("Checking player 3 can't end their turn yet\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == FALSE);

    printf("Checking the game state\n");
    checkGameState(game, 3, 3, 0, 2, 2);

    printf("Checking player 3 can't play player 2's ");
    printCardByComponents(F, YELLOW, SPADES);
    printf("\n");
    card = findCardInHand(game, 2, F, YELLOW, SPADES);
    assert(card != NULL);

    // We know that the card (F, YELLOW, SPADES) is not in player 3's hand,
    // so the following move should be invalid.
    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == FALSE);

    printf("Checking the state hasn't changed\n");
    checkGameState(game, 3, 3, 0, 2, 2);

    printf("Checking player 3 can pick up a card\n");
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);

    printf("Checking the top of the deck is ");
    printCardByComponents(NINE, PURPLE, DIAMONDS);
    printf("\n");
    card = getDeckCard(game, 0);
    assert(cardMatchesComponents(card, NINE, PURPLE, DIAMONDS));

    printf("Player 3 picks up the card\n");
    playMove(game, move);

    printf("Checking the card is now in player 3's hand\n");
    Card cardInHand = findCardInHand(game, 3, NINE, PURPLE, DIAMONDS);
    assert(cardInHand == card);

    printf("Checking player 3 can't pick up another card\n");
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == FALSE);

    printf("Checking the game state\n");
    checkGameState(game, 3, 3, 1, 2, 2);

    printf("Checking player 3 can end their turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending player 3's turn\n");
    playMove(game, move);
}

static void checkPlayer0Turn2(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 0's Turn =======\n");
    printf("Checking the game state\n");
    checkGameState(game, 0, 4, 0, 3, 2);
    
    printf("Checking player 0 has ");
    printCardByComponents(E, YELLOW, CLUBS);
    printf("\n");
    card = findCardInHand(game, 0, E, YELLOW, CLUBS);
    assert(card != NULL);

    printf("Checking player 0 can play this card\n");
    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == TRUE);

    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    printf("Checking player 0 no longer has ");
    printCardByComponents(E, YELLOW, CLUBS);
    printf("\n");
    card = findCardInHand(game, 0, E, YELLOW, CLUBS);
    assert(card == NULL);

    printf("Checking the new game state\n");
    checkGameState(game, 0, 4, 1, 3, 4);

    printf("Checking player 0 can end their turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending player 0's turn\n");
    playMove(game, move);
}

static void checkPlayer1Turn2(Game game){
    playerMove move;
    Card card;

    printf("\n======= Player 1's Turn =======\n");

    printf("Checking the game state\n");

    checkGameState(game, 1, 5, 0, 0, 4);

    printf("Checking player 1 has ");
    printCardByComponents(ZERO, BLUE, SPADES);
    printf("\n");

    card = findCardInHand(game, 1, ZERO, BLUE, SPADES);
    assert(card != NULL);

    printf("Checking player 1 can play this card\n");

    move.action = PLAY_CARD;
    move.card = card;
    assert(isValidMove(game, move) == TRUE);

    printf("Playing the card\n");

    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");

    checkTopDiscard(game, card);

    printf("Checking player 1 no longer has ");
    printCardByComponents(ZERO, BLUE, SPADES);
    printf("\n");

    card = findCardInHand(game, 1, ZERO, BLUE, SPADES);
    assert(card == NULL);

    printf("Checking the game state\n");

    checkGameState(game, 1, 5, 1, 0, 5);

    printf("Checking player 1 can end their turn\n");

    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending player 1's turn\n");

    playMove(game, move);

    printf("Checking the game state\n");

    checkGameState(game, 2, 6, 0, 1, 5);
}

// Checks various aspects of the game's state
static void checkGameState(Game game, int expectedPlayer, int expectedTurn,
        int expectedMoves, int expectedPreviousPlayer, int expectedTopDiscardTurn){

    assert(currentPlayer(game) == expectedPlayer);
    assert(currentTurn(game) == expectedTurn);
    assert(numTurns(game) == expectedTurn + 1);
    assert(currentTurnMoves(game) == expectedMoves);
    assert(getPreviousTurnPlayer(game) == expectedPreviousPlayer);
    assert(getTopDiscardTurnNumber(game) == expectedTopDiscardTurn);
}

// Tries to find a card with the given values in a player's hand and returns it
static Card findCardInHand(Game game, int player, value v, color c, suit s){
    int i = 0;
    Card card = getHandCard(game, player, i);
    while (card != NULL){
        if (cardMatchesComponents(card, v, c, s)){
            return card;
        }

        i++;
        card = getHandCard(game, player, i);
    }

    return NULL;
}

// Tries to find a card with the given values in the deck and returns it
static Card findCardInDeck(Game game, value v, color c, suit s){
    int i = 0;
    Card card = getDeckCard(game, i);
    while (card != NULL){
        if (cardMatchesComponents(card, v, c, s)){
            return card;
        }

        i++;
        card = getDeckCard(game, i);
    }

    return NULL;
}

// Checks if the top of the discard value is a particular card
static void checkTopDiscard(Game game, Card card){
    assert(getDiscardPileCard(game, 0) == card);
}

// Compare a card to the various components of a card
static int cardMatchesComponents(Card card, value v, color c, suit s){
    printf("Comparing with card: ");
    printCard(card);
    return cardValue(card) == v && cardColor(card) == c && cardSuit(card) == s;
}

// Calls the `printCardByComponents` function from the stage 1 tests.
// This function is copied from Andrew Bennett's assignment hints page.
static void printCard(Card card) {
    value v = cardValue(card);
    color c = cardColor(card);
    suit s = cardSuit(card);

    printCardByComponents(v, c, s);
    printf("\n");
}

static void printInitialCards(int deck_size, value values[], color colors[], suit suits[]) {
    printf("The initial cards passed to newGame are:\n");
    for (int i = 0; i < deck_size; i++) {
        // prints the index of the card within the starting deck, to
        // make it easier to see what's going on.
        printf("[card %2d] ", i);
        printCardByComponents(values[i], colors[i], suits[i]);
        printf("\n");
    }
}

// Print cards by their components in a nice format
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
