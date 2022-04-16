#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "scanner/testscanner.h"
#include <stdlib.h>
#include "parser/parser.h"
#include "parser/parsetreeprocessing.h"
#include "codeGenerator/codegenerator.h"

using namespace std;
string validateArgs(int argc, char* argv[]);
vector<string> getStrings(string fileName);
vector<string> splitString(string aString, char aDelim);
bool fileExists(string file);
string targetFile = "";
void writeTargetFile(string aTarget);

// Main function calls codeGen
int main(int argc, char* argv[])
{
    string fileName = validateArgs(argc, argv);
    ifstream file;

    if (fileName != "")
    {
        file.open(fileName);

        if (!file.is_open()) // this should not happen
        {
            cerr<<"Failed to open file. exiting... "<<endl;
            exit(EXIT_FAILURE);
        }
        cin.rdbuf(file.rdbuf()); // read cin from file
        //std::cout<<__FILE__<<__LINE__<<std::endl;
    }
    else
    {
        targetFile = "kb.asm";
    }

    Node* root = parser();
    string target = generateTarget(root);
    writeTargetFile(target);
    if (file.is_open()) file.close();

    return 0;
}

void writeTargetFile(string aTarget)
{
    ofstream out(targetFile);
    out << aTarget;
    out.close();
}

string validateArgs(int argc, char* argv[]) {
    if (argc > 2)
    {
        cout << "Too many arguments." << endl;
       exit(0);
    }
    if (argc == 2)
    {
        string file = argv[1];
        if (!fileExists(file+".fs"))
        {
            cout<<"File "<<file<<".fs does not exists, exiting..."<<endl;
            exit(0);
        }
        targetFile = file+".asm";
        return file+".fs";
    }
    return "";
}

bool fileExists(string file)
{
    struct stat buffer;
    return (stat (file.c_str(), &buffer)==0);
}

vector<string> getStrings(string fileName) {
    vector<string> retStrings;
    ifstream file;
    if (fileName.length() > 0)
    {
        file.open(fileName);
        cin.rdbuf(file.rdbuf()); // read cin from file
        if (!file.is_open())
        {
            cout<<"Failed to open "<<fileName<<" exiting..."<<endl;
           exit(0);
        }
    }
    string currLine;
    vector<string> strings;
    while (getline(cin, currLine))
    {
        if (currLine == "EOF" || currLine == "eof") break;
        strings = splitString(currLine, ' ');
        for (string s : strings)
        {
            if((s == "EOF" || s == "eof") && retStrings.size() > 0) return retStrings;
            retStrings.push_back(s);
        }
    }
    if (retStrings.size() > 0) return retStrings;
    else {
        std::cerr<<"File "<<fileName<<" is empty. exiting..."<<std::endl;
        exit(0);
    }
}

vector<string> splitString(string aString, char aDelim) {
    vector<string> strings;
    stringstream ss(aString);
    string token;
    while (getline(ss, token, aDelim)) {
        strings.push_back(token);
    }
    return strings;
}

