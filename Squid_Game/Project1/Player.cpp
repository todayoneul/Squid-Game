#include "Player.h"
#include "Game.h"

std::default_random_engine Player::random_engine(time(nullptr));
std::uniform_int_distribution<unsigned int> Player::ability_range(0, 100);
std::uniform_real_distribution<float> Player::possibility(0.f, 1.f);
std::uniform_int_distribution <unsigned int> PlayerRLGL::rand_dis(0, 10);//randomDistance�� ���� �߰�����
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




//���� �����ؾ� �� ��
bool PlayerRLGL::act()
{
	//random distance ����
	unsigned int randomDistance = rand_dis(random_engine);
	
	if (fearlessness > 100 * possibility(random_engine)) {
		//75% �̻��� Ȯ���� fearless_bonus_distance�� �ް�, 10%�� Ȯ���� ������, ������ ��쿣 �� �� ���� ����
		auto RLGL = [] {
			float p = possibility(random_engine);
			// p�� 0���� 100 ������ ������ ������ �����ǰ�, �̸� Ȯ���� ��ȯ�����ֱ� ���� 
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
			//fearless bounus distance ����
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

// �� �÷��̾��� Ȧ¦ �����, ���� ����� Ȯ���ϴ� �Լ�
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

//player�� ������ ������ �����ϰ�, Ȧ¦�� �Ǻ��ϴ� �Լ� ��� ���� �� ���� 
bool PlayerOddOrEven::act()
{
	int marbleRange = 1 + (Player::fearlessness) / 15;
	std::uniform_int_distribution<unsigned int> randMarble_choose(1, marbleRange);
	int chooseMarble_num = randMarble_choose(random_engine);

	if (chooseMarble_num % 2 == 1)
	{
		player1_marble = std::make_pair(1, chooseMarble_num); //pair�� ù��° ���ҿ��� Ȧ���� 1, ¦���� 2��, �ι�° ���ҿ��� ���� ������ ������ ����ִ´�. 
	}
	else
	{
		player1_marble = std::make_pair(2, chooseMarble_num);
	}
	return true;

};

bool PlayerOddOrEven::act2()
{
	const int known_marble = player1_marble.second;//known_marble�� ������ player�� agility�� Ȱ���� �� �ִ� ���

	//agility ũ�⿡ ���� Ž�� ������ �ٿ�����
	int expect_MarbleRange_start = known_marble - (100-agility) * 0.09; // ���� ���� ������ �ʿ��� ����
	int expect_MarbleRange_end = known_marble + (100-agility) * 0.09; 
	//���� ���� ���� ������ �������� ���� ���� �Լ��� ������ ����

		randMarble_expect = std::uniform_int_distribution<int>(expect_MarbleRange_start , expect_MarbleRange_end);

	int expectedMarble_num = randMarble_expect(random_engine);
	if (expectedMarble_num % 2 == 1)
	{
		player2_marble = std::make_pair(1, expectedMarble_num); //pair�� ù��° ���ҿ��� Ȧ���� 1, ¦���� 2��, �ι�° ���ҿ��� ������ ������ ������ ����ִ´�. 
	}
	else
	{
		player2_marble = std::make_pair(2, expectedMarble_num);
	}
	// plyaer1�� �� �� ���� ��й� -> Hole_In_One ���ӿ����� �����ϰ� ����Ǵ� �κ�
	PlayerMarbleGame::CompareOddorEven(player1_marble, player2_marble);
	if (check_OddOrEven && check_num)
	{
		//Ȧ¦ �� ���� �������� �������� ��, 12�� ȹ��
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
		//Ȧ¦�� ������ ��쿣 ���� �� 5�� ȹ��
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
		//������ Ʋ�ȴٸ�, ���濡�� 3�� �峳
		this->current_marble -= 5;
		if (this->current_marble <= 0) {
			this->playing = false;
			//this->dyingMessage();
		}
		return false;
	}

};

//���� ��ü(plyer1)�� ���� ������ Ȧ¦�� ������� Ȯ���ϰ�, ���� current_marble�� 40���� �Ѿ�ų� 0���� �������� playing false�� �ٲ� 
void PlayerOddOrEven::checkMarble() 
{
	//���� ������ ����ٸ� �׸�ŭ ���ܾ� �ϸ�, Ʋ�ȴٸ�, �׸�ŭ ���� ��
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

bool PlayerHole_In_One::act() // ���� ������
{
	this->current_marble -= 1;
	int holeDistance = randHole_distance(random_engine);
	int throw_distance_start = holeDistance - (100-agility) * 10; // ���� ���� ������ �ʿ��� ����
	int throw_distance_end =  holeDistance + (100-agility) * 10;
	randHole_distance = std::uniform_int_distribution<int>(throw_distance_start, throw_distance_end);
	
	int player_throw_distance = randHole_distance(random_engine);
	if (player_throw_distance >= (holeDistance - 25) && player_throw_distance <= (holeDistance + 25)) { // ���� ���� ũ�� �ȿ� ���� ����
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


