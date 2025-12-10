#include <iostream>
using namespace std;

int main() {
    string word;

    cout << "Enter a word: ";
    cin >> word;

    // Print the first character
    cout << "The first letter is: " << word[0] << endl;
    cout << word;

    return 0;
}