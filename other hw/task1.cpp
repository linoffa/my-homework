#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <string>

using namespace std;

ifstream in("file.txt"); //файл, из которого считываем
ofstream out("file_out.txt"); //файл, в который выводим

struct date //дата
{
    int dd, mm, yy;
};

struct people //данные о человеке
{
    string surname; //фамилия
    string profession; //профессия
    date date_of_birth; //дата рождения
    int experience; //стаж
    int salary; //зарплата
};

struct list //структура списка
{
    people inf;
    list *next;
    list *prev;
};

struct hashing_table // структура хэш-таблицы
{
    list *h = NULL;
    list *t = NULL;
};

void push (list *&h, list *&t, people x) //вставка элемента в конец списка
{ 
    list *r = new list; //создаем новый элемент
    r->inf = x;
    r->next = NULL; //всегда последний
    if (!h && !t)
    { //если список пуст
        r->prev = NULL; //первый элемент
        h = r; //это голова
    }
    else{
        t->next = r; //r - следующий для хвоста
        r->prev = t; //хвост - предыдущий для r
    }
    t = r; //r теперь хвост
}

void print (list *h, list *t) //печать элементов списка
{ 
    list *p = h; //указатель на голову
    while (p)
    { //пока не дошли до конца списка
        cout << p->inf.experience << " ";
        p = p->next; //переход к следующему элементу
    }
}

void del_node (list *&h, list *&t, list *r)
{ //удаляем после r
    if (r == h && r == t) //единственный элемент списка
        h = t = NULL;
    else if (r == h)
    { //удаляем голову списка
        h = h->next; //сдвигаем голову
        h->prev = NULL;
    }
    else if (r == t)
    { //удаляем хвост списка
        t = t->prev; //сдвигаем хвост
        t->next = NULL;
    }
    else
    {
        r->next->prev = r->prev; 
        //для следующего от r предыдущим становится r->prev
        r->prev->next = r->next; 
        //для предыдущего от r следующим становится r->next
    }
    delete r; //удаляем r
}

date str_to_date(string str) //перевод из строки в дату
{
    date x;
    string temp = str.substr(0,2); //день
    x.dd = atoi(temp.c_str());
    temp = str.substr(3, 2); //месяц
    x.mm = atoi(temp.c_str());
    temp = str.substr(6, 4); //год
    x.yy = atoi(temp.c_str());
    return x;
}

vector<people> in_file() //ввод данных из файла
{
    vector<people> x;
    people temp;
    while(in.peek() != EOF)
    {
        in >> temp.surname; //фамилия
        in >> temp.profession; //профессия
        string tmp;
        in >> tmp; 
        temp.date_of_birth = str_to_date(tmp); //дата рождения
        in >> temp.experience; //стаж
        in >> temp.salary; //зарплата
        x.push_back(temp);
    }
    
    return x;
}

int hash_function(int k, int size) // хэш-функция
{
    return k % size;
}

hashing_table *create(vector<people> x, int size) // создание хэш-таблицы
{
    hashing_table *table = new hashing_table[size]; // создаем новый элемент
    int num = 0;
    for (int i = 0; i < 25; i++)
    {
        num = hash_function(x[i].experience, size); 
        push(table[num].h, table[num].t, x[i]); 
        // добавляем в ячейку в зависимости от хэш-функции
    }
    return table;
}

void print_hashing_table(hashing_table *table, int size) 
{ // функция вывода хэш-таблицы
    for (int i = 0; i < size; i++)
    {
        cout << "Hash code " << i << " : ";
        print(table[i].h, table[i].t);
        cout << endl;
    }
}

void del(int m, hashing_table *&table, int size) 
{ // функция удаления эл-та хэш-таблицы
    int num = hash_function(m, size); // вычисляем значение хэш-функции
    list *p = table[num].h;
    while(p)
    {
        if (p->inf.experience == m) // если нашли эл-т 
        {
            del_node(table[num].h, table[num].t, p); // удаляем
            return;
        }
        p = p->next; // иначе ищем дальше
    }
}

list *find(int m, hashing_table *&table, int size)
{ // функция поиска эл-та хэш-таблицы
    int num = hash_function(m, size); // вычисляем значение хэш-функции
    list *p = table[num].h;
    while(p)
    {
        if (p->inf.experience == m) // если нашли эл-т 
            return p; // возвращаем указатель
        p = p->next; // идём дальше
    }
    return NULL;
}

void print_about_people(people curr) // выводим информацию о человеке
{
    cout << curr.surname << " " << curr.profession << " ";
    if (curr.date_of_birth.dd < 10)
        cout << "0" << curr.date_of_birth.dd << ".";
    else
        cout << curr.date_of_birth.dd << ".";
    if (curr.date_of_birth.mm < 10)
        cout << "0" << curr.date_of_birth.mm << ".";
    else
        cout << curr.date_of_birth.mm << ".";
    if (curr.date_of_birth.yy < 10)
        cout << "0" << curr.date_of_birth.yy << " ";
    else
        cout << curr.date_of_birth.yy << " ";
    cout << curr.experience << " " << curr.salary << endl;
}

void print_people(people x) //вывод в файл данных из структуры people
{
    out << setw(10) << left << x.surname; //фамилия, 10 позиций
    out << setw(15) << left << x.profession; //профессия, 15 позиций
    if (x.date_of_birth.dd < 10) out << left << '0' << x.date_of_birth.dd << '.';
    else out << left << x.date_of_birth.dd << '.'; //день рождения, добавляем 0, если одна цифра
    if (x.date_of_birth.mm < 10) out << '0' << x.date_of_birth.mm << '.';
    else out << x.date_of_birth.mm << '.'; //месяц рождения, добавляем 0, если одна цифра
    out << left << setw(6) << x.date_of_birth.yy; //год рождения, 6 позиций
    out << left << setw(5) << x.experience; //стаж, 5 позиций
    out << left << setw(10) << x.salary << endl; //зарплата, 10 позиций
}

int main(){
    vector<people> x;
    x = in_file();
    int size;
    cout << "Enter the size of table: ";
    cin >> size;
    hashing_table *table = create(x, size);
    cout << "Hashing table: " << endl;
    print_hashing_table(table, size);
    int number;
    cout << "Enter the number: ";
    cin >> number;
    if (!find(number, table, size))
        cout << "Number is not found" << endl;
    else
    {
        cout << "Number is found" << endl;
        list *p = find(number, table, size);
        while(p)
        {
            print_about_people(p->inf);
            del(number, table, size);
            p = find(number, table, size);
        }
        cout << endl; 
        cout << "After delete: " << endl;
        print_hashing_table(table, size);
    }

    return 0;
}