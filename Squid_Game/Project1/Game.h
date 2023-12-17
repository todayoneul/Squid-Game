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
	// ~Game();// 수정
	virtual ~Game()=0;
	virtual void join(Player* player) = 0;
	virtual void play() = 0;
	void printAlivePlayers();
	int getGameType(); // marble game 에서  game 고르기 위해 사용
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


//구슬게임

// 게임이 시작되면 살아남은 모든 참가자들에게 구슬 20개 부여
// 관리자(혹은 참가번호 다음 순서)와 1대 1 게임 or 구현 가능하다면 1대 1 대상은 랜덤
// 각 턴마다 player들은 게임 1,2중 선택 가능 - 게임의 선택은 프로그램이 정해줌(랜덤)

// 만약 구슬 20개를 모아 상대방의 구슬을 모두 없앤다면 그 즉시 게임 종료

// 게임 1. 홀짝게임 (OddOrEven)
//		게임은 단 2명씩 짝을 지어서, 턴은 10턴으로 제한 (인원이 홀수일 경우 맨 마지막 인원은 깍두기)
// 		후공인 자는 가지고 있는 구슬의 개수 중 임의의 개수를 선택
// 		선공은 선택한 구슬이 짝수인지 홀수인지 맞춰야 함
// 		맞추면 일정량의 구슬을 상대방으로 부터 가져옴 / 틀리면 일정량 상대방에게 헌납
// 		모든 턴이 종로되고도 최종승자가 결정되지 않으면 두명 모두 탈락
//		player의 fearlessness가 낮을 수록 더욱 안전한 플레이 -> 구슬을 조금씩만 선택하고, 구슬의 예상 범위도 작음

// 게임 2.구슬 구멍에 넣기 (Hole_In_One)
// 		구슬을 넣는 순간 게임 종료
// 		1턴마다 각 플레이어들은 구슬을 구멍에 던질 수 있음.
// 		구슬을 구멍에 넣는다면 그때까지 땅에 있는 모든 구슬을 가져옴.
// 		모든 턴이 종료되고도 최종 승자가 결정되지 않으면 두명 모두 탈락
//		player의 agility가 클수록 구멍에 구슬을 넣을 확률 + 
//		다만, player의 fearlessnesss가 높다면 구멍에 구슬을 넣을 확률 -


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
//	static const float break_rate; // player마다 다르게 할당
//
//public:
//	SugarHoneycombToffee() : Game("Sugar Honeycomb Toffee game") {};
//	~SugarHoneycombToffee() {};
//	void join(Player* player);
//	void play();
//};
