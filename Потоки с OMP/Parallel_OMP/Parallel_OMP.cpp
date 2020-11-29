#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <future>
#include <functional>
#include <set>
#include <omp.h>

using namespace std;

/* Неугодников Сергей Игоревич
	БПИ 197

	Вариант 14
	Определить множество индексов i, для которых A[i] и B[i] не имеют
	общих делителей (единицу в роли делителя не рассматривать).
	Входные данные: массивы целых положительных чисел А и B, произвольной длины ≥1000.
	Количество потоков является входным параметром
*/

const int c = 1000;

/// <summary>
/// Получаем максимальное значение длинны массива.
/// </summary>
/// <returns></returns>
int length() {

	std::string s_length;
	int max_length;

	std::cout << "Write max length of arrays (not lower than " << c << "): ";
	std::cin >> s_length;
	max_length = atoi(s_length.c_str());
	while (max_length < c) {
		std::cout << '\n' << "Wrong number." << '\n' << "Try again: ";
		std::cin >> s_length;
		max_length = atoi(s_length.c_str());
	}
	std::cout << '\n';
	return max_length;
}

/// <summary>
/// Получаем максимальное значение переменной в массиве
/// </summary>
/// <returns></returns>
int value() {

	std::string s_value;
	int max_value;

	std::cout << "Write max number of values in arrays (not lower than 1): ";
	std::cin >> s_value;
	max_value = atoi(s_value.c_str());
	while (max_value < 1) {
		std::cout << '\n' << "Wrong number." << '\n' << "Try again: ";
		std::cin >> s_value;
		max_value = atoi(s_value.c_str());
	}
	std::cout << '\n';
	return max_value;
}

/// <summary>
/// Получаем кол-во потоков, которые кол-вом
/// не превосходят минимальную длинну массива
/// </summary>
/// <param name="min_length"> минимальная длинна массива </param>
/// <returns></returns>
int t_count(int min_length) {

	std::string s_count;
	int count;

	std::cout << "Write count of threads (not lower than 1 and not higher than " << min_length << "): ";
	std::cin >> s_count;
	count = atoi(s_count.c_str());
	while (count < 1 || count > min_length) {
		std::cout << '\n' << "Wrong number." << '\n' << "Try again: ";
		std::cin >> s_count;
		count = atoi(s_count.c_str());
	}
	std::cout << '\n';
	return count;
}

/// <summary>
/// Создаем вектор с рандомной длинной и рандомными значениями
/// </summary>
/// <param name="max_length"> максимальная длинна массива </param>
/// <param name="max_value"> максимальное значение в массиве </param>
/// <returns></returns>
std::vector<int> create_vector(const int& max_length, const int& max_value) {

	std::vector <int> v;
	//Задаем длинну вектора из отрезка [1000, max_length]
	v.reserve(c + (rand() % (max_length - c + 1)));
	for (size_t i = 0; i < v.capacity(); i++)
	{
		//Добавляем переменную из отрезка [1, max_value]
		v.push_back(1 + (rand() % max_value));
	}
	return v;
}

/// <summary>
/// Выводим получившийся массив в консоль
/// </summary>
/// <param name="arr"> массив </param>
/// <param name="name"> название </param>
void write_array(const std::vector<int>& A, const std::vector<int>& B) {

	std::cout << "Array A:" << '\t' << "Array B:\n";
	for (int i = 0; i < std::min(A.size(), B.size()); i++)
		std::cout << "A[" << i << ']' << " = " << A[i] << '\t' << "B[" << i << ']' << " = " << B[i] << '\n';
	for (int i = std::min(A.size(), B.size()); i < std::max(A.size(), B.size()); i++)
		std::cout << '\t' << '\t' << "B[" << i << ']' << " = " << B[i] << '\n';
}

/// <summary>
/// Алгоритм евклида
/// </summary>
/// <param name="a"> первая переменная </param>
/// <param name="b"> вторая переменная </param>
/// <returns></returns>
bool evk(int a, int b) {
	while (a != 0 && b != 0) {
		if (a > b && b > 0)
			a %= b;
		else
			if (a > 0)
				b %= a;
	}
	return a == 1 || b == 1;
}

/// <summary>
/// Находим индексы элементов у которых нет
/// общих делителей
/// </summary>
/// <param name="A"> массив А</param>
/// <param name="B"> массив В </param>
/// <param name="thread_count"> количество потоков </param>
/// <param name="length"> рассматриваемая длинна части масивов </param>
/// <returns></returns>
std::set<int> Threads(const std::vector<int>& A, const std::vector<int>& B, std::size_t thread_count, std::size_t length) {
	//Сет индеков
	std::set<int> answer;

	//Задаем количество потоков
	omp_set_num_threads(thread_count);

	//Начинаем работу с потоками
#pragma omp parallel 
	{
#pragma omp for
		for (int i = 0; i < min(A.size(), B.size()); i++)
		{
			if (evk(A[i], B[i]))
#pragma omp critical
			{
				answer.insert(i);
			}
		}
	}
	return answer;
}

int main()
{
	srand(time(NULL));

	//Проверка на всякий случай
	try {
		//Максимально возможная длинна массивов и их максимально возможное значение
		const int max_length = length(), max_value = value();

		//Создаем вектора
		std::vector <int> A = create_vector(max_length, max_value);
		std::vector <int> B = create_vector(max_length, max_value);

		//Количество потоков
		int thread_count = t_count(std::min(A.size(), B.size()));

		//Выводим значение массивов в консоль
		write_array(A, B);

		//Вектор индексов
		std::vector<int> answer;
		//Сет потоков
		std::set <set<int>> threads;

		//Деление массивов на части.
		const int parts = std::min(A.size(), B.size()) / thread_count;

		//Получаем сеты с ответами
		threads.insert(Threads(A, B, thread_count, parts));

		//Записываем значения в вектор
		for (auto& thread : threads) {
			for (auto& i : thread)
				answer.push_back(i);
		}

		//Выписываем ответ в консоль
		std::cout << "\nAnswer:\n";
		for (auto& i : answer)
			std::cout << "[" << i << "]\n";
	}
	catch (...) {}
}