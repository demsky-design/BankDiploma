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

bool loginExists(string name)/*уникальные имена*/
{
    for (int i = 0; i < countUsers; i++)
    {
        if (users[i].name == name)
        {
            return true;
        }
    }

    return false;
}

void saveUsers()/*сохранение пользователей*/
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
}

void loadUsers()/*загрузка пользователей*/
{
    ifstream file("users.txt");

    countUsers = 0;

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

void registerUsers()
{
    string name;

    while (true)
    {
        cout << "Введите имя: ";
        cin >> name;

        if (!loginExists(name))
            break;

        system("cls");
        cout << "Такой логин уже существует.\n";
    }

    users[countUsers].name = name;

    while (true)
    {
        try
        {
            cout << "Введите пинкод: ";
            cin >> users[countUsers].pin;

            cout << "Введите стартовый баланс: ";
            cin >> users[countUsers].balance;

            break;
        }
        catch (ios_base::failure&)
        {
            cout << "Ошибка ввода! Используйте только цифры.\n";

            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    countUsers++;

    system("cls");
    cout << "Аккаунт создан.\n";

    saveUsers();
}
bool correctName(string name)
{
    for (int i = 0; i < name.length(); i++)
    {
        if (isdigit(name[i]))
        {
            return false;
        }
    }
    return true;
}
bool login()
{
    string name;
    int pincode;

    while (true)
    {
        try
        {
            cout << "Введите имя: ";
            cin >> name;

            cout << "Введите пинкод: ";
            cin >> pincode;

            break;
        }
        catch (ios_base::failure&)
        {
            cout << "Ошибка! Пинкод должен состоять только из цифр.\n";

            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    for (int i = 0; i < countUsers; i++)
    {
        if (users[i].name == name &&
            users[i].pin == pincode)
        {
            currentUser = i;

            system("cls");
            cout << "Вход выполнен.\n";
            return true;
        }
    }

    system("cls");
    cout << "Неверное имя или пинкод.\n";

    return false;
}
void ShowHistory()
{
    if (users[currentUser].countHistory == 0)
    {
        cout << "История пуста.\n";
        return;
    }

    system("cls");

    cout << "===== История операций =====\n";

    for (int i = 0; i < users[currentUser].countHistory; i++)
    {
        cout << i + 1 << ". " << users[currentUser].history[i] << endl;
    }
}
void pincodeChange()
{
    int oldPin;
    int newPin;

    while (true)
    {
        try
        {
            cout << "Введите текущий пинкод: ";
            cin >> oldPin;

            break;
        }
        catch (ios_base::failure&)
        {
            cout << "Введите только цифры!\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    if (oldPin != users[currentUser].pin)
    {
        system("cls");
        cout << "Неверный текущий пинкод\n";
        return;
    }

    while (true)
    {
        try
        {
            cout << "Введите новый пинкод: ";
            cin >> newPin;

            break;
        }
        catch (ios_base::failure&)
        {
            cout << "Введите только цифры!\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    users[currentUser].pin = newPin;

    system("cls");
    cout << "Пинкод успешно изменен\n";

    saveUsers();
}
void nameChange()
{
    string oldname;
    string newname;

    cout << "Введите текущее имя: ";
    cin >> oldname;

    if (oldname != users[currentUser].name)
    {
        system("cls");
        cout << "Неверное текущее имя\n";
        return;
    }

    while (true)
    {
        cout << "Введите новое имя: ";
        cin >> newname;

        if (!correctName(newname))
        {
            cout << "Имя не должно содержать цифры!\n";
            continue;
        }

        if (loginExists(newname))
        {
            cout << "Такой логин уже существует!\n";
            continue;
        }

        break;
    }

    users[currentUser].name = newname;

    system("cls");
    cout << "Имя успешно изменено\n";

    saveUsers();
}
void transfers() /*переводы*/
{
    string nametr;
    int moneytr;

    cout << "Введите имя получателя: ";
    cin >> nametr;

    cout << "Введите сумму: ";
    cin >> moneytr;

    if (moneytr <= 0)
    {
        system("cls");
        cout << "Неверная сумма\n";
        return;
    }

    for (int i = 0; i < countUsers; i++)
    {
        if (users[i].name == nametr)
        {
            if (i == currentUser)
            {
                system("cls");
                cout << "Нельзя перевести самому себе\n";
                return;
            }

            if (users[currentUser].balance < moneytr)
            {
                system("cls");
                cout << "Недостаточно средств\n";
                return;
            }

            users[currentUser].balance -= moneytr;
            users[i].balance += moneytr;

            users[currentUser].history[users[currentUser].countHistory++] =
                "Перевод -> " + nametr + " -" + to_string(moneytr);
            users[i].history[users[i].countHistory++] =
                "Получено <- " + users[currentUser].name + " +" + to_string(moneytr);

            system("cls");
            cout << "Перевод выполнен\n";

            saveUsers();
            return;
        }
    }
    system("cls");
    cout << "Пользователь не найден\n";
}
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
        cout << "6 Сменить пинкод\n";
        cout << "7 Сменить имя\n";
        cout << "0 Выход из аккаунта\n";

        cin >> choice;

        if (choice == 1)
        {
            system("cls");
            cout << "Баланс: " << users[currentUser].balance << endl;
        }

        else if (choice == 2)
        {
            int sum;
            cout << "Введите сумму: ";
            cin >> sum;

            if (sum > 0 && sum <= users[currentUser].balance)
            {
                users[currentUser].balance -= sum;

                users[currentUser].history[users[currentUser].countHistory++] =
                    "Снятие -" + to_string(sum);
                system("cls");
                cout << "Готово. Баланс: " << users[currentUser].balance << endl;
                saveUsers();
            }
            else
            {
                system("cls");
                cout << "Недостаточно средств\n";
            }
        }

        else if (choice == 3)
        {
            int sum;
            cout << "Введите сумму: ";
            cin >> sum;

            if (sum > 0)
            {
                users[currentUser].balance += sum;

                users[currentUser].history[users[currentUser].countHistory++] =
                    "Пополнение +" + to_string(sum);

                system("cls");
                cout << "Баланс: " << users[currentUser].balance << endl;

                saveUsers();
            }
            else
            {
                system("cls");
                cout << "Сумма должна быть больше нуля!\n";
            }
        }

        else if (choice == 4)
        {
            ShowHistory();
        }

        else if (choice == 5)
        {
            transfers();
        }

        else if (choice == 6)
        {
            pincodeChange();
        }

        else if (choice == 7)
        {
            nameChange();
        }

        else if (choice == 0)
        {
            currentUser = -1;
            break;
        }
    }
}

void menuuu()
{
    int action;

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
}

int main()
{
    setlocale(0, "rus");
    cin.exceptions(ios::failbit);

    loadUsers();

    menuuu();

    saveUsers();

    return 0;
}