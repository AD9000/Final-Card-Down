# Final-Card-Down
## Description:
This is a game that I developed during a course at UNSW.
Final Card Down is a game similar to UNO in gameplay and rules. However there are a few differences:

1. This game's cards have a number, color and a suit.
2. The card with zero is special in the sense that it can be played after any other card.

A player’s score at any point in the game is the sum of the cards in their hand.
The game ends when a player places their final card down onto the discard pile, leaving them with an empty hand. That's where the game gets its name from.

## Rules

Each player takes a turn.

During a turn a player may make one of the following actions:
1. Play a card (and discharge it on the top of the discard pile):
      - with the same value, suit or color (adjusted for a case where the last player played "D" and might have changed color, see below)
      - with a value of NULL (value of 0).
      - Draw a card from the draw pile.
      - End turn.
      
2. If the value of the Play card is
      - 2 (DRAW_TWO), the next player must either draw two cards from the deck into their hand or play another card with a value of 2. If they play a 2, they draw no cards and the player after them must draw 4 cards. Cards with a value of 2 may be stacked on each other increasing the number of cards to be drawn by 2 until a player can not place down a 2 card.
      - A (SKIP), the next player’s turn is skipped,
      - D (WILD CARD), the player may declare the color that the next card played must have (instead of the color of the card they played).
      
3. If the value of a card played by the player in the previous turn is not "2", the (current) player needs to either play a card (that satisfies the game rules) OR drawn a card, followed by end of the turn. If the value of a card played by the player in the previous turn is equal to "2", the (current) player can play another card with a value of "2" OR needs to draw two or more cards depending on the number of consecutive cards with value "2" at the top of the discard pile.

4. If there are no cards remaining in the draw pile, then the current discard pile is collected and inverted to become the draw pile. The discard pile does not need to be shuffled when turning it into the draw pile. The first card that was played into the discard pile will become the first card to be drawn from the draw pile.
If the first card in the discard pile at the start of the game is a special card, the card will have the effect that it usually had:
   - If the initial discard pile card has value 2, the first player must draw two cards (or play a card with value "2" from his/her hand).
   - If the initial discard pile card is A, the first player is skipped and it becomes the second player's turn.
   - If the initial discard pile card is D, nothing special happens, the card is treated like any other card (you can match on suit, color, or value).

5. The game ends with no winner when a player attempts to draw a card, but there is no way to draw a card – i.e. there are no cards left in the deck, and the discard pile only has one card in it.

## Next Steps/Ideas
So here I am listing a few ideas about how the project could be bettered:
(I will implement them as I get the time)

1. The game still needs a runner to call all the functions so that the players can 
   actually play the game.
2. Implement an AI player to play the game.
3. Make a GUI interface to make the game more immersive.
4. Security/Anti-cheat updates (Like calling the tester functions).
