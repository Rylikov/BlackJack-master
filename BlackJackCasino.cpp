#include <windows.h>
#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

enum class CardDeck {			//Хранилище карточных мастей 
	spade,
	club,
	diamond,
	heart,
	max,
};

enum class CardNominal {			//Хранилище карточных номиналов
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
	ten,
	jack,
	queen,
	king,
	ace,
	max,
};

struct Cards {				//Тип данных для карточного массива
	CardNominal rank;
	CardDeck shirt;
};

void printCard(const Cards& card) {	//Элемент массива расшифровывается. После чего выводятся значения карты
	switch (card.rank) {
	case CardNominal::two:		std::cout << "02_";		break;
	case CardNominal::three:	std::cout << "03_";		break;
	case CardNominal::four:		std::cout << "04_";		break;
	case CardNominal::five:		std::cout << "05_";		break;
	case CardNominal::six:		std::cout << "06_";		break;
	case CardNominal::seven:	std::cout << "07_";		break;
	case CardNominal::eight:	std::cout << "08_";		break;
	case CardNominal::nine:		std::cout << "09_";		break;
	case CardNominal::ten:		std::cout << "10_";		break;
	case CardNominal::jack:		std::cout << "Jack_";	break;
	case CardNominal::queen:	std::cout << "Queen_";	break;
	case CardNominal::king:		std::cout << "King_";	break;
	case CardNominal::ace:		std::cout << "Ace_";	break;
	}
	switch (card.shirt) {
	case CardDeck::spade:	std::cout << "Spade";		break;
	case CardDeck::club:	std::cout << "Club";		break;
	case CardDeck::diamond:	std::cout << "Diamond";		break;
	case CardDeck::heart:	std::cout << "Heart";		break;
	}
}

void printDeck(const std::array<Cards, 52>& card) {			//Распечатывается весь карточный массив
	int countSpaces = 0;
	for (const Cards index : card) {
		printCard(index);
		++countSpaces;
		if (countSpaces % 10)						//Оформляет вывод колоды в виде наглядной матрицы
			std::cout << " ";					//по 10 столбцов
		else
			std::cout << "\n";
	}
}

void swapCard(Cards& firstCard, Cards& secondCard) {			//Меняет в массиве местами две указанные карты
	printCard(firstCard);
	std::cout << "\t";
	printCard(secondCard);
	std::cout << "\n";
	std::swap(firstCard, secondCard);
	printCard(firstCard);
	std::cout << "\t";
	printCard(secondCard);
}

int getRandomNumber(int min, int max) {					//Создает рандомное число в необходимом диапазоне
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

void shuffleDeck(std::array<Cards, 52> & shDeck) {			//Перетасовывает колоду. Замена каждого элемента массива
	for (int index = 0; index < 52; ++index) {			//на рандомную карту из массива.
		int randCard = static_cast<int>(getRandomNumber(0, 51));
		//std::cout << "\t\t" << randCard << "\n";
		std::swap(shDeck[index], shDeck[randCard]);
	}
}

int getCardValue(const Cards & scoreCard) {				//Возвращает стоимость карты в игре
	switch (scoreCard.rank) {
	case CardNominal::two:		return 2;
	case CardNominal::three:	return 3;
	case CardNominal::four:		return 4;
	case CardNominal::five:		return 5;
	case CardNominal::six:		return 6;
	case CardNominal::seven:	return 7;
	case CardNominal::eight:	return 8;
	case CardNominal::nine:		return 9;
	case CardNominal::ten:		return 10;
	case CardNominal::jack:		return 10;
	case CardNominal::queen:	return 10;
	case CardNominal::king:		return 10;
	case CardNominal::ace:		return 11;
	}
}

void printHand(const std::vector<Cards>& stack)
{
	for (const Cards& element : stack) {
		printCard(element);
		std::cout << "\n";
	}
}

int aceCounter(const Cards &cardPtrPtr) {
	if (getCardValue(cardPtrPtr) == 11)
		return 1;
	else
		return 0;
}

void gameScore(Cards &vCard, int& dValue, std::vector<Cards>& pHand, int& pValue) {			//Интерфейс во время игры. Вторая карта диллера скрыта.
	system("cls");
	printCard(vCard);
	std::cout << "\n";
	std::cout << "Diller's score: " << dValue << "\n\n\n";
	printHand(pHand);
	std::cout << "Your's score: " << pValue << "\n\n\n";
}

void finalScore(std::vector<Cards> &dHand, int &dValue, std::vector<Cards> &pHand, int &pValue) {	//Интерфейс после игры. Карта диллера открыта.
	system("cls");
	printHand(dHand); 
	std::cout << "Diller's score: " << dValue << "\n\n\n";
	printHand(pHand);
	std::cout << "Your's score: " << pValue << "\n\n\n";
}

int playBlackjack(const std::array<Cards, 52> & playDeck)
{
	int index = 0;								//Счётчик программный для перехода по массиву.

	std::vector<Cards> dillerHand;				//Карты на руках у диллера.
	std::vector<Cards> playerHand;				//             и у игрока.

	const Cards* cardPtr = &playDeck[0];		//Создаёт указатель на первый элемент массива std::array
	
	int dillerValue = 0;						//Счётчик диллера.
	int playerValue = 0;						//Счётчик игрока.
	int dillerAces = 0;							//Счётчик тузов диллера.
	int playerAces = 0;							//Счётчик тузов игрока.

	dillerHand.push_back(*cardPtr);
	Cards visionCard = *cardPtr;				//Карта диллера, которую видно в начале игры 
	dillerAces += aceCounter(*cardPtr);
	dillerValue += getCardValue(*cardPtr++);

	dillerHand.push_back(*cardPtr);
	dillerAces += aceCounter(*cardPtr);
	int hiddenDillerValue = getCardValue(*cardPtr++);

	playerHand.push_back(*cardPtr);
	playerAces += aceCounter(*cardPtr);
	playerValue += getCardValue(*cardPtr++);

	playerHand.push_back(*cardPtr);
	playerAces += aceCounter(*cardPtr);
	playerValue += getCardValue(*cardPtr++);

	gameScore(visionCard, dillerValue, playerHand, playerValue);

	do {												//Сборка карт игрока
		if (playerValue > 21) { 						//Если перебор
			if (playerAces < 1) {
				dillerValue += hiddenDillerValue;
				finalScore(dillerHand, dillerValue, playerHand, playerValue);
				return 1;
			}
			else {
				playerAces -= 1;
				playerValue -= 10;
			}
		}

		char choice;
		do {
			gameScore(visionCard, dillerValue, playerHand, playerValue);
			std::cout << "Do you want take a card? (+/-)";	//Взять или оставить.
			std::cin >> choice;
			gameScore(visionCard, dillerValue, playerHand, playerValue);
		} while (choice != '+' && choice != '-');

		if (choice == '+') {
			playerHand.push_back(*cardPtr);
			playerAces += aceCounter(*cardPtr);
			playerValue += getCardValue(*cardPtr++);
			gameScore(visionCard, dillerValue, playerHand, playerValue);
		}
		else
			break;
	} while (true);

	dillerValue += hiddenDillerValue;
	if (dillerValue < 17) {
		do {											//Сборка карт диллера
			dillerHand.push_back(*cardPtr);
			dillerAces += aceCounter(*cardPtr);
			dillerValue += getCardValue(*cardPtr++);
			if (dillerValue > 21) {						//Если перебор
				if (dillerAces < 1) {
					finalScore(dillerHand, dillerValue, playerHand, playerValue);
					return 0;
				}
				else {
					dillerAces -= 1;
					dillerValue -= 10;
				}
			}
		} while (dillerValue < 17);
	}
	finalScore (dillerHand, dillerValue, playerHand, playerValue);
	
	if (playerValue > dillerValue)						// Финальные условия победы.
		return 0;
	else
		if (playerValue == dillerValue)
			return 2;
		else
			return 1;
}

int main() {
	char round;
	do{
	srand(static_cast<unsigned int>(time(0)));			//Создаёт рандомное число, основанное на часах Windows.
	rand();												//Фикс бага VC при котором первое рандомное число статично.
	std::array<Cards, 52> deck;							//Объявление карточного массива

	int index = 0;										//Инициализация массива
	for (int cardShirt = 0; cardShirt < static_cast<int>(CardDeck::max); ++cardShirt) {		// 
		for (int cardRank = 0; cardRank < static_cast<int>(CardNominal::max); ++cardRank) {		//
			deck[index].shirt = static_cast<CardDeck>(cardShirt);					//	
			deck[index].rank = static_cast<CardNominal>(cardRank);					//	
			++index;										//	
		}
	}

	shuffleDeck(deck);	// Перетасовка

	switch (playBlackjack(deck)) {
	case 0:		std::cout << "You are winner!" << std::endl;		break;
	case 1:		std::cout << "Diller is winner!" << std::endl;		break;
	default:	std::cout << "Dead heat!" << std::endl;				break;
	}
	system("pause");

	system("cls");
	std::cout << "Another game? (+/-) " << std::endl;
	std::cin >> round;
	}while (round == '+');
	system("pause");
	return 0;
}
