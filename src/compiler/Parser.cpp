
#include "iostream"
#include "fstream"
#include "string"
#include "CharScanner.cpp"

using namespace std;

void testByChar()
{
    fstream testByCharFile;
    char c;
    testByCharFile.open("./src/test4/TEST.txt", ios::in);
    int lineCount = 1;
    while (!testByCharFile.eof())
    {
        testByCharFile >> std::noskipws >> c;
        if (c == '\n') {
            cout << "eeeeeeeeeeeeeee" << endl;
            lineCount++;
        } else {
            cout << lineCount << ": " << c << endl;
        }
    }
    testByCharFile.close();
}

int main()
{
    // char *filePath = "./src/test4/TEST.txt";
    // ifstream file;
    // file.open(filePath, ios::in);
    // if (file.is_open())
    // {
    //     string line;
    //     while (getline(file, line))
    //     {
    //         if (line.empty())
    //         {
    //             continue;
    //         }
    //         cout << line << endl;
    //     }
    // }
    // else
    // {
    //     cout << "ERROR" << endl;
    // }
    // file.close();
    // testByChar();
    char* path = "./src/test4/TEST.txt";
    CharScanner scanner(path);
    int lineCount = 1;
    while (true)
    {
        char nextChar = scanner.getNextChar();
        if (nextChar == EOF) {
            cout << "EOF" << endl;
            break;
        } else if (nextChar == '\n') {
            lineCount++;
            cout << "enter" << endl;
        } else {
           cout << lineCount << ": " << nextChar << endl;
        }
    }
    
    system("pause");
    return 0;
}