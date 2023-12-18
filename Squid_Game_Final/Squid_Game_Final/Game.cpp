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
	for (int t = 0; t < turn; ++t) // 턴 수만큼 반복
	{
		std::cout << "[Trun #" << t + 1 << "]" << std::endl; // 몇번째 턴인지 출력

		auto player = players.begin(); // 처음부터

		while (player != players.end()) // 끝까지
		{
			if ((*player)->isPlaying() && !(*player)->act()) // players에 있는 player 객체가
															//playing = ture 고, act() 이후 false를 반환한다면
			{
				(*player)->dyingMessage(); // 떨어져 죽었다는 말이니, dyingMessage() 출력
				delete (*player); // 그 player 객체 삭제
				player = players.erase(player); // 객체 삭제했으니 다음 객체를 가르키는 iteraotr 반환
			}
			else
			{
				++player; // 죽지 않았다면 다음 player로 넘어가 act()
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
	auto player1 = players.begin();// 처음 참가자
	auto player2 = std::next(player1); // 바로 그 다음 참가자
	auto lastPlayer = std::prev(players.end()); // 끝 참자가

	std::vector<std::list<Player*>::iterator> playersToRemove; // 삭제할 플레이어를 가르키는 포인터 리스트에 iterator를 vector에 저장

	while (player2 != lastPlayer) {
		bool notPlaying_butAlive = false; // break로 나왔는지 판별하기위해(중간에 나오게되면 playing = false지만, 살아있으므로 x로 처리해줘야 함)
		for (int t = 0; t < 10; ++t) { // 10 턴동안 진행 -> 턴제로 진행할 경우 turn 넣어서 수정
			std::cout << "[Trun #" << t + 1 << "]" << std::endl;
			if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) { // 만약 누군가가 이미 40개 이상의 구슬을 획득했다면 
				notPlaying_butAlive = true; // 즉시 게임 종료
				break;
			}
			if ((t + 1) % 2 == 1) { // 홀수 턴은 player1이 홀짝 고르기 시작

				if ((*player1)->act() && (*player2)->act2()) { //player1이 임의의 구슬을 정하고 player2가 홀짝을 맞춘 경우(이미 player 2의 구슬은 재정비 됨)

					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) { // 누군가 40개 이상의 구슬이 있다면
						notPlaying_butAlive = true; // 게임 종료
						break;
					}
					(*player1)->checkMarble();//player1의 구슬 개수 재정비 및 개수 확인
				}
				else { //act2()가 false라면 예상이 틀린 것이니 구슬을 잃을 수밖에 없음

					(*player2)->checkMarble(); // player2의 구슬 개수 재정비 밋 개수 확인
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) { // 만약 checkMarble()함수로 인해 구슬 재정비로 누군가가 탈락하게 된다면
					notPlaying_butAlive = true; // 게임 종료
					break;
				}
			}
			else {// 짝수 턴에는 player2가 홀짝 고르기

				if ((*player2)->act() && (*player1)->act2()) { // 위와 마찬가지지만 순서만 다름
					if ((*player1)->hasEnoughMarbles() || (*player2)->hasEnoughMarbles()) {
						notPlaying_butAlive = true;
						break;
					}
					(*player2)->checkMarble();
				}
				else { // 짝수턴에 player1의 act2()가 false라면 예상이 틀린 것이고 구슬을 잃을 수밖에 없음
					(*player1)->checkMarble(); // player1의 구슬 개수 재정비 밋 개수 확인
				}
				if (!(*player1)->isPlaying() || !(*player2)->isPlaying()) {
					notPlaying_butAlive = true;
					break;
				}
			}
		}

		// 10번의 턴이 끝나고 난 뒤
		std::cout << "[[Game Over]]" << std::endl;
		//10번의 턴 모두 사용하였고 player1과 player2 모두 서로의 구슬을 모두 빼앗지 못하면 죽음
		if ((((*player1)->isPlaying() && (*player2)->isPlaying())) && !notPlaying_butAlive) {
			(*player1)->dyingMessage();
			(*player2)->dyingMessage();
			playersToRemove.push_back(player1);
			playersToRemove.push_back(player2);
		}
		//break문으로 밖으로 나왔을 경우
		else if (notPlaying_butAlive && (*player1)->hasEnoughMarbles()) { //player1이 구슬 40개 이상으로 통과

			(*player2)->dyingMessage();
			playersToRemove.push_back(player2);
		}
		else if (notPlaying_butAlive && (*player2)->hasEnoughMarbles()) {// player2가 구슬 40개 이상으로 통과
			(*player1)->dyingMessage();
			playersToRemove.push_back(player1);
		}


		//다음 두명의 player들로 초기화
		player1 = std::next(player2);
		if (player1 == lastPlayer)// 만약 player1의 다음 player가 존재하지 않는다면 깍두기로 부전승 시켜줌
			break;
		else
			player2 = std::next(player1);

	}

	if (!playersToRemove.empty()) {//게임에서 탈락한 사람이 존재하면
		for (auto& it : playersToRemove) {
			delete* it; // playersToRemove에는 Player객체를 가르키는 포인터가 존재하니, 그 포인터의 원본 삭제
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
		static bool check; // 성공해 살아남은 사람 check
		std::cout << "\n[Throw Marble]" << std::endl;

		while (true) { // 구슬을 다 굴리거나 구슬을 구멍 안에 넣을 때까지 반복
			bool check_player1_success = (*player1)->act();
			bool check_player2_success = (*player2)->act();

			if (check_player1_success) { // 만약 선공인 player1 이 성공한다면
				check = false; // 성공한 사람 check
				(*player1)->printStatus(); // player 1 성공 메세지 출력
				std::cout << " safely escaped from the Hole In One Game." << std::endl;
				break;
			}
			else if (check_player2_success) { // player 2가 성공한다면
				check = true; // 성공한 사람 check
				(*player2)->printStatus(); //player 2 성공 메세지 출력
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