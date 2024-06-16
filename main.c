#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines some standard types.
#define uint8_t		unsigned char
#define uint16_t	unsigned short
#define uint32_t	unsigned int
#define uint64_t	unsigned long long

// Adds support for the boolean type.
#define bool    int
#define true    1
#define false   0

#define nullptr 0

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
	snprintf(s, 5, "%d%c", c->Value, c->Suit);
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
//#define DECK_SIZE 52
// Creates a deck on the stack.
// #define DECK(_Var_Name, _Size) Deck _Var_Name; Deck__Construct(&_Var_Name, _Size);

// Defines the deck attributes.
typedef struct Deck { size_t size; Card * cards; } Deck;

// Creates the deck.
Deck * Deck__Construct(Deck * this, size_t size)
{
    // Creates the card array.
    this->cards = (Card *)malloc(sizeof(Card) * size);
    // Sets the deck size.
    this->size = size;
    // Returns the deck pointer.
    return this;
}

// Populates the deck of cards.
Deck * Deck__Populate(Deck * d)
{
    // Validates the deck size.
    if (d->size < 52) return nullptr;
	// A pointer to the suit offset.
	Card * pSuit = d->cards;
	// Populates each suit by iterating a section of the array.
	for (uint8_t i = 0; i < 13; i++) { pSuit[i].Suit = 'H'; pSuit[i].Value = (i + 1); } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i].Suit = 'C'; pSuit[i].Value = (i + 1); } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i].Suit = 'D'; pSuit[i].Value = (i + 1); } pSuit += 13;
	for (uint8_t i = 0; i < 13; i++) { pSuit[i].Suit = 'S'; pSuit[i].Value = (i + 1); }
	// Returns a pointer to the head of the deck.
	return d;
}

// Populates the deck of cards with the Si Stebbins order.
Deck * Deck__PopulateSiStebbins(Deck * d, uint8_t start)
{
    // Validates the deck size.
    if (d->size < 52) return nullptr;
	// A pointer to the suit offset.
	Card * pSuit = d->cards;
	// Populates each suit by iterating a section of the array.
	for (uint8_t i = 0, y = start; i < 52; i += 4) { pSuit[i].Suit = 'C'; pSuit[i].Value = y--; if (y == 0) y = 13; }
	for (uint8_t i = 1, y = start + 3; i < 52; i += 4) { pSuit[i].Suit = 'H'; pSuit[i].Value = y--; if (y == 0) y = 13; }
	for (uint8_t i = 2, y = start + 6; i < 52; i += 4) { pSuit[i].Suit = 'S'; pSuit[i].Value = y--; if (y == 0) y = 13; }
	for (uint8_t i = 3, y = start + 9; i < 52; i += 4) { pSuit[i].Suit = 'D'; pSuit[i].Value = y--; if (y == 0) y = 13; }
	// Returns a pointer to the head of the deck.
	return d;
}

// Returns the card at the index of the deck.
Card * Deck__GetCard(Deck* d, uint8_t index)
{
	// Wraps the deck index (assumes circular deck).
	if ((index %= d->size) == 0) index = d->size;
	// Returns the card.
	return &d->cards[index - 1];
}

// Returns the index of the specified card in the deck.
uint8_t Deck__GetCardIndex(Deck* d, Card * c)
{
	// Iterates through each card in the deck to find a match.
	for (uint8_t i = 1; i <= d->size; i++) if (Card__EqualToOperator(c, Deck__GetCard(d, i))) return i;
	// No index was found.
	return NULL;
}

// Returns the index of the specified card in the deck (using the string representation).
uint8_t Deck__GetCardStringIndex(Deck* d, char * c)
{
	// Iterates through each card in the deck to find a match.
	for (uint8_t i = 1; i <= d->size; i++) if (strcmp(c, Card__ToString(Deck__GetCard(d, i))) == 0) return i;
	// No index was found.
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////

// Prints the specified deck of cards.
void PrintDeck(Deck * d)
{
	// Iterates and prints the properties each card in the deck.
	for (uint8_t i = 0; i < d->size; i++) printf("The card at position %d is %s\n", i, Card__ToString(Deck__GetCard(d, i + 1)));
}

// The main entry point of the program.
int main(int argc, char * argv[])
{
	// Stores the user's input.
	uint32_t deckPos;

	// Creates a deck of cards on the stack.
	Deck normalDeck, siStebbinsDeck, combinedDeck;
	// Calls the class constructor on each instance.
	Deck__Construct(&normalDeck, 52);
	Deck__Construct(&siStebbinsDeck, 52);
	Deck__Construct(&combinedDeck, normalDeck.size + siStebbinsDeck.size);
	// Populates the deck of cards.
	Deck__Populate(&normalDeck);

	/////////// NORMAL DECK ///////////

	// Prints the deck of cards.
	PrintDeck(&normalDeck);
	// Prompts the user.
	printf("Please enter a card position: ");
	// Gets the user's input.
	scanf("%d", &deckPos);

	// Validates the user's input.
	if (deckPos <= normalDeck.size)
	{
		// Prints the card.
		printf("The card at position %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos)));
		// Prints the card before the position.
		printf("The card in the position before %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos - 1)));
		// Prints the card after the position.
		printf("The card in the position after %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&normalDeck, deckPos + 1)));
	}

	/////////// SI STEBBINS ///////////

    // Populates the Si Stebbins deck.
	Deck__PopulateSiStebbins(&siStebbinsDeck, ACE);

	PrintDeck(&siStebbinsDeck);
	// Prompts the user.
	printf("Please enter a card position: ");
	// Gets the user's input.
	scanf("%d", &deckPos);

	// Validates the user's input.
	if (deckPos <= siStebbinsDeck.size)
	{
		// Prints the card.
		printf("The card at position %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&siStebbinsDeck, deckPos)));
	}

	// Stores the user's choice.
	char cardToSplit[5];
	// Prompts the user.
	printf("Please enter the card to split by: ");
	// Gets the user's input.
	scanf("%s", cardToSplit);

	// Gets the card at the bottom of the deck.
	uint8_t pos = Deck__GetCardStringIndex(&siStebbinsDeck, cardToSplit);
	// Shows the card at the top of the deck.
	if (pos)
	{
		// Prints the card.
		printf("The card at the top of the deck is %s.\n", Card__ToString(Deck__GetCard(&siStebbinsDeck, pos + 1)));
	}

	//////////// COMBINED ////////////

    // Combines both decks.
	for (int i = 0, y = 0; y < (normalDeck.size + siStebbinsDeck.size); i++, y += 2)
    {
        combinedDeck.cards[y] = normalDeck.cards[i];
        combinedDeck.cards[y + 1] = siStebbinsDeck.cards[i];
    }

    // Prompts the user.
	printf("Please enter a card position: ");
	// Gets the user's input.
	scanf("%d", &deckPos);

	// Validates the user's input.
	if (deckPos <= combinedDeck.size)
	{
		// Prints the card.
		printf("The card at position %d is %s.\n", deckPos, Card__ToString(Deck__GetCard(&combinedDeck, deckPos)));
	}
}
