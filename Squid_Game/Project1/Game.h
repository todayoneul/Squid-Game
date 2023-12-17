#pragma once
#include <string>
#include <list>

class Player;

class Game
{
protected:
	std::string gameName;
	std::list<Player*> players;
	virtual void printGameName();
public:
	Game(std::string name) :gameName(name) {};
	// ~Game();// ����
	virtual ~Game()=0;
	virtual void join(Player* player) = 0;
	virtual void play() = 0;
	void printAlivePlayers();
	int getGameType(); // marble game ����  game ���� ���� ���
	std::list<Player*> getAlivePlayers() { return players; };
	std::string getGameName() { return gameName; };
	//Player* operator[](int index) { return players[index]; };
};



class RedLightGreenLight : public Game
{
	friend class PlayerRLGL;

	static const unsigned int distance;
	static const float fallDownRate;

	const unsigned int turn = 20;
public:
	RedLightGreenLight() : Game("Red Light Green Light") {};
	RedLightGreenLight(int t) : Game("Red Light Green Light"), turn(t) {};
	~RedLightGreenLight() {};
	void join(Player* player);
	void play();
};


class RPS : public Game
{
	friend class PlayerRPS;

public:
	RPS() : Game("Rock Paper Scissors") {};
	~RPS() {};
	void join(Player* player);
	void play();
};


//��������

// ������ ���۵Ǹ� ��Ƴ��� ��� �����ڵ鿡�� ���� 20�� �ο�
// ������(Ȥ�� ������ȣ ���� ����)�� 1�� 1 ���� or ���� �����ϴٸ� 1�� 1 ����� ����
// �� �ϸ��� player���� ���� 1,2�� ���� ���� - ������ ������ ���α׷��� ������(����)

// ���� ���� 20���� ��� ������ ������ ��� ���شٸ� �� ��� ���� ����

// ���� 1. Ȧ¦���� (OddOrEven)
//		������ �� 2�� ¦�� ���, ���� 10������ ���� (�ο��� Ȧ���� ��� �� ������ �ο��� ��α�)
// 		�İ��� �ڴ� ������ �ִ� ������ ���� �� ������ ������ ����
// 		������ ������ ������ ¦������ Ȧ������ ����� ��
// 		���߸� �������� ������ �������� ���� ������ / Ʋ���� ������ ���濡�� �峳
// 		��� ���� ���εǰ� �������ڰ� �������� ������ �θ� ��� Ż��
//		player�� fearlessness�� ���� ���� ���� ������ �÷��� -> ������ ���ݾ��� �����ϰ�, ������ ���� ������ ����

// ���� 2.���� ���ۿ� �ֱ� (Hole_In_One)
// 		������ �ִ� ���� ���� ����
// 		1�ϸ��� �� �÷��̾���� ������ ���ۿ� ���� �� ����.
// 		������ ���ۿ� �ִ´ٸ� �׶����� ���� �ִ� ��� ������ ������.
// 		��� ���� ����ǰ� ���� ���ڰ� �������� ������ �θ� ��� Ż��
//		player�� agility�� Ŭ���� ���ۿ� ������ ���� Ȯ�� + 
//		�ٸ�, player�� fearlessnesss�� ���ٸ� ���ۿ� ������ ���� Ȯ�� -


class MarbleGame : public Game
{
	friend class PlayerMarbleGame;
protected:
	static unsigned int turn;
public:
	MarbleGame() : Game("Marble Game") {};
	~MarbleGame() {};
	void join(Player* player) {};
	void play() {};
};



class OddOrEven : public MarbleGame
{
public:
	OddOrEven() { gameName = "Odd or Even"; };
	OddOrEven(unsigned int t)
	{
		gameName = "Odd or Even";

	};
	~OddOrEven() {};
	void join(Player* player);
	void play();
};

class Hole_In_One : public MarbleGame
{
public:
	Hole_In_One() { gameName = "Hole In One!"; };
	Hole_In_One(unsigned int t)
	{
		gameName = "Hole In One!";
	};
	~Hole_In_One() {};
	void join(Player* player);
	void play();
};
//
//class SugarHoneycombToffee : public Game
//{
//	friend class PlayerSugarHoneycombToffee;
//private:
//	const unsigned int cutting_len_circle = 500;
//	const unsigned int cutting_len_star = 780;
//	const unsigned int cutting_len_umbrellla = 1800;
//	static const float break_rate; // player���� �ٸ��� �Ҵ�
//
//public:
//	SugarHoneycombToffee() : Game("Sugar Honeycomb Toffee game") {};
//	~SugarHoneycombToffee() {};
//	void join(Player* player);
//	void play();
//};
