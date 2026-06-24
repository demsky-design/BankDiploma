#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class User
{
public:
    string name;
    int pin;
    int balance;

    string history[100];
    int countHistory = 0;
};

User users[100];

int countUsers = 0;
int currentUser = -1;
/*загрузка при запуске*/
void loadUsers()
{
    ifstream file("users.txt");

    while (
        file
        >> users[countUsers].name
        >> users[countUsers].pin
        >> users[countUsers].balance
        )
    {
        countUsers++;
    }

    file.close();
}
/*сохранить в файл*/
void saveUsers()
{
    ofstream file("users.txt");

    for (int i = 0; i < countUsers; i++)
    {
        file
            << users[i].name << endl
            << users[i].pin << endl
            << users[i].balance << endl;
    }

    file.close();

    cout << "Данные сохранены\n";
}
/*переводы */
void transfers()
{
    string nametr;
    int moneytr;

    cout << "Введите имя человека: ";
    cin >> nametr;

    cout << "Введите сумму: ";
    cin >> moneytr;

    if (moneytr <= 0)
    {
        cout << "Неверная сумма\n";
        return;
    }

    for (int i = 0; i < countUsers; i++)
    {
        if (users[i].name == nametr)
        {
            if (i == currentUser)
            {


                cout << "Нельзя перевести самому себе\n";
                return;
            }

            if (users[currentUser].balance >= moneytr)
            {
                users[currentUser].balance -= moneytr;

                users[i].balance += moneytr;

                users[currentUser]
                    .history[users[currentUser].countHistory]
                    =
                    "Перевод -> "
                    + nametr
                    + " -"
                    + to_string(moneytr);

                users[currentUser].countHistory++;


                users[i]
                    .history[users[i].countHistory]
                    =
                    "Получено <- "
                    + users[currentUser].name
                    + " +"
                    + to_string(moneytr);

                users[i].countHistory++;

                cout << "Перевод выполнен\n";

                return;
            }

            cout << "Недостаточно денег\n";
            return;
        }
    }

    saveUsers();

    cout << "Пользователь не найден\n";

}
/*зарегаться*/
void registerUsers()
{
    cout << "Введите имя: ";
    cin >> users[countUsers].name;

    cout << "Введите пинкод: ";
    cin >> users[countUsers].pin;

    cout << "Введите стартовый баланс: ";
    cin >> users[countUsers].balance;

    countUsers++;

    cout << "Аккаунт создан\n";

    saveUsers();
}
/*войти в акк*/
bool login()
{
    string name;
    int pincode;

    int invalidtimes = 0;

    while (invalidtimes < 3)
    {
        cout << "Введите имя: ";
        cin >> name;

        cout << "Введите пинкод: ";
        cin >> pincode;

        for (int i = 0; i < countUsers; i++)
        {
            if (
                users[i].name == name &&
                users[i].pin == pincode
                )
            {
                currentUser = i;

                cout << "Вход выполнен\n";

                return true;
            }
        }

        invalidtimes++;

        cout << "Неверные данные\n";
    }
    cout << "Аккаунт заблокирован\n";

    return false;
}
/*показать историю*/
void ShowHistory()
{
    if (users[currentUser].countHistory == 0)
    {
        cout << "История пуста\n";

        return;
    }

    for (
        int i = 0;
        i < users[currentUser].countHistory;
        i++
        )
    {
        cout
            << users[currentUser].history[i]
            << endl;
    }
}
/*менюшка подглавная*/
void menu()
{
    int choice;

    while (true)
    {
        cout << "\n===== МЕНЮ =====\n";

        cout << "1 Баланс\n";
        cout << "2 Снять\n";
        cout << "3 Пополнить\n";
        cout << "4 История\n";
        cout << "5 Перевод\n";
        cout << "0 Выход\n";


        cin >> choice;

        if (choice == 1)
        {
            cout
                << "Баланс: "
                << users[currentUser].balance
                << endl;
        }

        else if (choice == 2)
        {
            int summa;

            cout << "Введите сумму: ";
            cin >> summa;

            if (
                summa > 0 &&
                summa <= users[currentUser].balance
                )
            {
                users[currentUser].balance -= summa;

                users[currentUser]
                    .history[users[currentUser].countHistory]
                    =
                    "Снятие -"
                    + to_string(summa);

                users[currentUser].countHistory++;

                cout
                    << "Остаток: "
                    << users[currentUser].balance
                    << endl;
            }

            else
            {
                cout << "Недостаточно средств\n";
            }

            saveUsers();
        }

        else if (choice == 3)
        {
            int sum;

            cout << "Введите сумму: ";
            cin >> sum;

            if (sum > 0)
            {
                users[currentUser].balance += sum;

                users[currentUser]
                    .history[users[currentUser].countHistory]
                    =
                    "Пополнение +"
                    + to_string(sum);

                users[currentUser].countHistory++;

                cout
                    << "Баланс: "
                    << users[currentUser].balance
                    << endl;
            }

            saveUsers();
        }

        else if (choice == 4)
        {
            ShowHistory();
        }

        else if (choice == 5)
        {
            transfers();
        }

        else if (choice == 0)
        {
            break;
        }
    }
}

int main()
{
    setlocale(0, "rus");

    int action;
    /*главная менюшка*/
    while (true)
    {
        cout << "\n===== БАНК =====\n";

        cout << "1 Регистрация\n";
        cout << "2 Вход\n";
        cout << "0 Выход\n";

        cin >> action;

        if (action == 1)
        {
            registerUsers();
        }

        else if (action == 2)
        {
            if (login())
            {
                menu();
            }
        }

        else if (action == 0)
        {
            break;
        }
    }

    return 0;
}