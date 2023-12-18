#include "Player.h"
#include "Game.h"

std::default_random_engine Player::random_engine(time(nullptr));
std::uniform_int_distribution<unsigned int> Player::ability_range(0, 100);
std::uniform_real_distribution<float> Player::possibility(0.f, 1.f);
std::uniform_int_distribution <unsigned int> PlayerRLGL::rand_dis(0, 10);//randomDistance를 위해 추가해줌
// std::uniform_int_distribution<unsigned int> PlayerOddOrEven:: randMarble_choose(0,20);
std::uniform_int_distribution< int> PlayerOddOrEven::randMarble_expect;
std::uniform_int_distribution<int> PlayerHole_In_One::randHole_distance(0, 3000);

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
	// fearlessness가 높으면 일정 확률 이상으로 bonus distance 얻을 기회가 주어짐
	if (fearlessness > 100 * possibility(random_engine)) {
		//75% 이상의 확률로 fearless_bonus_distance를 받고, 10%의 확률로 죽으며, 나머지 경우엔 둘 다 하지 않음
		auto RLGL = [] { // 임의의 확률로 0,1,2를 반환하는 lambda 함수
			float p = possibility(random_engine);
			// p에 0부터 100 사이의 임의의 정수가 생성되고, 이를 확률로 변환기켜주기 위해 
			if (p < PlayerRLGL::fallDownRate) // 10퍼센트의 확률로 떨어져 죽음
				return 0;
			else if (p < 0.85f) // 75퍼센트 확률로 bonus distance 얻음
				return 1;
			else // 나머지 경우엔 아무것도 안함
				return 2;
		};

		if (RLGL() == 0) { // 10 퍼센트의 경우 떨어져 죽기 때문에 playing을 false로 바꿔주고 return false
			this->playing = false;
			return false;
		}
		else if (RLGL() == 1) { // 75퍼센트의 확률로 bonus distance 얻었을 경우 계산해서 현재 거리에 더해주기
			//fearless bounus distance 구현
			unsigned int fearless_bounus_distance = this->agility * (this->fearlessness * 0.01);

			current_distance += this->agility + randomDistance + fearless_bounus_distance;
		}
		else { // 나머지 경우엔 agility + randomdistance만 계산
			current_distance += this->agility + randomDistance;
		}

	}
	else {// 나머지 경우엔 agility + randomdistance만 계산
		current_distance += this->agility + randomDistance;
	}
	// 만약 경기장 크기보다 많이 움직이고, play중이라면
	if (current_distance >= RedLightGreenLight::distance && isPlaying()) { 
		this->playing = false; // playing을 false로 바꾸고 탈출 메세지 출력
		printStatus();
		std::cout << " safely escaped from the ground." << std::endl;
	}

	return true; // 떨어져서 죽지 않는 한, true 반환
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
void PlayerOddOrEven::CompareOddorEven(const std::pair<int,int>& p1, const std::pair<int,int>& p2)
{   //.first가 홀짝 판단, .second가 개수 판단
	if (p1.first == p2.first && p1.second == p2.second) // 사실 p1.second == p2.second 만 해도 되지만, 코드 읽기 쉽게 하기 위해
	{
		check_OddOrEven = true;
		check_num = true;
	}
	else if (p1.first == p2.first) // 홀짝만 맞췄을 경우
	{
		check_OddOrEven = true;
		check_num = false;
	}
	else { // 틀린 경우
		check_OddOrEven = false;
		check_num = false;
	}
}

//player가 구슬의 개수를 결정하고, 홀짝을 판별하는 함수 -> 구슬의 개수를 정하기만 하니, 항상 true 반환
bool PlayerOddOrEven::act()
{
	int marbleRange = 1 + (Player::fearlessness) / 15;
	std::uniform_int_distribution<unsigned int> randMarble_choose(1, marbleRange);
	int chooseMarble_num = randMarble_choose(random_engine);

	if (chooseMarble_num % 2 == 1)
	{
		player1_marble = std::make_pair(1, chooseMarble_num); //pair의 첫번째 원소에는 홀수면 1, 짝수면 2를, 
															  //두번째 원소에는 걸은 원소의 개수를 집어넣는다. 
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
	int expect_MarbleRange_start = known_marble - (100 - agility) * 0.09; // 이후 비율 조절이 필요해 보임
	int expect_MarbleRange_end = known_marble + (100 - agility) * 0.09;
	//범위 내의 구슬 개수를 랜덤으로 고르게 랜덤 함수를 재정의 해줌

	//player 2가 구슬 개수를 예측
	randMarble_expect = std::uniform_int_distribution<int>(expect_MarbleRange_start, expect_MarbleRange_end); 

	int expectedMarble_num = randMarble_expect(random_engine);
	if (expectedMarble_num % 2 == 1)
	{
		player2_marble = std::make_pair(1, expectedMarble_num); //pair의 첫번째 원소에는 홀수면 1, 짝수면 2를, 
																// 두번째 원소에는 예상한 원소의 개수를 집어넣는다. 
	}
	else
	{
		player2_marble = std::make_pair(2, expectedMarble_num);
	}
	// plyaer1과 비교 및 구슬 재분배 
	PlayerOddOrEven::CompareOddorEven(player1_marble, player2_marble);
	if (check_OddOrEven && check_num)
	{
		//홀짝 및 개수 예측까지 성공했을 시, 15개 획득
		this->current_marble += 15;
		//만약 구슬을 얻어 40개 이상이 되었다면
		if (current_marble >= 40 && isPlaying()) {
			this->playing = false; // playing = false로 바꾸고 탈출 출력
			printStatus();
			std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		}
		return true;
	}
	else if (check_OddOrEven && !check_num)
	{
		//홀짝만 맞췄을 경우엔 상대방 것 10개 획득
		this->current_marble += 10;
		//만약 구슬을 얻어 40개 이상이 되었다면
		if (current_marble >= 40 && isPlaying()) {
			this->playing = false;// playing = false로 바꾸고 탈출 출력
			printStatus();
			std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		}
		return true;
	}
	else
	{
		//예상이 틀렸다면, 상대방에게 5개 헌납
		this->current_marble -= 5;
		// 만약 구슬을 잃어 0개 이하가 되어 탈락했다면
		if (this->current_marble <= 0) {
			this->playing = false; // 일단 playing만 false로 바꿈 ->  return false로 처리해줄것임
			//this->dyingMessage();
		}
		return false;
	}

};

//현재 객체(player1)에 대해 상대방이 홀짝을 맞췄늦지 확인하고, 만약 current_marble이 40개가 넘어가거나 0보다 적으지면 playing false로 바꿈 
void PlayerOddOrEven::checkMarble()
{
	// 이 함수는 예상하는 쪽이 아닌 홀짝 내기를 걸은 쪽이 사용하는 함수 -> 상대방이 맞추고 틀리고에 따른 구슬 재분배 함수라고 이해
	
	//만약 상대방이 맞췄다면 그만큼 뺏겨야 하며, 틀렸다면, 그만큼 얻어야 함
	if (check_OddOrEven && check_num) {
		this->current_marble -= 15; // 상대방이 가져간 만큼 잃음
	}
	else if (check_OddOrEven) {
		this->current_marble -= 10; // 상대방이 가져만 만큼 잃음
	}
	else {
		this->current_marble += 5; // 상대방의 예상이 틀려 구슬을 얻음
	}
	if (current_marble >= 40 && isPlaying()) { // 만약 구슬을 얻어서 탈출하면
		printStatus();// 탈출 출력 및 playing =false
		std::cout << " safely escaped from the Odd OR Even Game." << std::endl;
		this->playing = false;

	}

	if (this->current_marble <= 0) { // 구슬을 다 잃으면
		this->playing = false; // playing = false
		//this->dyingMessage();
	}
	else
		this->playing = true; // 탈출하거나 탈락하지 않으면 계속 playing
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
	this->current_marble -= 1; // 구슬을 굴릴때마다 그 객채의 구슬 개수 1개씩 줄어듬
	int holeDistance = randHole_distance(random_engine);
	int throw_distance_start = holeDistance - (100 - agility) * 10; // 이후 비율 조절이 필요해 보임
	int throw_distance_end = holeDistance + (100 - agility) * 10;
	randHole_distance = std::uniform_int_distribution<int>(throw_distance_start, throw_distance_end);

	int player_throw_distance = randHole_distance(random_engine);
	if (player_throw_distance >= (holeDistance - 25) && player_throw_distance <= (holeDistance + 25)) { // 일정 구멍 크기 안에 들어가면 성공
		return true;
	}
	if (this->current_marble <= 0) { // 구슬을 모두 다 사용하면
		return false;
	}
	else {
		return false;
	}
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
		std::cout << " died." << std::endl;
	}
}

