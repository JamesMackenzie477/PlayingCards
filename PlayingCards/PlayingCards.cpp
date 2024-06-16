#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines some standard types.
#define uint8_t		unsigned char
#define uint16_t	unsigned short
#define uint32_t	unsigned int
#define uint64_t	unsigned long long

///////////////////////////////// CARD CLASS /////////////////////////////////

// Card numbers
#define KING	13
#define QUEEN	12
#define JACK	11
#define ACE		1

// Defines the card attributes.
typedef struct Card { char Suit; uint8_t Value; } Card;

// Returns a string of the card attributes.
char * Card__ToString(Card * c)
{
	// Allocates the string.
	char * s = (char *)malloc(5);
	// Formats the string.
	sprintf_s(s, 5, "%d%c", c->Value, c->Suit);
	// Returns the string.
	return s;
}

// True if both cards are equal.
bool Card__EqualToOperator(Card * c, Card * o)
{
	return (c->Suit == c->Suit) && (c->Value == c->Value);
}

///////////////////////////////// DECK CLASS /////////////////////////////////

// Defines the card deck size.
#define DECK_SIZE 52

// Defines the deck attributes.
typedef struct Deck { Card cards[52]; } Deck;

// Populates the deck of cards.
Deck * Deck__Populate(Deck * d)
{
	// A pointer to the suit offset.
	Card * pSuit = d->cards;
	// Populates each suit by iterating a section of the array.
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'H', (uint8_t)(i + 1) }; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'C', (uint8_t)(i + 1) }; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'D', (uint8_t)(i + 1) }; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'S', (uint8_t)(i + 1) }; }
	// Returns a pointer to the head of the deck.
	return d;
}

// Populates the deck of cards with the Si Stebbins order.
Deck * Deck__PopulateSiStebbins(Deck * d, uint8_t start)
{
	// A pointer to the suit offset.
	Card * pSuit = d->cards;
	// Populates each suit by iterating a section of the array.
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'C', start }; start += 3; if (start > 13) start -= 13; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'H', start }; start += 3; if (start > 13) start -= 13; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'S', start }; start += 3; if (start > 13) start -= 13; } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i] = { 'D', start }; start += 3; if (start > 13) start -= 13; }
	// Returns a pointer to the head of the deck.
	return d;
}

// Returns the card at the index of the deck.
Card * Deck__GetCard(Deck* d, uint8_t index)
{
	// Wraps the deck index (assumes circular deck).
	if ((index %= 52) == 0) index = 52;
	// Returns the card.
	return &d->cards[index - 1];
}

// Returns the index of the specified card in the deck.
uint8_t Deck__GetCardIndex(Deck* d, Card * c)
{
	// Iterates through each card in the deck to find a match.
	for (uint8_t i = 1; i <= DECK_SIZE; i++) if (Card__EqualToOperator(c, Deck__GetCard(d, i))) return i;
	// No index was found.
	return NULL;
}

// Returns the index of the specified card in the deck.
uint8_t Deck__GetCardIndex(Deck* d, char * c)
{
	// Iterates through each card in the deck to find a match.
	for (uint8_t i = 1; i <= DECK_SIZE; i++) if (strcmp(c, Card__ToString(Deck__GetCard(d, i))) == 0) return i;
	// No index was found.
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////

// Prints the specified deck of cards.
void PrintDeck(Deck * d)
{
	// Iterates and prints the properties each card in the deck.
	for (uint8_t i = 0; i < DECK_SIZE; i++) printf("The card at position %d is %s\n", i, Card__ToString(Deck__GetCard(d, i + 1)));
}

// The main entry point of the program.
int main(int argc, char * argv[])
{
	// Stores the user's input.
	uint32_t deckPos, splitPos;

	// Creates a deck of cards on the stack.
	Deck normalDeck, siStebbinsDeck;
	// Populates the deck of cards.
	Deck__Populate(&normalDeck);
	// Populates the Si Stebbins deck.
	Deck__PopulateSiStebbins(&siStebbinsDeck, ACE);

	/////////// NORMAL DECK ///////////

	// Prints the deck of cards.
	PrintDeck(&normalDeck);
	// Prompts the user.
	printf("Please enter a card position: ");
	// Gets the user's input.
	scanf_s("%d", &deckPos);

	// Validates the user's input.
	if (deckPos <= DECK_SIZE)
	{
		// Prints the card.
		printf("The card at position %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos)));
		// Prints the card before the position.
		printf("The card in position before %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos - 1)));
		// Prints the card after the position.
		printf("The card in position after %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos + 1)));
	}

	/////////// SI STEBBINS ///////////

	// Prompts the user.
	printf("Please enter a card position: ");
	// Gets the user's input.
	scanf_s("%d", &deckPos);

	// Validates the user's input.
	if (deckPos <= DECK_SIZE)
	{
		// Prints the card.
		printf("The card at position %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&siStebbinsDeck, deckPos)));
	}

	// Stores the user's choice.
	char cardToSplit[5];
	// Prompts the user.
	printf("Please enter the card to split by: ");
	// Gets the user's input.
	scanf_s("%s", cardToSplit);

	// Gets the card at the bottom of the deck.
	uint8_t pos = Deck__GetCardIndex(&siStebbinsDeck, cardToSplit);
	// Shows the card at the top of the deck.
	if (pos)
	{
		// Prints the card.
		printf("The card at the top of the deck is %s.\n", Card__ToString(Deck__GetCard(&siStebbinsDeck, pos - 1)));
	}

	//////////// COMBINED ////////////



	// Gets the user's input.
	scanf_s("%s", cardToSplit);
}