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
	// 내가 구현해야 할 곳
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

//Marble game에서 홀짝 게임을 진행할지, 구슬 넣기 게임을 진행할지 랜덤으로 선택해주는 함수 다른 class에서도 이용할 수 있도록 추상 class인 Game에 구현
int Game::getGameType()
{
	std::default_random_engine random_engine(time(nullptr));
	std::uniform_int_distribution<unsigned int> Choose_game(0, 1);
	unsigned int gameNum = Choose_game(random_engine);
	return gameNum;
};

//구슬 게임 - 홀짝게임 join함수
void OddOrEven::join(Player* player)
{
	players.push_back(new PlayerOddOrEven(*player));
}

void OddOrEven::play() {
	printGameName();
	//두명이 짝을 이뤄 진행하는 게임이므로 두개의 iterator 선언
	auto player1 = players.begin();
	auto player2 = std::next(player1);
	auto lastPlayer = std::prev(players.end());

	std::vector<std::list<Player*>::iterator> playersToRemove; // 삭제할 플레이어의 iterator 저장

	while (player2 != lastPlayer) {
		bool notPlaying_butAlive = false; // break로 나왔는지 판별하기위해(중간에 나오게되면 playing = false지만, 살아있으므로 땨로 처리해줘야 함)
		for (int t = 0; t < 10; ++t) {
			std::cout << "[Trun #" << t + 1 << "]" << std::endl;
			if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
				notPlaying_butAlive = true;
				break;
			}
			if ((t + 1) % 2 == 1) { // 홀수 턴은 player1이 홀짝 고르기 시작

				if ((*player1)->act() && (*player2)->act2()) { //player1이 임의의 구슬을 정하고 player2가 홀짝을 맞춘 경우
					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
						notPlaying_butAlive = true;
						break;
					}
					(*player1)->checkMarble();//player1의 구슬 개수 확인
				}
				else { //act2()가 false라면 예상이 틀린 것이니 구슬을 잃을 수밖에 없음
					(*player2)->checkMarble(); // player2의 구슬 개수 재정비 밋 개수 확인
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) { // 
					notPlaying_butAlive = true;
					break;
				}
			}
			else {// 짝수 턴에는 player2가 홀짝 고르기

				if ((*player2)->act() && (*player1)->act2()) {
					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
						notPlaying_butAlive = true;
						break;
					}
					(*player2)->checkMarble();
				}
				else { // 짝수턴에 player1의 act2()가 false라면 예상이 틀린 것이고 구슬을 잃을 수밖에 없음
					(*player1)->checkMarble(); // player1의 구슬 개수 재정비 밋 개수 확인
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) { // 
					notPlaying_butAlive = true;
					break;
				}
			}
		}
		
		// 10번의 턴이 끝나고 난 뒤
		std::cout << "[[Game Over]]" << std::endl;

		if ( (((*player1)->isPlaying() && (*player2)->isPlaying()))&& !notPlaying_butAlive) { // player1과 player2 모두 서로의 구슬을 모두 빼앗지 못하면 죽음
			(*player1)->dyingMessage();
			(*player2)->dyingMessage();
			playersToRemove.push_back(player1);
			playersToRemove.push_back(player2);
		}
		//break문으로 밖으로 나왔을 경우
		else if (notPlaying_butAlive &&(*player1)->hasEnoughMarbles()) { //player1은 구슬 40개 이상으로 통과
			
			(*player2)->dyingMessage();
			playersToRemove.push_back(player2);
		}
		else if (notPlaying_butAlive && (*player2)->hasEnoughMarbles()) {// player2는 구슬 40개 이상으로 통과
			(*player1)->dyingMessage();
			playersToRemove.push_back(player1);
		}


		//다음 두명의 player들로 초기화
		player1 = std::next(player2);
		if (player1 == lastPlayer)
			break;
		else
			player2 = std::next(player1);
		
	}
	
	if (!playersToRemove.empty()) {//게임에서 탈락한 사람이 없지 않다면
		for (auto& it : playersToRemove) {
			delete* it; // 그 객체 삭제
			it = players.erase(it);
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive After Odd or Even Game." << std::endl << std::endl;
}






//구슬 게임 - 구슬 넣기 게임 join함수
void Hole_In_One::join(Player* player)
{
	players.push_back(new PlayerHole_In_One(*player));
}

void Hole_In_One::play()
{
	printGameName();
	//두명이 짝을 이뤄 진행하는 게임이므로 두개의 iterator 선언
	auto player1 = players.begin();
	auto player2 = std::next(player1);
	auto lastPlayer = std::prev(players.end());

	std::vector<std::list<Player*>::iterator> playersToRemove; // 삭제할 플레이어의 iterator 저장

	while (player2 != lastPlayer) {
		bool check_marble_empty = false;
		std::cout << "\n[Throw Marble]" << std::endl;
		static bool check;
		while (true) { // 구슬을 다 굴리거나 구슬을 구멍 안에 넣을 때까지 반복
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
		
		//다음 두명의 player들로 초기화
		player1 = std::next(player2);
		if (player1 == lastPlayer)
			break;
		else
			player2 = std::next(player1);
		
	}
	
	if (!playersToRemove.empty()) {//게임에서 탈락한 사람이 없지 않다면
		for (auto& it : playersToRemove) {
			delete* it; // 그 객체 삭제
			it = players.erase(it);
		}
	}

	printAlivePlayers();

	std::cout << players.size() << " players are alive After Hole In One game." << std::endl << std::endl;
}
