#include <stdio.h>
#include <stdlib.h>


typedef struct _deck *Deck;
struct _deck {
    int data;
    struct _deck *nextCard;
};

static Deck createDeck(int deckSize, int data[])
{
	// Allocate memory for the first element in list
	Deck head = malloc(sizeof(struct _deck));
	
	// Initialize first card
	head -> data = data[0];
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
		current -> data = data[i];
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


int main(void)
{
    int data[10] = {1,2,3,4,5,6,7,8,9,0};
    Deck new = createDeck(10, data);
    Deck current = new;
    int i = 0;
    while (i < 10)
    {
        printf("%d\n", current -> data);
        current = current -> nextCard;
        i++;
    }
    return 0;
}
