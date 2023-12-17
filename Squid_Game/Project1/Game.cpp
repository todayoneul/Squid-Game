#include <iostream>
#include "Game.h"
#include "Player.h"

Game::~Game()
{
	for (auto player = players.begin(); player != players.end(); ++player)
	{
		delete (*player);
	}
	players.clear();
}

void Game::printGameName()
{
	std::cout << "[[[" << gameName << "]]]" << std::endl;
}

void Game::printAlivePlayers()
{
	for (auto player = players.begin(); player != players.end(); ++player)
	{
		(*player)->aliveMessage();
	}
}

const unsigned int RedLightGreenLight::distance = 1000;
const float RedLightGreenLight::fallDownRate = 0.1f;



void RedLightGreenLight::join(Player* player)
{
	players.push_back(new PlayerRLGL(*player));
}

void RedLightGreenLight::play()
{
	printGameName();
	// ���� �����ؾ� �� ��
	for (int t = 0; t < turn; ++t)
	{
		std::cout << "[Trun #" << t + 1 << "]" << std::endl;

		auto player = players.begin();

		while (player != players.end())
		{
			if ((*player)->isPlaying() && !(*player)->act())
			{
				(*player)->dyingMessage();
				delete (*player);
				player = players.erase(player);
			}
			else
			{
				++player;
			}
		}

	}

	std::cout << "[Game Over]" << std::endl;

	auto player = players.begin();
	while (player != players.end())
	{
		if ((*player)->isPlaying())
		{
			(*player)->dyingMessage();
			delete (*player);
			player = players.erase(player);
		}
		else
		{
			++player;
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive." << std::endl << std::endl;
}





void RPS::join(Player* player)
{
	players.push_back(new PlayerRPS(*player));
}

void RPS::play()
{
	printGameName();

	if (players.size() < 2)
	{
		std::cout << "There are not sufficient players.";
		return;
	}

	auto player = players.begin();

	while (player != players.end())
	{
		if ((*player)->act())
		{
			++player;
		}
		else
		{
			(*player)->dyingMessage();
			delete (*player);
			player = players.erase(player);
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive." << std::endl << std::endl;
}

//Marble game���� Ȧ¦ ������ ��������, ���� �ֱ� ������ �������� �������� �������ִ� �Լ� �ٸ� class������ �̿��� �� �ֵ��� �߻� class�� Game�� ����
int Game::getGameType()
{
	std::default_random_engine random_engine(time(nullptr));
	std::uniform_int_distribution<unsigned int> Choose_game(0, 1);
	unsigned int gameNum = Choose_game(random_engine);
	return gameNum;
};

//���� ���� - Ȧ¦���� join�Լ�
void OddOrEven::join(Player* player)
{
	players.push_back(new PlayerOddOrEven(*player));
}

void OddOrEven::play() {
	printGameName();
	//�θ��� ¦�� �̷� �����ϴ� �����̹Ƿ� �ΰ��� iterator ����
	auto player1 = players.begin();
	auto player2 = std::next(player1);
	auto lastPlayer = std::prev(players.end());

	std::vector<std::list<Player*>::iterator> playersToRemove; // ������ �÷��̾��� iterator ����

	while (player2 != lastPlayer) {
		bool notPlaying_butAlive = false; // break�� ���Դ��� �Ǻ��ϱ�����(�߰��� �����ԵǸ� playing = false����, ��������Ƿ� �x�� ó������� ��)
		for (int t = 0; t < 10; ++t) {
			std::cout << "[Trun #" << t + 1 << "]" << std::endl;
			if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
				notPlaying_butAlive = true;
				break;
			}
			if ((t + 1) % 2 == 1) { // Ȧ�� ���� player1�� Ȧ¦ ���� ����

				if ((*player1)->act() && (*player2)->act2()) { //player1�� ������ ������ ���ϰ� player2�� Ȧ¦�� ���� ���
					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
						notPlaying_butAlive = true;
						break;
					}
					(*player1)->checkMarble();//player1�� ���� ���� Ȯ��
				}
				else { //act2()�� false��� ������ Ʋ�� ���̴� ������ ���� ���ۿ� ����
					(*player2)->checkMarble(); // player2�� ���� ���� ������ �� ���� Ȯ��
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) { // 
					notPlaying_butAlive = true;
					break;
				}
			}
			else {// ¦�� �Ͽ��� player2�� Ȧ¦ ����

				if ((*player2)->act() && (*player1)->act2()) {
					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
						notPlaying_butAlive = true;
						break;
					}
					(*player2)->checkMarble();
				}
				else { // ¦���Ͽ� player1�� act2()�� false��� ������ Ʋ�� ���̰� ������ ���� ���ۿ� ����
					(*player1)->checkMarble(); // player1�� ���� ���� ������ �� ���� Ȯ��
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) { // 
					notPlaying_butAlive = true;
					break;
				}
			}
		}
		
		// 10���� ���� ������ �� ��
		std::cout << "[[Game Over]]" << std::endl;

		if ( (((*player1)->isPlaying() && (*player2)->isPlaying()))&& !notPlaying_butAlive) { // player1�� player2 ��� ������ ������ ��� ������ ���ϸ� ����
			(*player1)->dyingMessage();
			(*player2)->dyingMessage();
			playersToRemove.push_back(player1);
			playersToRemove.push_back(player2);
		}
		//break������ ������ ������ ���
		else if (notPlaying_butAlive &&(*player1)->hasEnoughMarbles()) { //player1�� ���� 40�� �̻����� ���
			
			(*player2)->dyingMessage();
			playersToRemove.push_back(player2);
		}
		else if (notPlaying_butAlive && (*player2)->hasEnoughMarbles()) {// player2�� ���� 40�� �̻����� ���
			(*player1)->dyingMessage();
			playersToRemove.push_back(player1);
		}


		//���� �θ��� player��� �ʱ�ȭ
		player1 = std::next(player2);
		if (player1 == lastPlayer)
			break;
		else
			player2 = std::next(player1);
		
	}
	
	if (!playersToRemove.empty()) {//���ӿ��� Ż���� ����� ���� �ʴٸ�
		for (auto& it : playersToRemove) {
			delete* it; // �� ��ü ����
			it = players.erase(it);
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive After Odd or Even Game." << std::endl << std::endl;
}






//���� ���� - ���� �ֱ� ���� join�Լ�
void Hole_In_One::join(Player* player)
{
	players.push_back(new PlayerHole_In_One(*player));
}

void Hole_In_One::play()
{
	printGameName();
	//�θ��� ¦�� �̷� �����ϴ� �����̹Ƿ� �ΰ��� iterator ����
	auto player1 = players.begin();
	auto player2 = std::next(player1);
	auto lastPlayer = std::prev(players.end());

	std::vector<std::list<Player*>::iterator> playersToRemove; // ������ �÷��̾��� iterator ����

	while (player2 != lastPlayer) {
		bool check_marble_empty = false;
		std::cout << "\n[Throw Marble]" << std::endl;
		static bool check;
		while (true) { // ������ �� �����ų� ������ ���� �ȿ� ���� ������ �ݺ�
			bool check_player1_success = (*player1)->act();
			bool check_player2_success = (*player2)->act();

			if (check_player1_success && check_player2_success) {
				std::cout << "Draw : Regame" << std::endl;
				continue;
			}
			else if (check_player1_success) {
				check = false;
				(*player1)->printStatus();
				std::cout << " safely escaped from the Hole In One Game." << std::endl;
				break;
			}
			else if (check_player2_success) {
				check = true;
				(*player2)->printStatus();
				std::cout << " safely escaped from the Hole In One Game." << std::endl;
				break;
			}
			else if (!(*player1)->hasEnoughMarbles() || !(*player2)->hasEnoughMarbles()) {
				check_marble_empty = true;
				break;
			}

		}
		std::cout << "[Round Over]" << std::endl;
		if (check_marble_empty) {
			(*player1)->dyingMessage();
			(*player2)->dyingMessage();
			playersToRemove.push_back(player1);
			playersToRemove.push_back(player2);
		}
		else if (!check) {
			(*player2)->dyingMessage();
			playersToRemove.push_back(player2);

		}
		else if (check) {
			(*player1)->dyingMessage();
			playersToRemove.push_back(player1);

		}
		
		//���� �θ��� player��� �ʱ�ȭ
		player1 = std::next(player2);
		if (player1 == lastPlayer)
			break;
		else
			player2 = std::next(player1);
		
	}
	
	if (!playersToRemove.empty()) {//���ӿ��� Ż���� ����� ���� �ʴٸ�
		for (auto& it : playersToRemove) {
			delete* it; // �� ��ü ����
			it = players.erase(it);
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive After Hole In One game." << std::endl << std::endl;
}
