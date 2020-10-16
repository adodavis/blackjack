#include <iostream>
#include <iomanip>
#include <time.h>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

void printHeader();
void shuffleChards(int[], int[]);
char findPicture(int);
void dealCards(int[], int[], int[], int&);
wstring findSuit(int);
int totalValues(int);
void askPlayer(int[], int[], int[], int);
void winLose(int, int);

void gotoxy(int h, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (INVALID_HANDLE_VALUE != hConsole)
	{
		COORD pos = { h, w };
		SetConsoleCursorPosition(hConsole, pos);
	}

	return;
}

int main()
{
	int cards[52], picked[52], totals[8], i = 0;
	char playAgain;

	// Randomization using time as seed
	srand(time(NULL));

	// Produces background color
	system("color 2F");

	// Produces sounds at startup
	Beep(250, 250);
	Beep(800, 200);
	Beep(457, 200);
	Beep(759, 500);

	do
	{
		shuffleChards(cards, picked);
		system("cls");

		// Zeroes out totals
		for (int j = 0; j < 8; j++)
			totals[j] = 0;

		// Clears picked cards
		for (int j = 0; j < 52; j++)
			picked[j] = 0;

		printHeader();
		dealCards(cards, picked, totals, i);
		askPlayer(cards, picked, totals, i);

		gotoxy(0, 16);

		// Prompts player to play an additional game
		wcout << "Would you like to play again (Y/N)?               ";
		gotoxy(0, 16);
		wcout << "Would you like to play again? (Y/N)? ";
		cin >> playAgain;

		if (islower(playAgain))
			playAgain = toupper(playAgain);

		system("cls");
	} while (playAgain == 'Y');

	return 0;
}

// Prints blackjack header
void printHeader()
{
	gotoxy(0, 0);

	wcout << setw(8) << "Dlr" << setw(8) << "P1" << setw(8) << "P2" << setw(8) << "P3"
		<< setw(8) << "P4" << setw(8) << "P5" << setw(8) << "P6" << setw(8) << "P7" << "\n";

	gotoxy(0, 1);

	wcout << setw(8) << "===" << setw(8) << "====" << setw(8) << "===="
		<< setw(8) << "====" << setw(8) << "====" << setw(8) << "===="
		<< setw(8) << "====" << setw(8) << "====" << "\n";
}

void shuffleChards(int cards[], int picked[])
{
	int i = 0, card;

	while (i < 52)
	{
		card = rand() % 52;

		while (picked[card] == 1)
			card = rand() % 52;

		cards[i] = card;

		picked[card] = 1;

		i++;
	}
}

// Determines picture to display on card
char findPicture(int card)
{
	char picture = ' ';

	switch (card % 13)
	{
	case 0:
		picture = 'A';
		break;
	case 10:
		picture = 'J';
		break;
	case 11:
		picture = 'Q';
		break;
	case 12:
		picture = 'K';
		break;
	default:
		picture = ' ';
	}

	return picture;
}

// Returns unicode characters for card suits
wstring findSuit(int suit)
{
	if (suit == 0)
		return L"\u2660";
	else if (suit == 1)
		return L"\u2665";
	else if (suit == 2)
		return L"\u2666";
	else
		return L"\u2663";
}

// Deals first two cards
void dealCards(int cards[], int picked[], int totals[], int& i)
{
	int j, suit, value, row = 0;
	char picture;
	wstring suitChar;

	_setmode(_fileno(stdout), _O_U16TEXT);

	i = -1;

	for (j = 0; j < 16; j++)
	{
		i++;

		if (j == 8)
		{
			row++;
			continue;
		}

		gotoxy(j % 8 * 8, row + 1);
		suit = cards[i] / 13;
		picture = findPicture(cards[i]);

		suitChar = findSuit(suit);

		// Initializes numerical value of card
		if (cards[i] % 13 < 10)
			value = cards[i] % 13 + 1;
		else
			value = 10;

		// Displays card
		if (picture != ' ')
			wcout << setw(7) << picture << suitChar << "\n";
		else
			wcout << setw(7) << value << suitChar << "\n";

		// Prints totals
		gotoxy(j % 8 * 8, 14);
		wcout << setw(8) << "TOTAL\n";
		totals[j % 8] = totals[j % 8] + totalValues(cards[i]);
		gotoxy(j % 8 * 8, 15);
		wcout << setw(6) << totals[j % 8] << "\n";
	}

	row = 2;
}

// Returns player totals
int totalValues(int i)
{
	i = i % 13;

	if (i > 8)
		return 10;
	else
		return i + 1;
}

// Prompts player to accept additional cards
void askPlayer(int cards[], int picked[], int totals[], int i)
{
	int suit, value, row, dlrTotal, playerTotal;
	char picture, response;
	wstring suitChar;

	for (int j = 1; j <= 7; j++)
	{
		row = 2;

		do
		{
			i++;

			gotoxy(0, 16);
			wcout << "Player " << j << ", would you like another card (Y/N)?          ";
			gotoxy(0, 16);
			wcout << "Player " << j << ", would you like another card? (Y/N)? ";
			cin >> response;

			if (islower(response))
				response = toupper(response);

			while (response != 'Y' && response != 'N')
			{
				gotoxy(0, 16);
				wcout << "Player " << j << ", would you like another card (Y/N)? ";
				cin >> response;
			}

			if (response == 'N')
				break;

			gotoxy(j % 8 * 8, row + 1);
			suit = cards[i] / 13;
			picture = findPicture(cards[i]);

			if (cards[i] % 13 < 10)
				value = cards[i] % 13 + 1;
			else
				value = 10;

			suitChar = findSuit(suit);

			if (picture != ' ')
				wcout << setw(7) << picture << suitChar << "\n";
			else
				wcout << setw(7) << value << suitChar << "\n";

			gotoxy(j % 8 * 8, 14);
			wcout << setw(8) << "TOTAL" << endl;
			totals[j % 8] = totals[j % 8] + totalValues(cards[i]);
			gotoxy(j % 8 * 8, 15);
			wcout << setw(6) << totals[j % 8] << endl;

			if (totals[j % 8] == 21)
			{
				Beep(750, 300);
				Beep(800, 300);
				Beep(730, 300);
				break;
			}
			else if (totals[j % 8] > 21)
			{
				Beep(350, 500);
				Beep(300, 500);
				Beep(250, 500);
				gotoxy(j % 8 * 8, 13);
				wcout << setw(8) << "BUST" << endl;
				break;
			}

			row++;

		} while (response == 'Y');
	}

	row = 1;

	// Prints dealer's cards
	while (totals[0] <= 17)
	{
		i++;

		gotoxy(0, row + 1);
		suit = cards[i] / 13;
		picture = findPicture(cards[i]);


		if (cards[i] % 13 < 10)
			value = cards[i] % 13 + 1;
		else
			value = 10;

		if (picture != ' ')
			wcout << setw(7) << picture << suitChar << endl;
		else
			wcout << setw(7) << value << suitChar << endl;

		gotoxy(0, 14);
		wcout << setw(8) << "TOTAL" << endl;
		totals[0] = totals[0] + totalValues(cards[i]);
		gotoxy(0, 15);
		wcout << setw(6) << totals[0] << endl;

		row++;

	}

	dlrTotal = totals[0];


	for (int i = 1; i < 8; i++)
	{
		playerTotal = totals[i];

		gotoxy(i % 8 * 8, 13);
		winLose(playerTotal, dlrTotal);
	}
}

// Determines if player wins, loses, or ties and prints the result
void winLose(int i, int dlrTotal)
{
	if (dlrTotal > 21 && i <= 21)
		wcout << setw(8) << "WIN" << "\n";
	else if ((dlrTotal <= 21 && i <= 21) && i > dlrTotal)
		wcout << setw(8) << "WIN" << "\n";
	else if ((dlrTotal <= 21 && i <= 21) && i < dlrTotal)
		wcout << setw(8) << "LOSE" << "\n";
	else if (i > 21)
		wcout << setw(8) << "LOSE" << "\n";
	else if (dlrTotal <= 21 && dlrTotal == i)
		wcout << setw(8) << "TIE" << "\n";
}
