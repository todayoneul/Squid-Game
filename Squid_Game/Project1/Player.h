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


class PlayerMarbleGame : public Player
{

public:
	std::pair< int, int> player1_marble; //Odd Or Even 뿐만 아니라 Hole In One 게임에서 동일한 매커니즘을 사용하므로 정의 class를 marbleGame으로 이동 필요
	std::pair< int, int> player2_marble;
	PlayerMarbleGame(const Player& player) : Player(player) { playing = true; };
	virtual bool act2() { return false; }; // 플레이어 2명이서 하는 게임이라, player1, player2의 act를 각각 정의해주기 위해 act2()함수를 순수 가상함수로 만들어줌
	virtual void checkMarble() = 0;
	void CompareOddorEven(const std::pair<int, int>& player1_marble, const std::pair<int, int>& player2_marble);
	bool check_OddOrEven = false;
	bool check_num = false;
	void dyingMessage();

};

class PlayerOddOrEven : public PlayerMarbleGame
{
	//구슬의 개수를 고르는, 그리고 구슬의 개수를 예상하는 random함수를 생성
	static std::uniform_int_distribution<int>randMarble_choose;
	static std::uniform_int_distribution<int>randMarble_expect;
public:
	bool hasEnoughMarbles() { return this->current_marble >= 40; };
	int current_marble = 20;
	PlayerOddOrEven(const Player& player) : PlayerMarbleGame(player) { playing = true; };
	bool act();
	bool act2();
	void checkMarble();
	void dyingMessage();
	
};


class PlayerHole_In_One : public PlayerMarbleGame
{
	static std::uniform_int_distribution<int>randHole_distance; // 구멍까지의 거리를 생성하는 random함수 생성
public:
	int current_marble = 10; // 결국 한번 들어가면 종료이므로, 구슬의 개수가 턴 수라고 볼 수 있음.
	PlayerHole_In_One(const Player& player) : PlayerMarbleGame(player) { playing = true; };
	bool hasEnoughMarbles() { return this->current_marble >0; };
	bool act();
	void checkMarble();
	void dyingMessage();

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