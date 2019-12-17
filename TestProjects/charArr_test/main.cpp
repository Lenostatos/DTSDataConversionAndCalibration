#include <iostream>

using namespace std;

int main()
{
    //char ** charArr = nullptr; // does not work
    char ** charArr = new char * [0];
    charArr[0] = 0;

    cout << charArr << "\n" << (charArr[0] ? "is not nullptr" : "is nullptr") << endl;

    return 0;
}
