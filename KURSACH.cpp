#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>
#include <cstdlib>
#include <conio.h>

using namespace std;

class Graph {
private:
    int size;
    vector<vector<int>> adjacencyMatrix;

public:
    Graph(int n) : size(n), adjacencyMatrix(n, vector<int>(n, 0)) {}

    void addEdge(int u, int v) {
        int weight = rand() % 11;  // Генерация случайного веса от 0 до 10
        adjacencyMatrix[u][v] = weight;
        adjacencyMatrix[v][u] = weight;
    }

    void printGraph() {
        cout << "Матрица смежности графа:" << endl;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << adjacencyMatrix[i][j] << ' ';
            }
            cout << endl;
        }
    }

    vector<set<int>> findAllIndependentSets() {
        vector<set<int>> independentSets;
        set<set<int>> uniqueSets;

        vector<int> currentSet;
        findAllIndependentSetsHelper(0, currentSet, independentSets, uniqueSets);

        return independentSets;
    }

    void findAllIndependentSetsHelper(int vertex, vector<int>& currentSet, vector<set<int>>& independentSets, set<set<int>>& uniqueSets) {
        bool isIndependent = true;
        for (int v : currentSet) {
            if (adjacencyMatrix[vertex][v] > 0) {  // Проверка, что вершины не связаны
                isIndependent = false;
                break;
            }
        }

        if (isIndependent) {
            currentSet.push_back(vertex);
            set<int> currentSetUnique(currentSet.begin(), currentSet.end());

            // Проверка на уникальность множества перед добавлением
            if (uniqueSets.find(currentSetUnique) == uniqueSets.end()) {
                uniqueSets.insert(currentSetUnique);
                independentSets.push_back(currentSetUnique);
            }

            for (int nextVertex = vertex + 1; nextVertex < size; ++nextVertex) {
                findAllIndependentSetsHelper(nextVertex, currentSet, independentSets, uniqueSets);
            }

            currentSet.pop_back();
        }

        if (vertex + 1 < size) {
            findAllIndependentSetsHelper(vertex + 1, currentSet, independentSets, uniqueSets);
        }
    }

    void saveResults(const vector<set<int>>& independentSets) {
        ofstream outputFile("independent_sets.txt");
        if (!outputFile.is_open()) {
            cout << "Ошибка при открытии файла для записи результатов." << endl;
            return;
        }

        for (const auto& independentSet : independentSets) {
            for (const int& vertex : independentSet) {
                outputFile << vertex << " ";
            }
            outputFile << endl;
        }

        cout << "Результаты сохранены в файле independent_sets.txt" << endl;
        outputFile.close();
    }

    void inputFromKeyboard() {
        cout << "Введите количество рёбер: ";
        int edges;
        cin >> edges;
        if (edges <= 0) {
            cout << "Такое количество рёбер быть не может" << endl;
            return;
        }

        cout << "Введите рёбра в формате 'вершина1 вершина2', по одной паре на строку:" << endl;
        for (int i = 0; i < edges; ++i) {
            int u, v;
            cin >> u >> v;
            addEdge(u, v);
        }
    }

    void inputFromFile(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Ошибка при открытии файла для чтения." << endl;
            return;
        }

        inputFile >> size;  // Используем size текущего объекта

        // Инициализация графа с учетом количества вершин
        adjacencyMatrix = vector<vector<int>>(size, vector<int>(size, 0));

        for (int i = 0; i < size; ++i) {
            int u, v;
            inputFile >> u >> v;
            addEdge(u, v);  // Используем метод addEdge для добавления ребер
        }

        inputFile.close();
    }


    void displayMenu() {
        cout << "\nМеню:\n";
        cout << "1. Вывести матрицу смежности\n";
        cout << "2. Найти все независимые множества вершин\n";
        cout << "3. Сохранить результаты в файл\n";
        cout << "4. Выйти из программы\n";
    }
};

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Rus");
    cout << "Курсовая работа на Тему: Поиск независимых множеств вершин графа." << endl;
    cout << "Выполнил студент группы 22ВВП2 Майоров Н.А." << endl;
    cout << "Нажмите любую клавишу для продолжения..." << endl;
    _getch(); // Ожидание нажатия клавиши

    system("cls"); // Очистка консоли

    Graph graph(0); // Инициализация пустого графа

    double choice;
    int proverka;
    do {
        
        while (true) {
            cout << "Выберите способ задания графа (1 - из файла, 2 - случайный): ";
            cin >> choice;

            // Проверка на дробное число
            if (cin.fail() || choice != static_cast<int>(choice)) {
                cout << "Некорректный выбор. Пожалуйста, введите целое число." << endl;
                cin.clear();   // Очистка флага ошибки
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Прочистка буфера ввода до конца строки
            }
            else {
                proverka = static_cast<int>(choice);
                break;  // Выход из цикла при корректном вводе
            }
        }

        switch (static_cast<int>(choice)) {
        case 1: {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;
            graph.inputFromFile(filename);
            break;
        }
        case 2: {
            int size;
            do {
                cout << "Введите размер графа (множества): ";
                cin >> size;
                if (size <= 0 || size >= 1000) {
                    cout << "Граф такого размера не может быть создан. Пожалуйста, введите корректное значение." << endl;
                    cin.clear();   // Очистка флага ошибки
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (size <= 0 || size >= 1000);

            graph = Graph(size); // Перегенерация графа

            // Автоматическое задание графа (случайные связи)
            for (int i = 0; i < size; ++i) {
                for (int j = i + 1; j < size; ++j) {
                    if (rand() % 2 == 0) {
                        graph.addEdge(i, j);
                    }
                }
            }
            break;
        }
        default:
            cout << "Некорректный выбор. Пожалуйста, введите 1 или 2." << endl;
        }
    } while (static_cast<int>(choice) != 1 && static_cast<int>(choice) != 2);


    int menuChoice;
    do {
        // Отображение меню и получение выбора пользователя
        graph.displayMenu();
        cout << "Выберите действие: ";
        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            graph.printGraph();
            break;
        case 2:
            // Поиск всех независимых множеств и вывод результатов
        {
            vector<set<int>> independentSets = graph.findAllIndependentSets();
            cout << "Все найденные независимые множества вершин графа:" << endl;
            for (size_t i = 0; i < independentSets.size(); ++i) {
                cout << "Множество " << i + 1 << ": ";
                for (const int& vertex : independentSets[i]) {
                    cout << vertex << " ";
                }
                cout << endl;
            }
        }
        break;
        case 3:
            // Сохранение результатов в файл
        {
            vector<set<int>> independentSets = graph.findAllIndependentSets();
            graph.saveResults(independentSets);
        }
        break;
        case 4:
            cout << "Программа завершена. Хорошего дня!" << endl;
            break;
        default:
            cout << "Некорректный выбор. Попробуйте еще раз." << endl;
        }

    } while (menuChoice != 4);

    return 0;
}

