// Author: Atharv Damle
// This is a sample player for playing the Final Card Down game.
// It does not use an AI but could be modified to.

// This one is very simple as it tries to find the first card
// that can be played and then plays it.

#include <stdlib.h>
#include "player.h"

#define NUM_COLORS 5

static Card findCard(Game game, Card discard);
static int isValidCard(Card a, Card check);
static color decideColor(Game game);
static int findMaxColor(Game game);
static int countColorCards(Game game, color color);


playerMove decideMove(Game game)
{
    playerMove move;

    Card card;

    // See the card on top of the draw pile
    Card discard = topDiscard(game);

    // See if any card in hand can be played
    if ((card = findCard(game, discard)) == NULL)
    {
        move.action = DRAW_CARD;
        if (!isValidMove(game, move))
        {
            move.action = END_TURN;
        }
    }
    else
    {
        move.action = PLAY_CARD;
        move.card = card;

        if (cardValue(card) == D)
        {
            move.nextColor = decideColor(game);
        }

        // If the card is not allowed
        // It could be because the player needs to draw
        // more cards
        if (!isValidMove(game, move))
        {
            move.action = DRAW_CARD;
        }

        // If player cannot draw a card either
        if (!isValidMove(game, move))
        {
            move.action = END_TURN;
        }
    }
    return move;
}

static color decideColor(Game game)
{
    // Find color with maximum cards
    int max = findMaxColor(game);
    
    // No such color exists
    // i.e it is the last card played in the game
    if (max == -1)
    {
        // Does not matter which
        // color is chosen
        max = BLUE;
    }
    return max;
}


// Finds the first card that can be played
static Card findCard(Game game, Card discard)
{
    Card card = NULL;
    int n = handCardCount(game);
    int i = 0;


    while (i < n && card == NULL)
    {
        Card current = handCard(game, i);

        // Card will not be NULL as there are n cards in hand
        if (isValidCard(current, discard))
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
        isValid = (cardSuit(a) == cardSuit(check) || 
                    cardValue(a) == cardValue(check) ||
                     cardColor(a) == cardColor(check));
    }

    return isValid;
}

// find the color whose cards in hand are
// maximum
// Say there are 2 red, 2 blue and 3 green
// The function returns green
// If there is no such color, returns -1
static int findMaxColor(Game game)
{
    color colors[] = {BLUE, RED, PURPLE,
            YELLOW, GREEN
    };
    int max = 0;
    
    // No color exists
    color maxColor = -1;
    
    int i = 0;
    while (i < NUM_COLORS)
    {
        int count = countColorCards(game, colors[i]);
        if (max < count)
        {
            // found
            max = count;
            maxColor = colors[i];
        }
        i++;
    }

    return maxColor;
}

// counts cards of a particular color in player's hand
static int countColorCards(Game game, color color)
{
    int count = 0;
    int n = handCardCount(game);

    while (count < n)
    {
        if (cardValue(handCard(game, count)) == color)
        {
            count++;
        }
    }

    return count;
}

