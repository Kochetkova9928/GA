#include <iostream> // для cout и т.п.
#include <vector> // для класса vector
#include <string> // для класса string
#include <algorithm> // для алгоритма сортировки
#include <time.h> // для случайных величин
#include <math.h> // для abs()

#define GA_SIZE 3000 // размер популяции
#define GA_MAXITER 20000 // максимальное число итераций
#define GA_BEST 0.2f // лучшие
#define GA_MUT 0.5f // мутации
#define GA_MUTATION RAND_MAX * GA_MUT
#define GA_TARGET std::string("МИР")
const std::string GA_TABLE = " абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";


using namespace std;

struct ga_struct
{
	string str; // строка
	unsigned int rating; // пригодность
};

// формиирование первой (начальной) популяции
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

// оценка популяции (пригодность)
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

// компаратор
bool sort_by_rating_comp(ga_struct a, ga_struct b) {
	return (a.rating < b.rating);
}

// сортировка популяций по оценке
inline void sort_by_rating(vector<ga_struct>& population) {
	sort(population.begin(), population.end(), sort_by_rating_comp);
}

// селекция
void selection(vector<ga_struct>& population, vector<ga_struct>& buffer, int best_size) {
	for (int i = 0; i < best_size; i++) {
		buffer[i].str = population[i].str;
		buffer[i].rating = population[i].rating;
	}
}

// мутация
void mutate(ga_struct& member) {
	int target_size = GA_TARGET.size();
	int ipos = rand() % target_size;

	member.str[ipos] = GA_TABLE[(rand() % GA_TABLE.size())];
}

// селекция, скрещивание, мутация
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

//вывод лучшей популяции
inline void print_best(vector<ga_struct>& pop) {
	cout << pop[0].str << " (" << pop[0].rating << ")" << endl;
}

// очистка буффера
inline void swap(vector<ga_struct>*& population, vector<ga_struct>*& buffer) {
	vector<ga_struct>* temp = population;
	population = buffer;
	buffer = temp;
}

int main() {
	srand(unsigned(time(NULL)));
	setlocale(LC_ALL, "Russian"); // задаём русский текст
	system("chcp 1251"); // настраиваем кодировку консоли

	vector<ga_struct> pop_a, pop_b;
	vector<ga_struct>* population, * buffer;

	first_population(pop_a, pop_b);
	population = &pop_a;
	buffer = &pop_b;

	for (int i = 0; i < GA_MAXITER; i++) {
		calc_rating(*population); // оцениваем популяцию
		sort_by_rating(*population); // сортируем популяцию
		print_best(*population); // выводим лучшую популяцию

		if ((*population)[0].rating == 0) {
			break;
		}

		mate(*population, *buffer); // спариваем популяции
		swap(population, buffer); // очищаем буферы
	}

	return 0;
}
