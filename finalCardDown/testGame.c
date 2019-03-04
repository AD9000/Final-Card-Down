//Author: Atharv Damle (z5232949)

/*
 * Tests to check:
 * A: newGame
 * 1. Null arrays/no deck
 * 2. Size of hand is zero
 * 3. Size of startDeck < number of players + 1
 *
 * 4. First card is a special card: ----> The normal conditions should work
 *
 * B: isValidMove
 *
 * 1. Illegal moves : null cards, etc
 * 2. Normal cases
 * 3. Top card is zero - check what ends up as the topCard
 * 4. How many cards can one player draw at once?
 *         - normal conditions
 *         - there are active draw twos
 * 5. Check player can only end turn when:
 *         - He has played one move at least or
 *         - He has drawn 2*active draw twos if required
 * 6. Special cases : What if player wants to play a special card and what if a player wants to play cards
 *                       over a special card
 *
 * C: playMove
 *
 * 1. Illegal moves : null cards, etc
 * 2. Normal cases
 * 3. How many cards can one player draw at once?
 *         - normal conditions
 *         - there are active draw twos
 * 4. Check player can only end turn when:
 *         - He has played one move at least or
 *         - He has drawn 2*active draw twos if required
 * 5. Special cases : What if player wants to play a special card and what if a player wants to play cards
 *                       over a special card
 * Look at how the game state is changed in cases 1 to 5
 *
 * 6. Can you add a card to the discardPile, does the card exist in the players hand
 * 7. Check the values of nextColor
 *
 * D: Other
 * 1. Keep track of player's points
 * 2. Check if someone wins the game
 * 3. Play a wild card
 * 4. Change color to color which is not in the game
 */


// Note: Used code from stage1.c and stage2.c (by Jacob Mikkelsen)

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

static void check_initial_PlayerHand(Game game, 
    int player, value values[], color colors[], suit suits[]);
static void check_initial_Deck(Game game, 
    int deck_size, value values[], color colors[], suit suits[]);
static void check_initial_DiscardPile(Game game, 
    int deck_size, value values[], color colors[], suit suits[]);

static void checkPlayer0Turn1(Game game);
static void checkPlayer1Turn1(Game game);
static void checkPlayer2Turn1(Game game);
static void checkPlayer3Turn1(Game game);
static void checkPlayer0Turn2(Game game);
static void checkPlayer1Turn2(Game game);

static int cardIsInHand(Game game, int player, 
    int alreadyMatched[], value v, color c, suit s);
static int cardMatchesComponents(Card card, value v, color c, suit s);
static void printCardByComponents(value v, color c, suit s);
static void test1_init_deck_hands(void);
static void checkTopDiscard(Game game, Card card);
static void checkGameState(Game game, int expectedPlayer, int expectedTurn,
        int expectedMoves, int expectedPreviousPlayer, int expectedTopDiscardTurn);
static void checkGameState2(Game game, int expectedDrawTwos, color expectedColor);

// Tries to find a card with the given values in a player's hand and returns it
static Card findCardInHand(Game game, int player, value v, color c, suit s);

static void test1_play_basic_hands(void);

static void testNewGameNotNull();
static void testNewGameWithNull();
static void test_stage3();
static void checkLessCardsInDeck();
static void checkSpecialFirstCard();
static void checkIsValidMove();

static void isValidMoveCheckPlayer0(Game game);
static void isValidMoveCheckPlayer1(Game game);
static void isValidMoveCheckPlayer2(Game game);
static void isValidMoveCheckPlayer3(Game game);

static void checkValues();
static void playRound();
static void otherPlayer0Test1(Game game);
static void otherPlayerTest(Game game, int player);
static Card findCardToPlay(Game game);
static int isValidCard(Card a, Card check);



int main (void) {

    printf("----------------------------------------------\n");
    printf("            Basic Testing\n");
    printf("----------------------------------------------\n");

    // Very basic tests
    testNewGameNotNull();
    testNewGameWithNull();

    // stage 1
    test1_init_deck_hands();

    // stage 2
    test1_play_basic_hands();

    // stage 3 - Check special cases
    // and return values of functions
    test_stage3();

    // Normal turn
    playRound();
}

/*  ----------------------------------------
        ... Test functions below ...
*/



// Initial testing before stage1 code
// Check that game is created. Not null
static void testNewGameNotNull()
{

    printf("\n------------Test - 1--------------\n");
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
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
        YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, 
        PURPLE, PURPLE
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS
    };

    printf("Testing game is created\n");
    Game game = newGame(deck_size, values, colors, suits);
    assert(game != NULL);
    destroyGame(game);
}

// Tests newGame with a NULL pointer. What if the arrays are null?
static void testNewGameWithNull()
{
    printf("\n------------Test - 2--------------\n");
    printf("Testing game with deck of size 0\n");
    int deck_size = 0;
    value values[] = {};
    color colors[] = {};
    suit suits[] = {};

    Game game = newGame(deck_size, values, colors, suits);
    assert(game == NULL);
    printf("Passed\n");


    printf("Destroying game\n");
    destroyGame(game);
}

// Stage 1 code
static void test1_init_deck_hands(void) {

    printf("\n----------------------------------------------\n");
    printf("            Stage 1 Testing\n");
    printf("----------------------------------------------\n");

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
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 
        YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, 
        PURPLE, PURPLE
    };

    suit suits[] = {
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS, HEARTS, DIAMONDS, 
        CLUBS, SPADES, QUESTIONS
    };

    Game game = newGame(deck_size, values, colors, suits);

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

    printf("All tests passed, awesome work!\n");
}


// Stage 2 code
static void test1_play_basic_hands(void){

    printf("\n----------------------------------------------\n");
    printf("            Stage 2 Testing\n");
    printf("----------------------------------------------\n");

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
        SPADES, SPADES, DIAMONDS, HEARTS,
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

// stage 3
static void test_stage3()
{

    printf("\n----------------------------------------------\n");
    printf("            Stage 3 Testing\n");
    printf("----------------------------------------------\n");
    // newGame() tests

    // 1. Size of deck is less than number of players + 1
    checkLessCardsInDeck();

    // 2. First card is a special card
    // (Normal cases were checked in stage 1 and 2)
    checkSpecialFirstCard();

    // isValidMove() and playMove() tests
    // (Normal cases checked in stage 2

    // Check how many cards a player can draw at once
    checkIsValidMove();

    printf("All tests passed, awesome work!\n");
}

static void playRound()
{
    printf("\n----------------------------------------------\n");
    printf("            Other Tests\n");
    printf("----------------------------------------------\n");

    // Other cases
    checkValues();

    printf("All tests passed, awesome work!\n");
}

// Game can only be played if there are at least
// NUM_PLAYERS + 1 cards in deck
static void checkLessCardsInDeck()
{
    int deck_size = 3;
    value values[] = {
        THREE, THREE, C
    };

    color colors[] = {
        RED, BLUE, YELLOW
    };

    suit suits[] = {
        SPADES, DIAMONDS, DIAMONDS
    };

    printf("Checking if there are less than 5 cards in the deck\n");
    Game game = newGame(deck_size, values, colors, suits);
    assert(game == NULL);
    printf("Passed\n");
}

// What if first card is a special card?
static void checkSpecialFirstCard()
{
    int deck_size = 36;

    value values1[] = {
        THREE, THREE, C, A,
        E, ZERO, F, B,
        A, ZERO, E, DRAW_TWO,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        A, THREE, FOUR, ZERO,
        A, NINE, D, B,
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
        PURPLE, PURPLE, YELLOW, YELLOW,
        RED, YELLOW, RED, PURPLE
    };

    suit suits[] = {
        SPADES, DIAMONDS, DIAMONDS, CLUBS,
        CLUBS, SPADES, SPADES, CLUBS,
        SPADES, SPADES, DIAMONDS, HEARTS,
        DIAMONDS, SPADES, QUESTIONS, SPADES,
        CLUBS, DIAMONDS, QUESTIONS, QUESTIONS,
        QUESTIONS, CLUBS, HEARTS, HEARTS,
        CLUBS, CLUBS, DIAMONDS, SPADES,
        HEARTS, DIAMONDS, CLUBS, HEARTS,
        DIAMONDS, HEARTS, HEARTS, DIAMONDS
    };

    printf("Case 1: First card in discardPile is a special card - Skip (A)\n");
    Game game = newGame(deck_size, values1, colors, suits);

    printf("Checking the game state\n");
    checkGameState(game, 1, 0, 0, -1, -1);
    checkGameState2(game, 0, PURPLE);


    destroyGame(game);

    value values2[] = {
        THREE, THREE, C, A,
        E, ZERO, F, B,
        A, ZERO, E, DRAW_TWO,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        DRAW_TWO, THREE, FOUR, ZERO,
        DRAW_TWO, NINE, D, B,
        SIX, SIX, FOUR, FIVE
    };

    printf("Case 2: First card in discardPile is a special card - Draw two\n");
    game = newGame(deck_size, values2, colors, suits);

    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);
    checkGameState2(game, 1, PURPLE);

    destroyGame(game);

    value values3[] = {
        THREE, THREE, C, A,
        E, ZERO, F, B,
        A, ZERO, E, DRAW_TWO,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        A, THREE, FOUR, ZERO,
        D, NINE, D, B,
        SIX, SIX, FOUR, FIVE
    };
    printf("Case 3: First card in discardPile is a special card - D (wild card)\n");
    game = newGame(deck_size, values3, colors, suits);

    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);
    checkGameState2(game, 0, PURPLE);

    destroyGame(game);


    value values4[] = {
        THREE, THREE, C, A,
        E, ZERO, F, B,
        A, ZERO, E, DRAW_TWO,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, THREE,
        ZERO, THREE, FOUR, ZERO,
        ZERO, NINE, D, B,
        SIX, SIX, FOUR, FIVE
    };
    printf("Case 4: First card in discardPile is a special card - zero\n");
    game = newGame(deck_size, values4, colors, suits);

    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);
    checkGameState2(game, 0, PURPLE);

    destroyGame(game);

    printf("Passed\n");

}

static Card findCardToPlay(Game game)
{
    Card card = NULL;
    // Top card on discardPile
    Card discard = getDiscardPileCard(game, 0);

    int n = handCardCount(game);
    int i = 0;
    while (i < n && card == NULL)
    {
        Card current = handCard(game, i);

        // Card will not be NULL as there are n cards in hand
        if (cardValue(discard) == D)
        {
            if (cardSuit(current) == cardSuit(current) || cardValue(current) == D || cardColor(current) == getCurrentColor(game))
            {
                card = current;
            }
        }
        else if (isValidCard(current, discard))
        {
            card = current;
        }

        i++;
    }

    return card;
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
// Checks that player cannot draw excessive cards
static void checkIsValidMove()
{
    int deck_size = 36;

    value values[] = {
        THREE, DRAW_TWO, DRAW_TWO, A,
        E, ZERO, F, B,
        A, ZERO, E, B,
        ONE, SIX, FIVE, THREE,
        ONE, SEVEN, F, F,
        FOUR, ZERO, A, D,
        A, THREE, FOUR, ZERO,
        ONE, NINE, THREE, FOUR,
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
        PURPLE, PURPLE, YELLOW, YELLOW,
        RED, YELLOW, RED, PURPLE
    };

    suit suits[] = {
        SPADES, DIAMONDS, DIAMONDS, CLUBS,
        CLUBS, SPADES, SPADES, CLUBS,
        SPADES, SPADES, DIAMONDS, HEARTS,
        DIAMONDS, SPADES, QUESTIONS, SPADES,
        CLUBS, DIAMONDS, QUESTIONS, QUESTIONS,
        QUESTIONS, CLUBS, HEARTS, HEARTS,
        CLUBS, CLUBS, DIAMONDS, SPADES,
        DIAMONDS, DIAMONDS, DIAMONDS, HEARTS,
        DIAMONDS, HEARTS, HEARTS, DIAMONDS
    };

    Game game = newGame(deck_size, values, colors, suits);

    printf("----------------------------------------------\n");
    printf("   Checking the maximum number of cards a \n"
            "   player can draw at any point in the game   \n");

    printf("----------------------------------------------\n");
    printf("   Case 1: Top card is not a draw two   \n");
    printf("----------------------------------------------\n");
    isValidMoveCheckPlayer0(game);

    printf("----------------------------------------------\n");
    printf("\n   Case 2: Top card is a draw two   \n");
    printf("   Playing two turns first\n");
    printf("----------------------------------------------\n");
    isValidMoveCheckPlayer1(game);
    isValidMoveCheckPlayer2(game);
    isValidMoveCheckPlayer3(game);
    printf("----------------------------------------------\n");

}

static void checkValues()
{
    int deck_size = 50;
    value values[] = {
        // Player hands
        ZERO, DRAW_TWO, D, ONE,
        FOUR, FIVE, SIX, SEVEN, EIGHT,
        NINE, THREE, DRAW_TWO, DRAW_TWO,
        A, E, ZERO, F,
        B, A, ZERO, E,
        B, ONE, SIX, FIVE,
        THREE, ONE, SEVEN, F,
        // discardPile card
        F,
        // drawPile cards
        FOUR,
        ZERO, A, D, A, THREE, FOUR, ZERO, ONE, NINE,
        THREE, FOUR, SIX, SIX, FOUR, FIVE, E, F, ZERO, ONE
    };

    color colors[] = {
        // Player hands
        RED, BLUE, YELLOW, PURPLE,
        YELLOW, BLUE, YELLOW, GREEN,
        GREEN, BLUE, BLUE, BLUE,
        PURPLE, PURPLE, RED, BLUE,
        GREEN, GREEN, PURPLE, RED,
        GREEN, GREEN, BLUE, RED,
        PURPLE, YELLOW, BLUE, BLUE,
        // discardPile card
        PURPLE,
        // drawPile cards
        PURPLE,
        YELLOW, YELLOW, RED, YELLOW, RED, PURPLE, YELLOW, YELLOW, YELLOW, YELLOW,
        PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, 
        PURPLE, PURPLE, PURPLE
    };

    suit suits[] = {
        // Player hands
        HEARTS, SPADES, DIAMONDS, DIAMONDS,
        CLUBS, CLUBS, SPADES, SPADES,
        CLUBS, SPADES, SPADES, DIAMONDS,
        HEARTS, DIAMONDS, SPADES, QUESTIONS,
        SPADES, CLUBS, DIAMONDS, QUESTIONS,
        QUESTIONS, QUESTIONS, CLUBS, HEARTS,
        HEARTS, CLUBS, CLUBS, DIAMONDS,
        // discardPile card
        SPADES,
        // drawPile cards
        DIAMONDS, DIAMONDS, DIAMONDS, HEARTS,
        DIAMONDS, HEARTS, HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS,
        HEARTS, DIAMONDS, QUESTIONS, SPADES, QUESTIONS, QUESTIONS, QUESTIONS,
        SPADES, HEARTS, HEARTS
    };

    Game game = newGame(deck_size, values, colors, suits);

    otherPlayer0Test1(game);
    otherPlayerTest(game,1);
    otherPlayerTest(game, 2);

}
//-------------------------------------------------------------------------
// Helper functions:
//-------------------------------------------------------------------------

// Make a normal move
// Find a card to play from the players hand
// and play it
static void otherPlayerTest(Game game, int player)
{
    playerMove move;
    Card card;

    printf("-------------Player %d's turn-----------\n", player);

    Card play = findCardToPlay(game);
    if (play != NULL)
    {
        printf("Checking player 0 has ");
        printCardByComponents(cardValue(play), cardColor(play), cardSuit(play));
        printf("\n");
        card = findCardInHand(game, player, cardValue(play), cardColor(play), cardSuit(play));
        assert(card != NULL);

        // We know the card is valid to play at
        // this point, so let's check it.
        printf("Checking it is valid for player %d to play this card\n", player);
        move.action = PLAY_CARD;
        move.card = card;
        assert(isValidMove(game, move) == TRUE);

        // Let's play the card
        printf("Playing the card\n");
        playMove(game, move);

        printf("Checking played card is at the top of the discard pile\n");
        checkTopDiscard(game, card);
    }
    else
    {
        printf("Since player cannot play a card, trying to draw a card\n");
        move.action = DRAW_CARD;

        if (isValidMove(game, move))
        {
            // Draw a card
            playMove(game, move);
        }
        else
        {
            printf("Checking player 1 can end turn\n");
            move.action = END_TURN;
            assert(isValidMove(game, move) == TRUE);

            printf("Ending turn\n");
            playMove(game, move);
        }
    }

}

// Testing special action of zero
static void otherPlayer0Test1(Game game)
{
    playerMove move;
    Card card;


    printf("-------------Player 0's turn-----------\n");
    // Since the player will play a zero, it does
    // not matter what the top card is
    // So even though the top card of the drawPile
    // is PURPLE F of SPADES,
    // The player must be able to play a card

    // Check game state
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);
    checkGameState2(game, 0, PURPLE);

    printf("Checking player 0 has ");
    printCardByComponents(ZERO, RED, HEARTS);
    printf("\n");
    card = findCardInHand(game, 0, ZERO, RED, HEARTS);
    assert(card != NULL);

    // We know the card (ZERO, RED, HEARTS) is valid to play at
    // this point, so let's check it.
    printf("Checking it is valid for player 1 to play this card\n");
    move.action = PLAY_CARD;
    move.card = card;  // card (ZERO, RED, HEARTS)
    assert(isValidMove(game, move) == TRUE);  // isValidMove should return TRUE

    // Let's make sure that "isValidMove" above has not changed the game state.
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);
    checkGameState2(game, 0, PURPLE);

    // Let's play the card (ZERO, RED, HEARTS).
    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    printf("Checking player 0 can end turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Checking player 0's points\n");
    assert(playerPoints(game, 0) == 48);

    printf("Checking game is not over yet\n");
    assert(gameWinner(game) == NOT_FINISHED);

    printf("Ending turn\n");
    playMove(game, move);

}

static void isValidMoveCheckPlayer0(Game game)
{
    playerMove move;
    Card card;

    printf("-------------Player 0's turn-----------\n");
    // Value of top card of discardPile is NINE
    // Thus player can only draw 1 card

    printf("Checking it is valid for player 0 to draw a card\n");
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == TRUE);

    // Checking "isValidMove" above has not changed the game state.
    printf("Checking the game state\n");
    checkGameState(game, 0, 0, 0, -1, -1);

    //Draw the card (NINE, PURPLE, DIAMONDS).
    printf("Drawing the card\n");
    playMove(game, move);

    // Player drew the card
    // thus, it must be in his hand
    printf("Checking player 0 has ");
    printCardByComponents(NINE, PURPLE, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 0, NINE, PURPLE, DIAMONDS);
    assert(card != NULL);

    printf("Checking it is NOT valid for player 0 to draw a card\n");
    move.action = DRAW_CARD;
    assert(isValidMove(game, move) == FALSE);

    printf("Checking player 0 can end turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending turn\n");
    playMove(game, move);
    printf("Passed\n");
}

static void isValidMoveCheckPlayer1(Game game)
{
    playerMove move;
    Card card;

    printf("-------------Player 1's turn-----------\n");

    // Check game state
    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 0, 0, -1);
    checkGameState2(game, 0, PURPLE);

    printf("Checking player 1 has ");
    printCardByComponents(DRAW_TWO, BLUE, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 1, DRAW_TWO, BLUE, DIAMONDS);
    assert(card != NULL);

    // We know the card (DRAW_TWO, BLUE, DIAMONDS) is valid to play at
    // this point, so let's check it.
    printf("Checking it is valid for player 1 to play this card\n");
    move.action = PLAY_CARD;
    move.card = card;  // card (DRAW_TWO, BLUE, DIAMONDS)
    assert(isValidMove(game, move) == TRUE);  // isValidMove should return TRUE

    // Let's make sure that "isValidMove" above has not changed the game state.
    printf("Checking the game state\n");
    checkGameState(game, 1, 1, 0, 0, -1);
    checkGameState2(game, 0, PURPLE);

    // Let's play the card (DRAW_TWO, YELLOW, DIAMONDS).
    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    printf("Checking player 1 can end turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending turn\n");
    playMove(game, move);
}

static void isValidMoveCheckPlayer2(Game game)
{
    playerMove move;
    Card card;

    printf("-------------Player 2's turn-----------\n");

    // Check game state
    printf("Checking the game state\n");
    checkGameState(game, 2, 2, 0, 1, 1);
    checkGameState2(game, 1, BLUE);

    printf("Checking player 2 has ");
    printCardByComponents(DRAW_TWO, YELLOW, DIAMONDS);
    printf("\n");
    card = findCardInHand(game, 2, DRAW_TWO, YELLOW, DIAMONDS);
    assert(card != NULL);

    // We know the card (DRAW_TWO, YELLOW, DIAMONDS) is valid to play at
    // this point, so let's check it.
    printf("Checking it is valid for player 2 to play this card\n");
    move.action = PLAY_CARD;
    move.card = card;  // card (DRAW_TWO, YELLOW, DIAMONDS)
    assert(isValidMove(game, move) == TRUE);  // isValidMove should return TRUE

    // Let's make sure that "isValidMove" above has not changed the game state.
    printf("Checking the game state\n");
    checkGameState(game, 2, 2, 0, 1, 1);
    checkGameState2(game, 1, BLUE);

    // Let's play the card (DRAW_TWO, YELLOW, DIAMONDS).
    printf("Playing the card\n");
    playMove(game, move);

    printf("Checking played card is at the top of the discard pile\n");
    checkTopDiscard(game, card);

    // Check that
    printf("Checking the game state\n");
    checkGameState(game, 2, 2, 1, 1, 2);
    checkGameState2(game, 2, YELLOW);

    printf("Checking player 2 can end turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending turn\n");
    playMove(game, move);
}

static void isValidMoveCheckPlayer3(Game game)
{
    playerMove move;
    Card hcard;
    // Now check that the player must
    // draw 2*number of draw twos cards
    printf("-------------Player 3's turn-----------\n");

    // Changing the value so that control enters
    // the loop
    move.action = DRAW_CARD;
    // Draw as many cards as possible
    int i = 0;
    while (isValidMove(game, move))
    {
        // Before carrying out the main test,
        // Check special case when player
        // Wants to play an invalid card (on a draw two)
        printf("Checking player cannot play any card\n");

        int j = 0;
        int n = handCardCount(game);
        while (j < n)
        {
            // Since player has no draw two,
            // the player cannot play any card
            // and must draw one

            // Also checking special case that player
            // cannot play a zero(or any other special
            // card) over a draw two as player
            // 3's deck contains all other cards
            Card card = getHandCard(game, 3, j);
            printf("The player chooses: ");
            printCardByComponents(cardValue(card), cardColor(card), cardSuit(card));
            printf("\n");
            move.action = PLAY_CARD;
            move.card = card;
            assert(isValidMove(game, move) == FALSE);
            j++;
        }

        // Since the player has not drawn enough cards
        // player cannot end turn
        printf("Checking player cannot end turn\n");
        move.action = END_TURN;
        assert(isValidMove(game, move) == FALSE);

        printf("Checking it is valid for player 3 to draw a card\n");
        move.action = DRAW_CARD;

        // Find which card will be drawn
        Card drawn = getDeckCard(game, 0);

        printf("Drawing the card\n");
        playMove(game, move);

        printf("Cards drawn: %d\n", i+1);
        // Player drew the card
        // thus, it must be in his hand
        printf("Checking player 3 has ");
        printCardByComponents(cardValue(drawn), 
            cardColor(drawn), cardSuit(drawn));
        printf("\n");
        hcard = findCardInHand(game, 3, cardValue(drawn), 
            cardColor(drawn), cardSuit(drawn));
        assert(hcard != NULL);

        i++;
    }

    // Checking how many cards the player needs
    // to draw before he can end turn
    // We know he must draw 4 cards before he
    // can end the turn as two draw twos have been played
    assert(i == 4);


    printf("Checking player 3 can end turn\n");
    move.action = END_TURN;
    assert(isValidMove(game, move) == TRUE);

    printf("Ending turn\n");
    playMove(game, move);
    printf("Passed\n");
}

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

    // At this point, player 1 should be able o draw a card, let's check it.
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

    // Let's check that the hand of player 1 is propelry updated.
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
    card = findCardInHand(game, 1, C, YELLOW, DIAMONDS);
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

    // We that the card (F, YELLOW, SPADES) is not in player 3's hand,
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
    printCardByComponents(THREE, RED, SPADES);
    printf("\n");
    card = findCardInHand(game, 0, THREE, RED, SPADES);
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

    card = findCardInHand(game, 0, ZERO, BLUE, SPADES);
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

// Checks other aspects of the game state
static void checkGameState2(Game game, int expectedDrawTwos, color expectedColor)
{
    assert(getActiveDrawTwos(game) == expectedDrawTwos);
    assert(getCurrentColor(game) == expectedColor);
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

// Checks if the top of the discard value is a particular card
static void checkTopDiscard(Game game, Card card){
    assert(getDiscardPileCard(game, 0) == card);
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


