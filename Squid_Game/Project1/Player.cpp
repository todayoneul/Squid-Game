#include "Player.h"
#include "Game.h"

std::default_random_engine Player::random_engine(time(nullptr));
std::uniform_int_distribution<unsigned int> Player::ability_range(0, 100);
std::uniform_real_distribution<float> Player::possibility(0.f, 1.f);
std::uniform_int_distribution <unsigned int> PlayerRLGL::rand_dis(0, 10);//randomDistance를 위해 추가해줌
// std::uniform_int_distribution<unsigned int> PlayerOddOrEven:: randMarble_choose(0,20);
std::uniform_int_distribution< int> PlayerOddOrEven::randMarble_expect;
std::uniform_int_distribution<int> PlayerHole_In_One::randHole_distance(0,3000);

float PlayerRLGL::fallDownRate = 0.1f;

Player::Player(int number)
{
	this->number = number;

	agility = ability_range(random_engine);
	fearlessness = ability_range(random_engine);
}

Player::Player(int number, int agility, int fearlessness)
{
	this->number = number;
	this->agility = agility;
	this->fearlessness = fearlessness;
}




//내가 구현해야 할 곳
bool PlayerRLGL::act()
{
	//random distance 구현
	unsigned int randomDistance = rand_dis(random_engine);
	
	if (fearlessness > 100 * possibility(random_engine)) {
		//75% 이상의 확률로 fearless_bonus_distance를 받고, 10%의 확률로 죽으며, 나머지 경우엔 둘 다 하지 않음
		auto RLGL = [] {
			float p = possibility(random_engine);
			// p에 0부터 100 사이의 임의의 정수가 생성되고, 이를 확률로 변환기켜주기 위해 
			if (p < PlayerRLGL::fallDownRate)
				return 0;
			else if (p < 0.85f)
				return 1;
			else
				return 2;
			};

		if (RLGL() == 0) {
			 this->playing = false;
			return false;
		}
		else if (RLGL() == 1) {
			//fearless bounus distance 구현
			unsigned int fearless_bounus_distance = this->agility * (this->fearlessness * 0.01);

			current_distance += this->agility + randomDistance + fearless_bounus_distance;
		}
		else {
			current_distance += this->agility + randomDistance;
		}
		
	}
	else {
		current_distance += this->agility + randomDistance;
	}
	if (current_distance >= RedLightGreenLight::distance && isPlaying()) {
		this->playing = false;
		printStatus();
		std::cout << " safely escaped from the ground." << std::endl;
	}

	return true;
}

void PlayerRLGL::dyingMessage()
{
	if (isPlaying())
	{
		printStatus();
		std::cout << " is still on the ground and died." << std::endl;
	}

	else
	{
		printStatus();
		std::cout << " fell down and died." << std::endl;
	}
};





bool PlayerRPS::act()
{
	enum rpsType { Rock, Paper, Scissors };
	auto rps = [] {
		float p = possibility(random_engine);

		if (p < float(1.f / 3.f))
			return rpsType::Rock;
		else if (p < float(2.f / 3.f))
			return rpsType::Paper;
		else
			return  rpsType::Scissors;
		};

	rpsType myRPS, yourRPS;
	do {
		myRPS = rps();
		yourRPS = rps();
	} while (myRPS == yourRPS);

	if (myRPS == Rock)
	{
		if (yourRPS == Paper)
			return false;
		else
			return true;
	}
	else if (myRPS == Paper)
	{
		if (yourRPS == Scissors)
			return false;
		else
			return true;
	}
	else
	{
		if (yourRPS == Rock)
			return false;
		else
			return true;
	}
}


void PlayerRPS::dyingMessage()
{
	printStatus();
	std::cout << " died." << std::endl;
};

// 두 플레이어의 홀짝 예상과, 개수 얘상을 확인하는 함수
void PlayerMarbleGame::CompareOddorEven(const std::pair< int,  int>& p1, const std::pair< int,  int>& p2)
{
	if (p1.first == p2.first && p1.second == p2.second)
	{
		PlayerMarbleGame::check_OddOrEven = true;
		PlayerMarbleGame::check_num = true;
	}
	else if (p1.first == p2.first)
	{
		PlayerMarbleGame::check_OddOrEven = true;
		PlayerMarbleGame::check_num = false;
	}
	else {
		PlayerMarbleGame::check_OddOrEven = false;
		PlayerMarbleGame::check_num = false;
	}
}

//player가 구슬의 개수를 결정하고, 홀짝을 판별하는 함수 얘는 잃을 게 없음 
bool PlayerOddOrEven::act()
{
	int marbleRange = 1 + (Player::fearlessness) / 15;
	std::uniform_int_distribution<unsigned int> randMarble_choose(1, marbleRange);
	int chooseMarble_num = randMarble_choose(random_engine);

	if (chooseMarble_num % 2 == 1)
	{
		player1_marble = std::make_pair(1, chooseMarble_num); //pair의 첫번째 원소에는 홀수면 1, 짝수면 2를, 두번째 원소에는 걸은 원소의 개수를 집어넣는다. 
	}
	else
	{
		player1_marble = std::make_pair(2, chooseMarble_num);
	}
	return true;

};

bool PlayerOddOrEven::act2()
{
	const int known_marble = player1_marble.second;//known_marble을 가지고 player의 agility를 활용할 수 있는 방법

	//agility 크기에 따라 탐색 범위를 줄여주자
	int expect_MarbleRange_start = known_marble - (100-agility) * 0.09; // 이후 비율 조절이 필요해 보임
	int expect_MarbleRange_end = known_marble + (100-agility) * 0.09; 
	//범위 내의 구슬 개수를 랜덤으로 고르게 랜덤 함수를 재정의 해줌

		randMarble_expect = std::uniform_int_distribution<int>(expect_MarbleRange_start , expect_MarbleRange_end);

	int expectedMarble_num = randMarble_expect(random_engine);
	if (expectedMarble_num % 2 == 1)
	{
		player2_marble = std::make_pair(1, expectedMarble_num); //pair의 첫번째 원소에는 홀수면 1, 짝수면 2를, 두번째 원소에는 예상한 원소의 개수를 집어넣는다. 
	}
	else
	{
		player2_marble = std::make_pair(2, expectedMarble_num);
	}
	// plyaer1과 비교 및 구슬 재분배 -> Hole_In_One 게임에서도 동일하게 적용되는 부분
	PlayerMarbleGame::CompareOddorEven(player1_marble, player2_marble);
	if (check_OddOrEven && check_num)
	{
		//홀짝 및 개수 예측까지 성공했을 시, 12개 획득
		this->current_marble += 15;
		if (current_marble >= 40 && isPlaying()) {
			this->playing = false;
			printStatus();
			std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		}
		return true;
	}
	else if (check_OddOrEven && !check_num)
	{
		//홀짝만 맞췄을 경우엔 상대방 것 5개 획득
		this->current_marble += 10;
		if (current_marble >= 40 && isPlaying()) {
			this->playing = false;
			printStatus();
			std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		}
		return true;
	}
	else
	{
		//예상이 틀렸다면, 상대방에게 3개 헌납
		this->current_marble -= 5;
		if (this->current_marble <= 0) {
			this->playing = false;
			//this->dyingMessage();
		}
		return false;
	}

};

//현재 객체(plyer1)에 대해 상대방이 홀짝을 맞췄늦지 확인하고, 만약 current_marble이 40개가 넘어가거나 0보다 적으지면 playing false로 바꿈 
void PlayerOddOrEven::checkMarble() 
{
	//만약 상대방이 맞췄다면 그만큼 뺏겨야 하며, 틀렸다면, 그만큼 얻어야 함
	if (check_OddOrEven && check_num) {
		this->current_marble -= 15;
	}
	else if (check_OddOrEven) {
		this->current_marble -= 10;
	}
	else {
		this->current_marble += 5;
	}
	if (current_marble >= 40 && isPlaying()) {
		printStatus();
		std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		this->playing = false;

	}

	if (this->current_marble <= 0) {
		this->playing = false; 
		//this->dyingMessage();
	}
	else
		this->playing = true;
}

void PlayerMarbleGame::dyingMessage() 
{

		printStatus();
		std::cout << " Game over! died." << std::endl;

}

void PlayerOddOrEven::dyingMessage()
{
	if (isPlaying())
	{
		printStatus();
		std::cout << " Game over! died." << std::endl;
	}

}

bool PlayerHole_In_One::act() // 구슬 굴리기
{
	this->current_marble -= 1;
	int holeDistance = randHole_distance(random_engine);
	int throw_distance_start = holeDistance - (100-agility) * 10; // 이후 비율 조절이 필요해 보임
	int throw_distance_end =  holeDistance + (100-agility) * 10;
	randHole_distance = std::uniform_int_distribution<int>(throw_distance_start, throw_distance_end);
	
	int player_throw_distance = randHole_distance(random_engine);
	if (player_throw_distance >= (holeDistance - 25) && player_throw_distance <= (holeDistance + 25)) { // 일정 구멍 크기 안에 들어가면 성공
		return true;
	}
	if (this->current_marble == 0) {
		return false;
	}
	else {
		return false;
	}
 }



void PlayerHole_In_One::checkMarble() {

}

void PlayerHole_In_One::dyingMessage()
{
	if (isPlaying())
	{
		printStatus();
		std::cout << " failure to insert marbles in hole and died." << std::endl;
	}

	else
	{
		printStatus();
		std::cout << " lost all marbles and died." << std::endl;
	}
}


