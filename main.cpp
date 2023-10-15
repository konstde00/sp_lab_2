#include <iostream>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <locale>

using namespace std;

const int N = 5;

class Automaton
{
public:
    Automaton() {}

    void readFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "File open error!" << filename << endl;
            return;
        }

        string line;

        readAlphabet(file);
        readStates(file);
        readInitialState(file);
        readFinalStates(file);

        while (getline(file, line))
        {
            readTransition(line);
        }

        file.close();
    }

    bool checkPrefix(const string& w0)
    {
        currentState = initialState;
        set <string> words = GenerateWordsOfLength(N);
        for (string w1 : words)
        {
            bool isAutomatonWorking = true;
            string w = w0 + w1;
            for (char s : w)
            {
                if (transitions.count(make_pair(currentState, s)) == 0)
                {
                    isAutomatonWorking = false;
                    break;
                }
                currentState = transitions[make_pair(currentState, s)];
            }
            if(isAutomatonWorking && finalStates.count(currentState) > 0)
            {
                cout << "Automata accepts words of format w = w0w1 = " << w;
                return true;
            }
        }
        return false;
    }

private:
    set<char> alphabet;
    set<int> states;
    int initialState;
    set<int> finalStates;
    int currentState;
    map<pair<int, char>, int> transitions;

    void readAlphabet(ifstream& file)
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        char s;
        while (ss >> s)
        {
            if(s != ' ')
            {
                alphabet.insert(s);
            }
        }
    }

    void readStates(ifstream& file)
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        int state;
        while (ss >> state)
        {
            states.insert(state);
        }
    }

    void readInitialState(ifstream& file)
    {
        file >> initialState;
    }

    void readFinalStates(ifstream& file)
    {
        int numFinalStates;
        file >> numFinalStates;
        for (int i = 0; i < numFinalStates; ++i)
        {
            int finalState;
            file >> finalState;
            finalStates.insert(finalState);
        }
        string line;
        getline(file, line);
    }

    void readTransition(const string& line)
    {
        int fromState, toState;
        char inputSymbol;
        stringstream ss(line);
        ss >> fromState >> inputSymbol >> toState;
        transitions[make_pair(fromState, inputSymbol)] = toState;
    }

    set<string> GenerateWordsOfLength(int n)
    {
        set<string> words;
        for(int i = 0; i <= n; i++)
        {
            generateWords("", initialState, i, words);
        }
        return words;
    }

    void generateWords(const string& prefix, int currentState, int n, set<string>& words)
    {
        if (n == 0)
        {
            words.insert(prefix);
            return;
        }

        for (char s : alphabet)
        {
            if (transitions.count(make_pair(currentState, s)) > 0)
            {
                int nextState = transitions[make_pair(currentState, s)];
                generateWords(prefix + s, nextState, n - 1, words);
            }
        }
    }
};

int main()
{
    setlocale (LC_CTYPE, "ukr");

    Automaton automaton;

    automaton.readFile("../input.txt");

    string w0;
    cout << "Input w0: ";
    cin >> w0;

    try
    {
        if (!automaton.checkPrefix(w0))
        {
            cout << "Automata does not accept words of format w = w0w1";
        }
    }
    catch (const char* e)
    {
        std::cerr << "Error: " << e << '\n';
    }
    return 0;
}
