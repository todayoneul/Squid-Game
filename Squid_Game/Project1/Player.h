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
	virtual bool act2() { return true; }; //marble game ���� ���
	virtual void checkMarble() { };// marble game���� ���
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
	//���� �Ÿ� �߰��� ����
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
	std::pair< int, int> player1_marble; //Odd Or Even �Ӹ� �ƴ϶� Hole In One ���ӿ��� ������ ��Ŀ������ ����ϹǷ� ���� class�� marbleGame���� �̵� �ʿ�
	std::pair< int, int> player2_marble;
	PlayerMarbleGame(const Player& player) : Player(player) { playing = true; };
	virtual bool act2() { return false; }; // �÷��̾� 2���̼� �ϴ� �����̶�, player1, player2�� act�� ���� �������ֱ� ���� act2()�Լ��� ���� �����Լ��� �������
	virtual void checkMarble() = 0;
	void CompareOddorEven(const std::pair<int, int>& player1_marble, const std::pair<int, int>& player2_marble);
	bool check_OddOrEven = false;
	bool check_num = false;
	void dyingMessage();

};

class PlayerOddOrEven : public PlayerMarbleGame
{
	//������ ������ ����, �׸��� ������ ������ �����ϴ� random�Լ��� ����
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
	static std::uniform_int_distribution<int>randHole_distance; // ���۱����� �Ÿ��� �����ϴ� random�Լ� ����
public:
	int current_marble = 10; // �ᱹ �ѹ� ���� �����̹Ƿ�, ������ ������ �� ����� �� �� ����.
	PlayerHole_In_One(const Player& player) : PlayerMarbleGame(player) { playing = true; };
	bool hasEnoughMarbles() { return this->current_marble >0; };
	bool act();
	void checkMarble();
	void dyingMessage();

};

//class PlayerSugarHoneycombToffee : public Player
//{
//	//�ް� �̱� ���ӿ��� �ſ� �幮 Ȯ���� ������ ������ ȹ�� ���ɼ��� �߰� - �����ͷ� ������ �� �ִ� ��ô���� random���� �ο�
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