#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <iomanip>


using namespace std;


class Validator
{
public:
    void validate(string expression)
    {
        this->expression = expression;
        checkForRestrictedSymbols();
        if (!isError) checkFirstAndLastSymbols();
        if (!isError) checkForBraces();
        if (!isError) checkVariablesNames();
        if (!isError) checkDotPosition();
        if (!isError) checkForRedundantBraces();
        if (!isError) checkOperations();
    }

    bool passed()
    {
        if (expression.empty())
        {
            cout << "Expression is empty!" << endl;
            return false;
        }
        else
        {
            return !isError;
        }
    }
    
private:
    string expression;
    bool isError = false;


    void checkForRestrictedSymbols()
    {
        smatch matches;
        regex allExceptAllowedReg(R"([^\w+\-*/()\.\s])");

        if (regex_search(expression, matches, allExceptAllowedReg))
        {
            cout << "Restricted symbol(-s):  " << matches.str(0) << "\t[at " << matches.position(0) << "]" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
        else
        {
            cout << "No restricted symbols   \tOK" << endl;
        }
    }

    void checkFirstAndLastSymbols()
    {
        regex borderSymbols(R"([+\-(\w\s].*[)\w\s])");

        if (regex_match(expression, borderSymbols))
        {
            cout << "Beginning and ending are fine\tOK" << endl;
        }
        else
        {
            cout << "Unallowed symbol(-s) at the begining or ending!" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
    }

    void checkForBraces()
    {
        string str = expression;
        smatch matches;
        regex braces(R"([()])");

        int openedBraces = 0;
        bool closedBeforeOpening = false;

        if (regex_search(str, matches, braces))
        {
            while (regex_search(str, matches, braces))
            {
                if (matches.str(0) == "(") openedBraces += 1;
                else if (matches.str(0) == ")") openedBraces -= 1;

                if (openedBraces < 0)
                {
                    isError = true;
                    closedBeforeOpening = true;
                    cout << "Closed braces before opening!" << "\t[at " << matches.position(0) << "]" << endl;
                    cout << endl << expression << endl << endl;
                    break;
                }
                
                str = matches.suffix().str(); // removing current match to get next
            }

            if (!closedBeforeOpening){
                if (openedBraces != 0)
                {
                    isError = true;
                    cout << "Unclosed braces had been found!" << "\t[at " << matches.position(0) << "]" << endl;
                    cout << endl << expression << endl << endl;
                }
                else
                {
                    cout << "Braces matching          \tOK" << endl;
                }
            }
        }
        else
        {
            cout << "No braces, skipping        \tOK" << endl;
        }
    }

    void checkVariablesNames()
    {
        smatch matches;
        regex wrongVariables(R"((^|[()+\-/*\s])(\d+[A-Za-z_]\w+))"); // which starting with a number(-s)

        if (regex_search(expression, matches, wrongVariables))
        {
            cout << "Wrong variable name: \t" << matches.str(2) << "\t[at " << matches.position(2) << "]" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
        else
        {
            cout << "Variables are fine        \tOK" << endl;
        }
    }

    void checkDotPosition()
    {
        smatch matches;
        regex wrongDotPosition(R"(((^|[()+\-/*\s])([A-Za-z_]\w*)?\.|(^|[()+\-/*\s])\w*\.[^\d]|\d+\.\d+(\.\d+)+))");

        if (regex_search(expression, matches, wrongDotPosition))
        {
            cout << "Wrong dot placement: \t" << matches.str(0) << "\t[at " << matches.position(0) << "]" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
        else
        {
            cout << "Dot position is fine     \tOK" << endl;
        }
    }

    void checkForRedundantBraces()
    {
        smatch matches;
        regex emptyBraces(R"(\(\s*(\w+|\d+\.\d+)?\s*\))");

        if (regex_search(expression, matches, emptyBraces))
        {
            cout << "Redundant braces:  \t" << matches.str(0) << "\t[at " << matches.position(0) << "]" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
        else
        {
            cout << "No redundant braces      \tOK" << endl;
        }
    }

    void checkOperations()
    {
        smatch matches;
        string reg1 = R"(([+\-*/]{2,}|)"; // 2+ ops together
        string reg2 = R"([+\-*/]+\s+[+\-*/]+|)"; // 2+ ops with spaces
        string reg3 = R"(\(\s*[+*/]|)"; // op after (
        string reg4 = R"([+\-*/]\s*\)|)"; // op before )
        string reg5 = R"([)\w]\s+[\w(]|)"; // no ops among vars/consts/braces with spaces
        string reg6 = R"(\)\(|)"; // no ops among braces
        string reg7 = R"(\w+\(|)"; // no ops among var and brace
        string reg8 = R"(\(\-(\w+|d+\.\d+)\)))"; // single - op in braces
        regex wrongOps(reg1 + reg2 + reg3 + reg4 + reg5 + reg6 + reg7 + reg8);

        if (regex_search(expression, matches, wrongOps))
        {
            cout << "Incorrect usage of operations:\t" << matches.str(0) << "\t[at " << matches.position(0) << "]" << endl;
            cout << endl << expression << endl << endl;
            isError = true;
        }
        else
        {
            cout << "All operations are correct\tOK" << endl << endl;
        }
    }
};


class Token
{
public:
    Token()
    {
        this->token = "";
        this->id = 0;
        this->type = "";
    }

    Token(string token, int id)
    {
        this->token = token;
        this->id = id;
        setType();
    }

    Token & operator= (const Token & other)
    {
        this->token = other.token;
        this->type = other.type;
        this->id = other.id;
        return *this;
    }

    void print()
    {
        cout << token << "\t  " << type << "   \tid " << id << endl;
    }

    string getToken()
    {
        return token;
    }

    string getType()
    {
        return type;
    }

    int getId()
    {
        return id;
    }

private:
    string token;
    string type;
    int id;

    void setType()
    {
        regex operationsRegexp(R"([*/+\-])");
        regex bracesRegexp(R"([()])");
        regex varConstRegexp(R"(\w+\.?\w*)");

        if (regex_match(token, operationsRegexp))
        {
            type = "operation";
        }
        else if (regex_match(token, bracesRegexp))
        {
            type = "brace";
        }
        else if (regex_match(token, varConstRegexp))
        {
            type = "var";
        }
        else
        {
            type = "unknown";
        }
    }
};


class Tokenizer
{
public:
    Tokenizer(string expression)
    {
        this->expression = expression;
        divideIntoTokens();
        printTokens();

        rebuildExpression();
        cout << "Rebuilt with tokens expression:" << endl;
        cout << this->expression << endl << endl;

        cout << "Starting to generate graph layers from the expression:" << endl;
        execLayers();

        // showing input expression with optimization applied
        rebuildExpression();
        optimizeDivision();
        optimizeSubstraction();
        optimizeGroups(); // commutative law

        cout << "------------------------\n";
        cout << "Input expression after the first optimization:\n";
        cout << this->expression << "\n\n";
        cout << "Graph (operation number in braces): \n";
        printGraph();
    }

    void divideIntoTokens()
    {
        string str = expression;
        smatch matches;
        regex regular(R"([*+/\-]|\w+\.?\d*|[()])");

        while (regex_search(str, matches, regular))
        {
            tokens.push_back(Token(matches.str(0), globalId));
            globalId++; // token id
            str = matches.suffix().str(); // removing current match to get next
        }
    }

    void rebuildExpression()
    {
        string expr;
        for (int i = 0; i < tokens.size(); i++)
        {
            expr += tokens[i].getToken();
        }
        this->expression = expr;
    }

    void removeRedundantBraces()
    {
        regex regular(R"(\((\w+|\d+\.\d+)\))");
        do
        {
            expression = regex_replace(expression, regular, "$1");
        } 
        while (regex_search(expression, regular));
    }

    bool searchDivision(int & endExec, map<int, string> & layerDict)
    {
        string str = expression;
        smatch matches;
        regex regular(R"((^|[\+\-\*\(])(\w+\.?\d*/\w+\.?\d*))");

        int matchesNum = 0;
        int prevPartLen = 0;
        while (regex_search(str, matches, regular))
        {
            layerDict.emplace(matches.position(2) + prevPartLen, matches.str(2));
            // cout << "Position=" << matches.position(2) + prevPartLen << " Match: " << matches.str(2) << endl; // print pos

            prevPartLen += matches.prefix().length() + matches.str(0).length();
            
            str = matches.suffix().str(); // removing current match to get next
            matchesNum++;
        }

        if (matchesNum == 0) endExec++;
        return matchesNum > 0;
    }

    void execDivision(int layerNum)
    {
        regex regular(R"((^|[\+\-\*\(])(\w+\.?\d*/\w+\.?\d*))");

        expression = regex_replace(expression, regular, "$1(" + to_string(layerNum) + "L)");
    }

    bool searchMultiplication(int & endExec, map<int, string> & layerDict)
    {
        string str = expression;
        smatch matches;
        regex regular(R"((\w+\.?\d*\*\w+\.?\d*)([\*\-\+\)]|$|/\())");

        int matchesNum = 0;
        int prevPartLen = 0;
        while (regex_search(str, matches, regular))
        {
            layerDict.emplace(matches.position(1) + prevPartLen, matches.str(1));

            prevPartLen += matches.prefix().length() + matches.str(0).length();
            str = matches.suffix().str(); // removing current match to get next
            matchesNum++;
        }

        if (matchesNum == 0) endExec++;
        return matchesNum > 0;
    }

    void execMultiplication(int layerNum)
    {
        regex regular(R"((\w+\.?\d*\*\w+\.?\d*)([\*\-\+\)]|$|/\())");

        expression = regex_replace(expression, regular, '(' + to_string(layerNum) + "L)$2");
    }

    bool searchSubstraction1(int & endExec, map<int, string> & layerDict)
    {
        // A - B
        string str = expression;
        smatch matches;
        regex regular(R"(([\+\(]|^)(\w+\.?\d*\-\w+\.?\d*)([\+\-\)]|$))");

        int matchesNum = 0;
        int prevPartLen = 0;
        
        while (regex_search(str, matches, regular))
        {
            layerDict.emplace(matches.position(2) + prevPartLen, matches.str(2));

            prevPartLen += matches.prefix().length() + matches.str(0).length();

            str = matches.suffix().str(); // removing current match to get next
            matchesNum++;
        }

        if (matchesNum == 0) endExec++;
        return matchesNum > 0;
    }

    void execSubstraction1(int layerNum)
    {
        // A - B
        regex regular(R"(([\+\(]|^)(\w+\.?\d*\-\w+\.?\d*)([\+\-\)]|$))");

        expression = regex_replace(expression, regular, "$1(" + to_string(layerNum) + "L)$3");
    }

    bool searchSubstraction2(int & endExec, map<int, string> & layerDict)
    {
        // -A + B
        string str = expression;
        smatch matches;
        regex regular(R"(\-(\w+\.?\d*)\+(\w+\.?\d*)([\+\-\)]|$))");

        int matchesNum = 0;
        int prevPartLen = 0;
        
        while (regex_search(str, matches, regular))
        {
            layerDict.emplace(matches.position(1) + prevPartLen, matches.str(2) + '-' + matches.str(1));

            prevPartLen += matches.prefix().length() + matches.str(0).length();

            str = matches.suffix().str(); // removing current match to get next
            matchesNum++;
        }

        if (matchesNum == 0) endExec++;
        return matchesNum > 0;
    }

    void execSubstraction2(int layerNum)
    {
        // -A + B
        regex regular(R"(\-(\w+\.?\d*)\+(\w+\.?\d*)([\+\-\)]|$))");

        expression = regex_replace(expression, regular, "+(" + to_string(layerNum) + "L)$3");
        expression = regex_replace(expression, regex(R"(\(\+)"), "(");  // clear redundant +
        expression = regex_replace(expression, regex(R"(^\+)"), "");
    }

    bool searchSummarization(int & endExec, map<int, string> & layerDict)
    {
        string str = expression;
        smatch matches;
        regex regular(R"(([\+\(]|^)(\w+\.?\d*\+\w+\.?\d*)([\+\-\)]|$))");

        int matchesNum = 0;
        int prevPartLen = 0;
        
        while (regex_search(str, matches, regular))
        {
            layerDict.emplace(matches.position(2) + prevPartLen, matches.str(2));

            prevPartLen += matches.prefix().length() + matches.str(0).length();

            str = matches.suffix().str(); // removing current match to get next
            matchesNum++;
        }

        if (matchesNum == 0) endExec++;
        return matchesNum > 0;
    }

    void execSummarization(int layerNum)
    {
        smatch matches;
        string tmp{expression};
        expression.clear();
        regex regular(R"(([\+\(]|^)(\w+\.?\d*\+\w+\.?\d*)([\+\-\)]|$))");

        // expression = regex_replace(expression, regular, "$1(" + to_string(layerNum) + "L)$3");

        while (regex_search(tmp, matches, regular))
        {
            expression += matches.prefix().str() + matches.str(1) + "(" + to_string(layerNum) + "L)";
            tmp = matches.str(3) + matches.suffix().str();
        }
        expression += tmp;
    }

    void numberLoperations()
    {
        int startId = 0;
        regex regular(R"(\d+L([/\*\+\-\)]|$))");
        smatch sm{};
        string tmp{expression};
        expression.clear();

        while (regex_search(tmp, sm, regular))
        {
            expression += string(sm.prefix()) + sm.str(0) + to_string(startId++);
            tmp = sm.suffix();
        }
        expression += tmp;
    }

    void optimizeDivision()
    {
        regex regular1(R"((/\w+\.?\d*/\w+\.?\d*))");
        regex regular2(R"(\{(/)(\w+\.?\d*)(/(\w+\.?\d*))\})");
        regex regular3(R"(/\((\w+\.?\d*(\*\w+\.?\d*)+)\)/\((\w+\.?\d*(\*\w+\.?\d*)+)\))");
        regex regular4(R"(/\((\w+\.?\d*(\*\w+\.?\d*)+)\)/(\w+\.?\d*))");

        expression = regex_replace(expression, regular1, "{$1}");
        expression = regex_replace(expression, regular2, "$1($2*$4)");
        expression = regex_replace(expression, regular3, "/($1*$3)");
        expression = regex_replace(expression, regular4, "/($1*$3)");
    }

    void optimizeSubstraction()
    {
        regex regular1(R"(\-(\w+\.?\d*)\-(\w+\.?\d*)([\-\+\)]|$))");  // -A-B = -(A+B)
        regex regular2(R"(\-\((\w+\.?\d*(\+\w+\.?\d*)+)\)\-(\w+\.?\d*)([\-\+\)]|$))");  // -(A+B+...)-C = -(A+B+...+C)
        regex regular3(R"(\-\((\w+\.?\d*(\+\w+\.?\d*)+)\)\-\((\w+\.?\d*(\+\w+\.?\d*)+)\)([\-\+\)]|$))"); // -(A+B+...)-(C+D+...) = -(A+B+...+C+D+...)

        smatch sm;
        string tmp{expression};
        while (regex_search(tmp, regular1))
        {
            expression = regex_replace(expression, regular1, "-($1+$2)$3");
            tmp = sm.suffix();
        }

        expression = regex_replace(expression, regular2, "-($1+$3)$4");
        expression = regex_replace(expression, regular3, "-($1+$3)$5");
    }

    void optimizeGroups()
    {
        // A+(B/C+(K+M))+D+(B/C+(K+M))
        vector<string> groups;
        regex regular1(R"((\(\-?\w+\.?\d*([\-\+\*/]\w+\.?\d*)+\)))");  // (-A+-*/B...)
        int startId = 0;

        // split on groups
        string expr = expression;
        smatch subm;
        while (regex_search(expr, subm, regex(R"(\(.+\))")))
        {

            string tmp = expression;
            smatch sm{};
            while (regex_search(tmp, sm, regular1))
            {
                groups.push_back(sm.str(1).substr(1, sm.str(1).length()-2));
                tmp = sm.suffix();
            }
            regexReplaceWithId(expression, regular1, startId);

            expr = subm.suffix();
        }
        
        // optimize groups
        for (int i = 0; i < groups.size(); i++)
        {
            optimizeFreeNegativeAdditions(groups[i]);
            optimizeFreePositiveAdditions(groups[i]);
        }

        // optimize main expression
        optimizeFreeNegativeAdditions(expression);
        optimizeFreePositiveAdditions(expression);

        // collect groups
        regex regular2(R"((\d)G)");
        smatch sm2;
        string tmp2{expression};
        expression.clear();
        while (regex_search(tmp2, sm2, regular2))
        {
            expression += string(sm2.prefix()) + "(" + groups[stoi(sm2.str(1))] + ")";
            tmp2 = string(sm2.suffix());
        }
        expression += tmp2;
    }

    void execLayers()
    {   
        int layerNum = 0;
        
        int endExec = 0;
        while (endExec < 5) // there is 5 possible operations (including 2 variants of substraction)
        {
            endExec = 0;

            cout << endl << "--- Layer " << layerNum << ":\n"; 

            cout << endl << "Input expression:\n";
            cout << this->expression + "\n";
            
            optimizeDivision();
            optimizeSubstraction();
            optimizeGroups(); // commutative law

            cout << "Optimized expression:\n";
            cout << this->expression + "\n";

            // creating ordered dictionary and putting operations in it
            map<int, string> orderedDictLayer;

            vector<bool> opsToExec {0, 0, 0, 0, 0};
            if (searchDivision(endExec, orderedDictLayer))      {opsToExec[0] = true;}
            if (opsToExec[0]) {execDivision(layerNum);}
            if (searchMultiplication(endExec, orderedDictLayer)){opsToExec[1] = true;}
            if (opsToExec[1]) {execMultiplication(layerNum);}
            if (searchSubstraction1(endExec, orderedDictLayer)) {opsToExec[2] = true;}
            if (opsToExec[2]) {execSubstraction1(layerNum);}
            if (searchSubstraction2(endExec, orderedDictLayer)) {opsToExec[3] = true;}
            if (opsToExec[3]) {execSubstraction2(layerNum);}
            if (searchSummarization(endExec, orderedDictLayer)) {opsToExec[4] = true;}
            if (opsToExec[4]) {execSummarization(layerNum);}
            
            cout << "Extracted operations:\n";
            // creating layer and filling it up with dictionary
            vector<string> layer;
            for (pair<int, string> operation : orderedDictLayer)
            {
                layer.push_back(operation.second);
                cout << operation.second + "\t";
            }
            cout << "\n";

            // pushing layer to the end of graph
            graph.push_back(layer);
            
            numberLoperations();
            removeRedundantBraces();
            layerNum++;

            // cout << endl << "Result expression:" << endl;
            // cout << this->expression << endl;
        }
        graph.pop_back();
    }

    void printTokens()
    {
        for (int i = 0; i < tokens.size(); i++)
        {
            tokens[i].print();
        }
        cout << endl;
    }

    void printGraph()
    {
        int opNum = 0;
        for (int i = 0; i < graph.size(); i++)
        {
            cout << "Layer " + to_string(i) + ":\t";
            for (int j = 0; j < graph[i].size(); j++)
            {
                cout << graph[i][j] << " (" << opNum++ << ") " << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    vector<vector<string>> getGraph()
    {
        return graph;
    }

private:
    string expression;
    vector<Token> tokens;
    int globalId = 0;

    vector<vector<string>> graph;

    void regexReplaceWithId(string& str, const regex& regular, int& startId)
    {
        smatch sm{};
        string tmp{str};
        str.clear();

        while (regex_search(tmp, sm, regular))
        {
            str += string(sm.prefix()) + to_string(startId++) + "G";
            tmp = sm.suffix();
        }
        str += tmp;
    }

    void optimizeFreeNegativeAdditions(string& group)
    {
        regex regular1(R"((\-(\d+L\d+|[A-Za-z_]\w*|\d+\.?\d*))([\-\+]))"); // -A
        smatch sm{};
        string tmp{group};
        group.clear();

        vector<string> freeAds;

        while (regex_search(tmp, sm, regular1))
        {
            freeAds.push_back(sm.str(1));
            group += string(sm.prefix());
            tmp = sm.str(3) + string(sm.suffix());
        }
        group += tmp;
        
        for (int i = 0; i < freeAds.size(); i++)
        {
            group += freeAds[i];
        }
        if (group.substr(0, 1).compare("+") == 0) group = group.substr(1, group.length()-1); // remove + at the begininng
    }

    void optimizeFreePositiveAdditions(string& group)
    {
        regex regular1(R"((^|\+)(\d+L\d+|[A-Za-z_]\w*|\d+\.?\d*)([\-\+]))"); // +A
        smatch sm{};
        string tmp{group};
        group.clear();

        vector<string> freeAds;

        while (regex_search(tmp, sm, regular1))
        {
            freeAds.push_back(sm.str(2));
            group += string(sm.prefix());
            tmp = sm.str(3) + string(sm.suffix());
        }
        group += tmp;

        for (int i = 0; i < freeAds.size(); i++)
        {
            group += "+" + freeAds[i];
        }
        if (group.substr(0, 1).compare("+") == 0) group = group.substr(1, group.length()-1); // remove + at the begininng
    }

};


class VectorSystem
{
public:
    VectorSystem(vector<vector<string>>& graph, string expression)
    {
        this->expression = expression;
        this->graph = graph;

        fillDependencyGraph();
        setStartingTimeOnDepGraph();
        calcCoefs();
        printDependencies();

    }

    void fillDependencyGraph()
    {
        regex reOp(R"(\w+\.?\d*([\+\-\*/])\w+\.?\d*)"); // get op
        regex reDep(R"((^|[/\*\-\+])(\d+)L(\d+)($|[/\*\-\+]))"); // get dependencies (2 - layer, 3 - op)

        int opNum = 0;
        vector<int> layerLenths;
        for (int i = 0; i < graph.size(); i++)
        {
            int layerLen = 0;
            for (int j = 0; j < graph[i].size(); j++)
            {
                opNum++;
                layerLen++; //  op  d1 d2 ready start core
                vector<int> dep {0, -1, -1, 0, 0, 0};

                string str2 = graph[i][j];
                smatch sm2;
                regex_search(str2, sm2, reOp);

                if (sm2.str(1).compare("/") == 0)
                {
                    dep[0] = 0;
                }
                else if (sm2.str(1).compare("*") == 0)
                {
                    dep[0] = 1;
                }
                else if (sm2.str(1).compare("-") == 0)
                {
                    dep[0] = 3; // interpret - as + 
                }
                else if (sm2.str(1).compare("+") == 0)
                {
                    dep[0] = 3;
                }


                string str = graph[i][j];
                smatch sm;
                int depNum = 1;
                while (regex_search(str, sm, reDep))
                {
                    int depOpNo = 0;
                    for (int k = 0; k < stoi(sm.str(2)); k++) // prev ops on layers
                    {
                        depOpNo += layerLenths[k];
                    }
                    depOpNo += stoi(sm.str(3)); // op num on current layer

                    dep[depNum] = depOpNo;

                    str = sm.suffix();
                    depNum++;
                }
                
                depGraph.push_back(dep);

                // print out
                // for (size_t r = 0; r < dep.size(); r++)
                // {
                //     cout << dep[r] << " ";
                // }
                // cout << endl;
            }
            layerLenths.push_back(layerLen);
        }
    }

    void setStartingTimeOnDepGraph()
    {
        int last = depGraph.size()-1;

        vector<vector<int>> opTimers; // 0  0    00
        for (size_t i = 0; i < operations.size(); i++)
        {
            opTimers.push_back(vector<int>(cores[i]));
        }

        for (int i = 0; i <= last; i++)
        {
            if (depGraph[i][3] == 0)
            {
                int firstDep = 0;
                int secondDep = 0;
                int currentCore = 0;

                int tmpTimer = opTimers[depGraph[i][0]][0];
                // for op cores choose one that has lowest timer
                for (int c = 0; c < opTimers[depGraph[i][0]].size(); c++)
                {
                    if (opTimers[depGraph[i][0]][c] < tmpTimer)
                    {
                        currentCore = c;
                        tmpTimer = opTimers[depGraph[i][0]][c];
                    }
                }
                

                // First dependency
                if (depGraph[i][1] == -1)
                {
                    firstDep = 1;
                    depGraph[i][4] = opTimers[depGraph[i][0]][currentCore];
                }
                else if (depGraph[depGraph[i][1]][3] == 1)
                {
                    firstDep = 1;
                    // dep start tact + dep time
                    depGraph[i][4] = depGraph[depGraph[i][1]][4] + opLayers[depGraph[depGraph[i][1]][0]];

                    if (depGraph[i][4] < opTimers[depGraph[i][0]][currentCore])
                    {
                        depGraph[i][4] = opTimers[depGraph[i][0]][currentCore];
                    }
                    
                }

                // Second dependency
                if (depGraph[i][2] == -1)
                {
                    secondDep = 1;
                    if (depGraph[i][4] < opTimers[depGraph[i][0]][currentCore])
                    {
                        depGraph[i][4] = opTimers[depGraph[i][0]][currentCore];
                    }
                    
                }
                else if (depGraph[depGraph[i][2]][3] == 1)
                {
                    secondDep = 1;
                    // dep start tact + dep time
                    int secondDepEndTime = depGraph[depGraph[i][2]][4] + opLayers[depGraph[depGraph[i][2]][0]];
                    if (secondDepEndTime > depGraph[i][4])
                    {
                        depGraph[i][4] = secondDepEndTime;
                    }

                    if (opTimers[depGraph[i][0]][currentCore] < secondDepEndTime)
                    {
                        opTimers[depGraph[i][0]][currentCore] = secondDepEndTime;
                    }
                    
                }

                // Marking dep as ready
                if (firstDep == 1 && secondDep == 1)
                {
                    depGraph[i][3] = 1;
                    opTimers[depGraph[i][0]][currentCore]++;
                    depGraph[i][5] = currentCore;
                }
            }
        }
        
        // print
        // for (size_t i = 0; i < depGraph.size(); i++)
        // {
        //     for (size_t j = 0; j < depGraph[i].size(); j++)
        //     {
        //         cout << depGraph[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
    }

    void calcCoefs()
    {
        singleCoreTimer = 0;
        for (size_t i = 0; i < depGraph.size(); i++)
        {
            singleCoreTimer += opLayers[depGraph[i][0]];
        }
        
        vectorSysTimer = depGraph.back()[4] + opLayers[depGraph.back()[0]];
        cout << "Result execution time = " << vectorSysTimer << endl;
        cout << "Single thread time    = " << singleCoreTimer << endl;
        cout << "Acceleration factor   = " << float(singleCoreTimer)/vectorSysTimer << endl;
        layersNum = cores[0]*opLayers[0] + cores[1]*opLayers[1] + cores[2]*opLayers[2] + cores[3]*opLayers[3];
        cout << "Efficiency factor = " << (float(singleCoreTimer)/vectorSysTimer)/layersNum << endl;
        cout << endl;
    }

    void printDependencies()
    {
        vector<vector<string>> depView;
        for (size_t i = 0; i <= vectorSysTimer; i++)
        {
            depView.push_back(vector<string>(layersNum));
        }
        
        for (size_t i = 0; i < depGraph.size(); i++)
        {
            int startCol = 0;
            int startRow = depGraph[i][4];

            for (size_t j = 0; j < depGraph[i][0]; j++)
            {
                startCol += opLayers[j] * cores[j];
            }
            startCol += depGraph[i][5] * opLayers[depGraph[i][0]];
            
            for (size_t j = 0; j < opLayers[depGraph[i][0]]; j++)
            {
                depView[startRow][startCol] += to_string(i);
                startRow++;
                startCol++;
            }
        }
        
        // print operation labels
        cout << "Load in vector system:" << endl;
        for (int i = 0; i < cores.size(); i++)
        {
            for (int j = 0; j < cores[i]; j++)
            {
                for (int k = 0; k < opLayers[i]; k++)
                {
                    cout << setw(3) << operations[i];
                }
            }
        }
        cout << endl;
        for (int i = 0; i < cores.size(); i++)
        {
            for (int j = 0; j < cores[i]; j++)
            {
                for (int k = 0; k < opLayers[i]; k++)
                {
                    cout << "---";
                }
            }
        }
        cout << endl;

        int timer = 1;
        // print dependencies
        for (size_t i = 0; i < vectorSysTimer; i++)
        {
            for (size_t j = 0; j < layersNum; j++)
            {
                cout << setw(3) << depView[i][j];
            }
            cout << " | " << to_string(timer++) << endl;

        }
        cout << endl;
    }

private:
    string expression;
    vector<string> operations {"/", "*", "-", "+"};
    vector<int>         cores { 1,   1,   0,   2 };  // specialized conveyors
    vector<int>      opLayers { 4,   3,   2,   2 };  // layers
    vector<vector<string>> graph;
    vector<vector<int>> depGraph;
    vector<vector<pair<string, int>>> dataGraph;
    vector<vector<vector<string>>> taskView;
    vector<int> opTime { 1,   1,   1,   1 };
    int singleCoreTimer = 0;
    int vectorSysTimer = 0;
    int layersNum = 0;
};


int main()
{
    
    // string expression = "888*317-K+(785*P+535)/50+F+197*(356-72/6+T)"; // #3
    // string expression = "a/b/c/d+m*(z-k*4-f-r)+5+n*x*y*l-e"; // #2
    string expression = "a+b*(c-d-e+(l+k)/t)";  // #1
    // string expression = "a/b/c/d+m*(z-k*4-f-r)+5+n*x*y*l-e";

    cout << "User expression:" << endl;
    cout << expression << endl << endl;

    Validator validator;
    validator.validate(expression);

    if (validator.passed())
    {
        Tokenizer tokenizer(expression);

        vector<vector<string>> graph = tokenizer.getGraph();

        VectorSystem vecSys(graph, expression);
    }

    return 0;
}