#pragma once
#include <iostream>
#include <random>
#include <utility> // pair
#include <time.h>

class Player
{
public:
	Player(int number);
	Player(int number, int agility, int fearlessness);
	~Player() {};
	virtual bool hasEnoughMarbles() { return false; };
	virtual bool act() { return true; };
	virtual bool act2() { return true; }; //marble game 에서 사용
	virtual void checkMarble() { };// marble game에서 사용
	virtual bool isPlaying() { return playing; };
	void aliveMessage() { printStatus(); std::cout << " is alive." << std::endl; };
	virtual void dyingMessage() { printStatus(); std::cout << " died." << std::endl; };
	virtual void printStatus() { std::cout << "Player #" << number << "(" << agility << "," << fearlessness << ")"; };
protected:
	unsigned int number;
	unsigned int agility;
	unsigned int fearlessness;
	bool playing = true;
	static std::default_random_engine random_engine;
	static std::uniform_int_distribution<unsigned int> ability_range;

	static std::uniform_real_distribution<float> possibility;
};



class PlayerRLGL : public Player
{
	//랜덤 거리 추가를 위해
	static std::uniform_int_distribution<unsigned int> rand_dis;
	static float fallDownRate;
	unsigned int current_distance = 0;
public:
	PlayerRLGL(const Player& player) : Player(player) { playing = true; };
	bool act();
	void dyingMessage();
};

class PlayerRPS : public Player
{
public:
	PlayerRPS(const Player& player) : Player(player) {  };
	bool act();
	void dyingMessage();
};

// 구슬 게임 참가자 class
class PlayerMarbleGame : public Player
{

public:
	PlayerMarbleGame(const Player& player) : Player(player) { playing = true; }; 
	virtual bool act2() { return false; }; // 플레이어 2명이서 하는 게임이라, player1, player2의 act를
										   // 각각 정의해주어야 하는 경우가 있어 act2()함수를 가상함수로 만들어줌

	virtual void checkMarble() {}; // 각각의 게임에서 구슬 체크 함수 구현
	void dyingMessage(); // 죽음 메세지 출력 함수
};

//홀짝 게임 player class
class PlayerOddOrEven : public PlayerMarbleGame
{
	//구슬의 개수를 고르는, 그리고 구슬의 개수를 예상하는 random함수 2개 생성
	static std::uniform_int_distribution<int>randMarble_choose;
	static std::uniform_int_distribution<int>randMarble_expect;
	//홀짝 판별과, 개수 판별을 위한 bool 멤버 선언
	bool check_OddOrEven = false;
	bool check_num = false;
public:
	std::pair< int, int> player1_marble;  // player 1 의 구슬 저장 pair < 홀짝 구분 , 구슬 개수>
	std::pair< int, int> player2_marble; // player 1 의 구슬 저장 pair < 홀짝 구분 , 구슬 개수>
	void CompareOddorEven(const std::pair<int, int>& player1_marble, const std::pair<int, int>& player2_marble); // 홀짝 구분 함수 구현
	bool hasEnoughMarbles() { return this->current_marble >= 40; }; // 게임 통과 체크를 위한 함수
	int current_marble = 20; // 현재 구슬 개수
	PlayerOddOrEven(const Player& player) : PlayerMarbleGame(player) { playing = true; };
	bool act(); // player1 행동 함수
	bool act2(); // player2 행동 함수 
	void checkMarble(); // 구슬 개수 확인 및 재분배 함수
	void dyingMessage(); // 죽음 메세지 출력 함수

};

//구슬 넣기 게임 player class
class PlayerHole_In_One : public PlayerMarbleGame
{
	static std::uniform_int_distribution<int>randHole_distance; // 구멍까지의 거리를 생성하는 random함수 생성
public:
	int current_marble = 10; // 구슬이 구멍에 들어가기만 하면 무조건 종료이므로, 구슬의 개수가 턴 수라고 볼 수 있음.
	PlayerHole_In_One(const Player& player) : PlayerMarbleGame(player) { playing = true; };
	bool hasEnoughMarbles() { return this->current_marble > 0; };
	bool act(); // 행동 함수
	void dyingMessage(); // 죽음 메세지 출력 함수

};

//class PlayerSugarHoneycombToffee : public Player
//{
//	//달고나 뽑기 게임에서 매우 드문 확률로 라이터 아이템 획득 가능성을 추가 - 라이터로 진행할 수 있는 진척도를 random으로 부여
//	static std::uniform_int_distribution<unsigned int> rand_Torch;
//	static float ShakingHandRate;
//	unsigned int current_progeress_rate = 0;
//
//public:
//	PlayerSugarHoneycombToffee(const Player& player) : Player(player) {};
//	~PlayerSugarHoneycombToffee() {};
//	bool act();
//	void dyingMessage();
//};