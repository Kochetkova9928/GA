#include <iostream> // ��� cout � �.�.
#include <vector> // ��� ������ vector
#include <string> // ��� ������ string
#include <algorithm> // ��� ��������� ����������
#include <time.h> // ��� ��������� �������
#include <math.h> // ��� abs()

#define GA_SIZE 3000 // ������ ���������
#define GA_MAXITER 20000 // ������������ ����� ��������
#define GA_BEST 0.2f // ������
#define GA_MUT 0.5f // �������
#define GA_MUTATION RAND_MAX * GA_MUT
#define GA_TARGET std::string("���")
const std::string GA_TABLE = " �������������������������������������Ũ��������������������������";


using namespace std;

struct ga_struct
{
	string str; // ������
	unsigned int rating; // �����������
};

// ������������� ������ (���������) ���������
void first_population(vector<ga_struct>& population, vector<ga_struct>& buffer) {
	int target_size = GA_TARGET.size();

	for (int i = 0; i < GA_SIZE; i++) {
		ga_struct chromosoma;

		chromosoma.rating = 0;
		chromosoma.str.erase();

		for (int j = 0; j < target_size; j++) {
			chromosoma.str += GA_TABLE[(rand() % GA_TABLE.size())];
		}

		population.push_back(chromosoma);
	}

	buffer.resize(GA_SIZE);
}

// ������ ��������� (�����������)
void calc_rating(vector<ga_struct>& population) {
	string target = GA_TARGET;
	int target_size = target.size();
	unsigned int rating;

	for (int i = 0; i < GA_SIZE; i++) {
		rating = 0;
		for (int j = 0; j < target_size; j++) {
			rating += abs(int(population[i].str[j] - target[j]));
		}

		population[i].rating = rating;
	}
}

// ����������
bool sort_by_rating_comp(ga_struct a, ga_struct b) {
	return (a.rating < b.rating);
}

// ���������� ��������� �� ������
inline void sort_by_rating(vector<ga_struct>& population) {
	sort(population.begin(), population.end(), sort_by_rating_comp);
}

// ��������
void selection(vector<ga_struct>& population, vector<ga_struct>& buffer, int best_size) {
	for (int i = 0; i < best_size; i++) {
		buffer[i].str = population[i].str;
		buffer[i].rating = population[i].rating;
	}
}

// �������
void mutate(ga_struct& member) {
	int target_size = GA_TARGET.size();
	int ipos = rand() % target_size;

	member.str[ipos] = GA_TABLE[(rand() % GA_TABLE.size())];
}

// ��������, �����������, �������
void mate(vector<ga_struct>& population, vector<ga_struct>& buffer) {
	int best_size = GA_SIZE * GA_BEST;
	int target_size = GA_TARGET.size();
	int rand_target;
	int p1;
	int p2;

	selection(population, buffer, best_size);


	for (int i = best_size; i < GA_SIZE; i++) {
		p1 = rand() % (GA_SIZE / 2);
		p2 = rand() % (GA_SIZE / 2);
		rand_target = rand() % target_size;

		buffer[i].str = population[p1].str.substr(0, rand_target) +
			population[p2].str.substr(rand_target, best_size - rand_target);

		if (rand() < GA_MUTATION) {
			mutate(buffer[i]);
		}
	}
}

//����� ������ ���������
inline void print_best(vector<ga_struct>& pop) {
	cout << pop[0].str << " (" << pop[0].rating << ")" << endl;
}

// ������� �������
inline void swap(vector<ga_struct>*& population, vector<ga_struct>*& buffer) {
	vector<ga_struct>* temp = population;
	population = buffer;
	buffer = temp;
}

int main() {
	srand(unsigned(time(NULL)));
	setlocale(LC_ALL, "Russian"); // ����� ������� �����
	system("chcp 1251"); // ����������� ��������� �������

	vector<ga_struct> pop_a, pop_b;
	vector<ga_struct>* population, * buffer;

	first_population(pop_a, pop_b);
	population = &pop_a;
	buffer = &pop_b;

	for (int i = 0; i < GA_MAXITER; i++) {
		calc_rating(*population); // ��������� ���������
		sort_by_rating(*population); // ��������� ���������
		print_best(*population); // ������� ������ ���������

		if ((*population)[0].rating == 0) {
			break;
		}

		mate(*population, *buffer); // ��������� ���������
		swap(population, buffer); // ������� ������
	}

	return 0;
}