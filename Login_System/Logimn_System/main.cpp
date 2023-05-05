#include <iostream>
#include <fstream>
#include <string>
#include <bcrypt.h>

using namespace std;

// Функція для введення пароля з прихованням введення
string GetPasswordInput()
{
    string password;
    char ch;
    while ((ch = _getch()) != 13) // 13 - ASCII-код клавіші Enter
    {
        if (ch == 8 && password.length() > 0) // 8 - ASCII-код клавіші Backspace
        {
            cout << "\b \b";
            password.pop_back();
        }
        else if (ch != 8)
        {
            cout << "*";
            password.push_back(ch);
        }
    }
    cout << endl;
    return password;
}

// Функція для перевірки наявності користувача в файлі
bool UserExists(string username)
{
    ifstream read("C:\\" + username + ".txt");
    return read.good();
}

// Функція для реєстрації нового користувача
bool RegisterUser()
{
    string username, password;

    cout << "Select a username: ";
    cin >> username;

    if (UserExists(username))
    {
        cout << "User with this username already exists." << endl;
        return false;
    }

    cout << "Select a password: ";
    password = GetPasswordInput();
    string hash = BCrypt::generateHash(password);

    ofstream file("C:\\" + username + ".txt");
    if (!file.is_open())
    {
        cout << "Failed to create user." << endl;
        return false;
    }
    file << username << endl << hash;
    file.close();

    cout << "Successfully registered user." << endl;
    return true;
}

// Функція для перевірки введення користувача та пароля
bool IsLoggedIn()
{
    string username, password, un, pw;

    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    password = GetPasswordInput();

    ifstream read("C:\\" + username + ".txt");
    getline(read, un);
    getline(read, pw);

    if (un == username && BCrypt::validatePassword(password, pw))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    int choice;

    cout << "1: Register\n2: Login\nYour choice: ";
    cin >> choice;

    if (choice == 1)
    {
        RegisterUser();
        main();
    }
    else if (choice == 2)
    {
        bool status = IsLoggedIn();

        if (!status)
        {
            cout << "Incorrect username or password." << endl;
            return 0;
        }
        else
        {
            cout << "Successfully logged in." << endl;
            return 1;
        }
    }

    return 0;
}
