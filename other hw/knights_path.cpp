#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<int> used(65);                                            // вектор посещенных вершин
vector<int> parents(65);                                         // вектор из предков
vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}; // буковки шахматной доски
vector<vector<int>> vert = {{-17, -10, 6, 15},                   // возможные ходы из текущей вершины, в зависимости от столбца
                            {-15, -6, 10, 17},
                            {-17, -15, -6, 10, 15, 17},
                            {-17, -15, -10, -6, 6, 10, 15, 17},
                            {-17, -15, -10, -6, 6, 10, 15, 17},
                            {-17, -15, -10, -6, 6, 10, 15, 17},
                            {-17, -15, -10, -6, 6, 10, 15, 17},
                            {-17, -15, -10, 6, 15, 17}};

struct queue // структура очереди
{
    int inf;
    queue *next;
};

void push(queue *&h, queue *&t, int x) // добавление элемента в очередь
{
    queue *r = new queue; // создаём новый элемент
    r->inf = x;           // информационное поле = х
    r->next = NULL;       // всегда последний
    if (!h && !t)         // если очередь пуста
        h = t = r;        // это и голова и хвост
    else
    {
        t->next = r; // r - следующий для хвоста
        t = r;       // теперь r - хвост
    }
}

int pop(queue *&h, queue *&t) // удаление элемента из очереди
{
    queue *r = h;   // создаём указатель на голову
    int i = h->inf; // сохраняем значение головы
    h = h->next;    // сдвигаем указатель на следующий эдлемент
    if (!h)         // если удаляем последний элемент из очереди
        t = NULL;
    delete r; // удаляем первый элемент
    return i; // возвращаем его значение
}

bool range(int x) // проверка - выходит ли за границы поля шаг
{
    if (x >= 1 && x <= 64)
        return true;
    return false;
}

void bfs(int start)
{                       // функция обхода в ширину
    queue *head = NULL; // инициализация очереди
    queue *tail = NULL;
    used[start] = 1;         // отмечаем вершину посещенной
    push(head, tail, start); // добавляем в очередь
    parents[start] = -1;     // у стартовой вершины предок отсутствует
    while (head && tail)
    {                            // пока очередь не пуста
        int v = pop(head, tail); // извлекаем вершину из очереди
        for (auto y : vert[v % 8])
        { // проходимся по смежным с ней вершинам
            int u = v + y;
            if (range(u)) // если шаг возможен
            {
                if (used[u])
                    continue;
                // если вершину не посещали
                used[u] = 1;         // отмечаем посещённой
                push(head, tail, u); // добавляем в очередь
                parents[u] = v;      // запоминаем предка
            }
        }
    }
}

int coord_to_number(string str1) // перевод координаты клетки в число
{
    int num = str1[1] - '0' - 1; // переводим номер строки в число
    int let = str1[0] - 'A' + 1; // переводим букву столбца в число

    return 8 * num + let; // получаем занумерованные из левого нижнего угла
}

string number_to_coord(int num) // перевод из номера в букво-число
{
    string str;
    str = letters[(num - 1) % 8] + to_string((num + 7) / 8); //(num - 1) % 8 - определяем номер столбца и находим нужную букву
    //(num + 7) / 8 - определяем номер строки
    return str; // возвращаем координату клетки
}

void cout_path(int end)
{ // выводим кратчайший путь из стартовой в конечную клетку
    vector<int> path;
    int v = end;
    while (v != -1) // идём по вектору предков из конечной клетки до начальной
    {
        path.push_back(v); // добавляем в вектор
        v = parents[v];    // переходим к предку
    }

    reverse(path.begin(), path.end()); // переворачиваем, тк путь записывался в обратном порядке
    for (auto v : path)                // выводим путь
        cout << number_to_coord(v) << ' ';
}

int main()
{
    string start, end;
    cout << "enter start coordinate: ";
    getline(cin, start);
    cout << "enter end coordinate: ";
    getline(cin, end);

    bfs(coord_to_number(start));

    cout << "answer: ";
    cout_path(coord_to_number(end));

    return 0;
}