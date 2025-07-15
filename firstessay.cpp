#include <stdio.h>
#include <iostream>
#include <stack>
using namespace std;

void player(string name, int age)
{
    std::string::size_type sz; // for string to int conversion
    cout << "Player Name: " << name << endl;
    cout << "Player Age: " << age << endl;
}

int main()
{
    // This program prints "Hello, World!" to the console.
    player("John Doe", 25);
    try
    {
        player("bobi", 0.58);
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }
    catch (...)
    {
        cout << "An unknown error occurred." << endl;
    }
    player("Alice Johnson", 22);
    cout << "Hello, World!" << endl;
    std ::cout << "this is the kindom come.... this is the kindom come....when you feel my dreams" << endl;
    return 0;
}
