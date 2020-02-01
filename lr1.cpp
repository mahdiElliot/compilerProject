#include<iostream>
#include<fstream>
#include <iostream>
#include<sstream>
#include<vector>
#include<math.h>
#include <map> 
#include <iterator> 

using namespace std;

#define tslang "tslang.txt"
#define tslang_grammer "grammer.txt"

int line = 1;//line generator
int label = 1; // label generator
int reg = 0;//register generator

bool grammerError = false;

const short T_num = 1;
const short T_list = 2;

//node for a variable or a function
struct symbolTable{
    string variable;
    vector<double> value;
    int len = value.size()+1;
    short type;
    bool isFunc;
    short numberOfParams;
    int reg = -1;
    map<string , symbolTable> list;
};


//type and val of a expression
struct exprVal{
    string tok;
    short type;
    vector<double> val;
    int len = val.size()+1;
    int lenReg;
    int reg = -1;
    int pos=0;
};

struct terminal_nonTerminal{
    bool isTerminal;
    string var;
};

struct grammer{
    vector<terminal_nonTerminal> right;
    int dot = 0;
    string left;
    vector<terminal_nonTerminal> lookahead;

    bool checkForClosure = false;
};

struct itemSet{ //node
    vector<grammer> grammers;
};

struct edge{
    int src,dest;
    string term_nonterm;    
};

vector<grammer> grammersList; //main grammers

/***************************************************************/
/***************************************************************/

bool greaterThan(char gt)
{
    return gt == '>';
}

bool lessThan(char lt)
{
    return lt == '<';
}

bool equal(char eq)
{
    return eq == '=';
}

bool comma(char c)
{
    return c == ',';
}
 bool colon(char d)
 {
     return d == ':';
 }

 bool semiColon(char s)
 {
     return s == ';';
 }

bool isDigit(char digit)
{
    return digit >= '0' && digit <= '9';
}

bool openBrace(char obrace)
{
    return obrace == '{';
}

bool closeBrace(char cbrace)
{
    return cbrace == '}';
}

bool openParenthesis(char op)
{
    return op == '(';
}

bool closeParenthesis(char cp)
{
    return cp == ')';
}

bool openBracket(char b)
{
    return b == '[';
}

bool closeBracket(char b)
{
    return b == ']';
}

bool add(char p)
{
    return p == '+';
}

bool sub(char s)
{
    return s == '-';
}

bool mul(char m)
{
    return m == '*'; 
}

bool div(char d)
{
    return d == '/';
}

bool mod(char m)
{
    return m == '%';
}

bool doubleQ(char dq)
{
    return dq == '\"';
}

bool singleQ(char sq)
{
    return sq == '\'';
}

bool And(char a)
{
    return a == '&';
}

bool Or(char o)
{
    return o == '|';
}

bool Not(char n)
{
    return n == '!';
}

bool signs(char c)
{
    return greaterThan(c) || lessThan(c) || equal(c) ||
     comma(c) || colon(c) || semiColon(c) || openBrace(c) ||
     closeBrace(c) || openParenthesis(c) ||
     closeParenthesis(c) || openBracket(c) ||
     closeBracket(c) || add(c) || sub(c) ||
     mul(c) || div(c) || mod(c) || doubleQ(c) ||
     singleQ(c) || And(c) || Not(c) || c == '.' || c == '^';

}


bool Main(string word)          { return word == "main"; }

bool If(string word)            { return word == "if"; }

bool Else(string word)          { return word == "else"; }

bool For(string word)           { return word == "for"; }

bool While(string word)         { return word == "while"; }

bool In(string word)            { return word == "in"; }

bool Return(string word)        { return word == "return";}

bool num(string word)           { return word == "num"; }

bool list(string word)          { return word == "list"; }

bool local(string word)         { return word == "local"; }

bool numread(string word)       { return word == "numread"; }

bool numprint(string word)      { return word == "numprint"; }

bool vec(string word)           { return word == "vec"; }

bool veclen(string word)        { return word == "veclen"; } 

bool exit(string word)          { return word == "exit"; }

bool isKey(string word)
{
    return Main(word) || If(word) || Else(word) || For(word) || While(word) || In(word)
        || Return(word) || num(word) || list(word) || local(word) || numread(word)
        || numprint(word) || vec(word) || veclen(word) || exit(word);
}

bool isWord(string letter)
{
    bool valid=false;
    if((int(letter[0])>=65 && int(letter[0])<=90) || 
    (int(letter[0])>=97 && int(letter[0]<=122))
    || letter[0] == '_')
    {
        if(letter.size() == 1) valid=true;
            for(int i=1 ; i<letter.size() ; i++)
                if(isDigit(letter[i]) || (int(letter[i])>=65 && int(letter[i])<=90) ||
                 (int(letter[i])>=97 && int(letter[i]<=122)) || letter[i] == '_' )
                    valid=true;
                else
                return false;
    }

    return valid;
}

bool isIntNumber(string number)
{
    double num;
    stringstream iss(number);
    iss>>num;

    if(num>= -2147483648 && num<= 2147483647)
    {
        for(int i=0; i<number.size(); i++)
            if(number[i] != '0' && number[i] != '1' && number[i] != '2' && number[i] != '3' && number[i] != '4' && 
            number[i] != '5' && number[i] != '6' && number[i] != '7' && number[i] != '8' && number[i] != '9' && number[i] !='.')
                return false;

        return true;
    }
    
    return false;
}

bool isDoubleNumber(string number)
{
    double num;
    stringstream iss(number);
    iss>>num;

    if(num>= -1.7 * pow(10,308)  && num<= 1.7 * pow(10,308))
    {
        for(int i=0; i<number.size(); i++)
            if(number[i] != '0' && number[i] != '1' && number[i] != '2' && number[i] != '3' && number[i] != '4' && 
            number[i] != '5' && number[i] != '6' && number[i] != '7' && number[i] != '8' && number[i] != '9' && number[i] !='.')
                return false;

        return true;
    }
    
    return false;
}

bool isNumber(string number)
{
    return isIntNumber(number) || isDoubleNumber(number);
}

string nextToken(ifstream &myfile)
{
    char c;
    string token = "";

    if(!myfile.eof())
    {
        myfile.get(c);
        short a = (short) c;
        while((c == ' ' || c == '\n' || c == '#' || c == '\r') && !myfile.eof())    
        {
            if(c == '#')
            {
                myfile.get(c);
                while((c != '\r') && !myfile.eof())    myfile.get(c);
            }
            if(c == '\n')   line++;
            myfile.get(c);
        }

        if(!myfile.eof())
        {
            if((c>='a' && c<= 'z') || (c>='A' && c<='Z') ||
                isDigit(c) || c == '_')
            {
                if(c == '_')
                {
                    char c1;
                    myfile.get(c1);
                    if((c == ' ' || c == '\n' || c == '\r')
                     && !myfile.eof())
                        return "_";

                    myfile.unget();
                }
                token ="";

                while(((c>= 'a' && c<='z') || (c>='A' && c<='Z') ||
                    isDigit(c) || c == '_') && !myfile.eof())
                {
                    token = token + c;
                    myfile.get(c);
                }
                
                if(!myfile.eof())   
                {
                    myfile.unget();
                }
                
            }
            else if(comma(c))   return ",";
            else if(c == '~')   return "~";
            else if(semiColon(c))    return ";";
            else if(openBrace(c))    return "{";
            else if(closeBrace(c))   return "}";
            else if(openParenthesis(c))  return "(";
            else if(closeParenthesis(c)) return ")";
            else if(openBracket(c))  return "[";
            else if(closeBracket(c)) return "]";
            else if(add(c))          return "+";
            else if(sub(c))          return "-";
            else if(mul(c))          return "*";
            else if(div(c))          return "/";
            else if(mod(c))          return "%";
            else if(c == '^')
            {
                char c1;
                myfile.get(c1);
                if(semiColon(c1))
                    return "^;";
                
                return "^";
            }
            else if (colon(c))
            {
                char c1;
                myfile.get(c1);
                if(equal(c1))
                    return ":=";
                
                myfile.unget();
                return ":";
            }
            else if(And(c)){
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(And(c1))
                        return "&&";

                    myfile.unget();
                    return "&";
                }
            }
            else if(Or(c)){
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(Or(c1))
                        return "||";

                    myfile.unget();
                    return "|";
                }
            }
            else if(Not(c))          return "!";
            else if(equal(c))
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(equal(c1))
                        return "==";

                    myfile.unget();
                    return "=";
                }

            }   
            else if(c == '>')
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(equal(c1))   return ">=";

                    myfile.unget();
                    return ">";
                }
            }
            else if(c == '<')
            {
                if(!myfile.eof())
                {
                    char c1;
                    myfile.get(c1);
                    if(c1 == '=')   return "<=";

                    myfile.unget();
                    return "<";
                }

            }
        }
    }
    return token;
}

itemSet Goto(itemSet I, string term_nonTerm);
void closure(itemSet &I);
void lookAhead(itemSet &I);
void takeGrammers();
void addBasics(map<string , symbolTable> &symtable);
void parseAndGenerateIR(map<string , symbolTable> &symtable);

int main()
{
    takeGrammers();

    if(grammerError)
        return 0;

    line = 1;
    map<string , symbolTable> symtable;
    addBasics(symtable);
    parseAndGenerateIR(symtable);

    return 0;
}

void everyword(ifstream &gr, string &token, grammer &tempg)
{
    if((isWord(token) || isNumber(token) || signs(token[0]) ||
        token == "_" || token == "||") &&
        (token != "^;" && token != ":="))
    {
        terminal_nonTerminal word;
        word.isTerminal = signs(token[0]) || isNumber(token) ||
            isKey(token) || token == "iden";

        word.var = token;
        tempg.right.push_back(word);

        token = nextToken(gr);
        everyword(gr, token, tempg);
    }
    else if(token == ":=")
    {
        grammerError = true;
        cout<<line<<"wrong grammer\n";
    }
}

void anything1(ifstream &gr, string &token, grammer &tempg)
{
    if((isWord(token) || isNumber(token) || token == "_" || signs(token[0])) &&
        (token != "^;" && token != ":="))
    {
        everyword(gr, token, tempg);
    }
    else if(token == "~")
    {
        terminal_nonTerminal t;
        t.var = "~";
        t.isTerminal = true;
        tempg.right.push_back(t);
        
        token = nextToken(gr);
    }
    else {
        grammerError = true;
        cout<<line<<": wrong grammer\n";
    }
}

void anything(ifstream &gr, string &token, grammer &tempg)
{
    anything1(gr, token, tempg);
    grammersList.push_back(tempg);
    string l = tempg.left;
    grammer t;
    tempg = t;
    tempg.left = l;
    while(token == "|")
    {
        token = nextToken(gr);
        anything1(gr, token, tempg);
        grammersList.push_back(tempg);
        l = tempg.left;
        tempg = t;
        tempg.left = l;
    }
}

void startgrammer(ifstream &gr, string &token)
{
    if(isWord(token))
    {
        grammer tempg;
        tempg.left = token;
        token = nextToken(gr);
        if(token == ":=")
        {
            token = nextToken(gr);
            anything(gr, token, tempg);

            if(token == "^;")
            {
                token = nextToken(gr);
                startgrammer(gr,token);
            }
        }
        else
        {
            grammerError = true;
            cout<<line<<": wrong grammer\n";
        }
    }
    else if(token != "")
    {
        grammerError = true;
        cout<<line<<": wrong grammer\n";
    }
}

void takeGrammers()
{
    ifstream gr;
    gr.open(tslang_grammer, ios::in);
    string token = nextToken(gr);

    grammer g;
    g.left = "start";
    terminal_nonTerminal temp;
    temp.isTerminal = false;
    temp.var = token;
    g.right.push_back(temp);
    grammersList.push_back(g);

    startgrammer(gr, token);
}

itemSet Goto(itemSet I, string term_nonTerm)
{
    itemSet item;
    for(int i = 0 ; i < I.grammers.size() ; i++)
    {
        int dotPos = I.grammers[i].dot;
        if(dotPos<I.grammers[i].right.size())
        {
            if(I.grammers[i].right[dotPos].var 
                == term_nonTerm)
            {
                grammer newG;
                newG = I.grammers[i];
                newG.dot++;
                item.grammers.push_back(newG);
            }
        }
    }

    return item;
}

void closure(itemSet &I)
{
    for(int i = 0 ; i < I.grammers.size() ; i++)
    {
        int dotPos = I.grammers[i].dot;
        if(!I.grammers[i].right[dotPos].isTerminal) // where is dot? before a non_terminal?
            for(int j = 0 ; j < grammersList.size() ; j++)
                if(I.grammers[i].right[dotPos].var ==
                    grammersList[j].left)
                {
                    grammer newG = grammersList[j];

                    // bool checkEquality = false;
                    // for(int k = 0 ; k < I.grammers.size() ; i++)
                    //     if(newG.left == I.grammers[k].left)
                    //     {
                    //         if(newG.right.size() == I.grammers[k].right.size())
                    //             for(int l = 0 ; l < I.grammers[k].right.size() ; l++)
                    //                 if(I.grammers[k].right[l].var == newG.right[l].var)
                    //                     checkEquality = true;
                    //                 else
                    //                 {
                    //                     checkEquality = false;
                    //                     break;
                    //                 }

                    //         if (checkEquality)  break;
                    //     }

                    // if(!checkEquality)   
                    //     I.grammers.push_back(newG);
                }
    }
}

terminal_nonTerminal findLookAheadFornonTerminal(string possLoA)
{
    for(int i = 0 ; i < grammersList.size() ; i++)
        if(grammersList[i].left == possLoA)
        {
            if(grammersList[i].right[0].isTerminal)
                return grammersList[i].right[0];
            else
                return 
                    findLookAheadFornonTerminal(grammersList[i].right[0].var);
        }
}

void lookAhead(itemSet &I)
{
    for(int i = 0 ; i < I.grammers.size() ; i++)
        if(I.grammers[i].left != "start")
        {
            string left = I.grammers[i].left;
            for(int j = 0 ; j < I.grammers.size() ; j++)
                for(int k = 0 ; k < I.grammers[j].right.size() ; k++)
                    if(left == I.grammers[j].right[k].var)
                    {
                        int dotPos = I.grammers[j].dot;
                        if(dotPos + 1 < I.grammers[j].right.size())
                        {    
                            terminal_nonTerminal possibleLoA;
                            possibleLoA = I.grammers[j].right[dotPos + 1];
                            if(!possibleLoA.isTerminal)
                                possibleLoA = 
                                    findLookAheadFornonTerminal(possibleLoA.var);

                            I.grammers[i].lookahead.push_back(possibleLoA);            
                        }
                    }
        }
}

void addBasics(map<string , symbolTable> &symtable)
{
    //////////adding existing functions//////
    symbolTable numRead; //index : 0
    numRead.variable = "numread";
    numRead.type = T_num;
    numRead.isFunc = true;
    numRead.numberOfParams = 0;

    symbolTable numPrint; //index : 1
    numPrint.variable = "numprint";
    numPrint.type = 0;
    numPrint.isFunc = true;
    numPrint.numberOfParams = 1;
    
    symbolTable input;
    input.variable = "input";
    input.type = T_num;
    input.isFunc = false;
    input.numberOfParams = 0;

    numPrint.list[input.variable] = input;

    symbolTable Vec; //index : 2
    Vec.variable = "vec";
    Vec.type = T_list;
    Vec.isFunc = true; 
    Vec.numberOfParams = 1;

    symbolTable number;
    number.variable = "number";
    number.type = T_num;
    number.isFunc = false;
    number.numberOfParams = 0;
    
    Vec.list[number.variable] = number;

    symbolTable Veclen; //index : 3
    Veclen.variable = "veclen";
    Veclen.type = T_num;
    Veclen.isFunc = true;
    Veclen.numberOfParams = 1;

    symbolTable vect;
    vect.variable = "vector";
    vect.type = T_list;
    vect.isFunc = false;
    vect.numberOfParams = 0;

    Veclen.list[vect.variable] = vect;

    symbolTable Exit; //index : 4
    Exit.variable = "exit";
    Exit.type = 0;
    Exit.isFunc = true;
    Exit.numberOfParams = 1;
    Exit.list[Exit.variable] = input;

    symtable[numRead.variable] = numRead;
    symtable[numPrint.variable] = numPrint;
    symtable[Vec.variable] = Vec;
    symtable[Veclen.variable] = Veclen;
    symtable[Exit.variable] = Exit;
}

void parseAndGenerateIR(map<string , symbolTable> &symtable)
{
    vector<vector<int>> adj; // dfa

    map<int , itemSet> item;

    itemSet I;
    I.grammers.push_back(grammersList[0]);
    closure(I);
    lookAhead(I);
    lookAhead(I);

    item[0] = I;

    map<string , string> dots;
    for(int i = 0 ; i < I.grammers.size() ; i++)
    {
        int dotPos = I.grammers[i].dot;
        dots[I.grammers[i].right[dotPos].var] = I.grammers[i].right[dotPos].var;
    }

    int no_of_nodes = 1;
    for (auto x : dots)
    {
        itemSet I2;
        I2 = Goto(I, x.second);
        closure(I2);
        lookAhead(I2);
        lookAhead(I2);

        bool checkEquality = false;
        int destination = no_of_nodes;

        string m;
        for(int i = 0 ; i <  I2.grammers.size() ; i++)
        {
            m.append(I2.grammers[i].left);

            for(int j = 0 ; j < I2.grammers[i].right.size() ; j++)
                m.append(I2.grammers[i].right[j].var);

            for(int j = 0 ; j < I2.grammers[i].lookahead.size() ; j++)
                m.append(I2.grammers[i].lookahead[j].var);

            m.append(" " + I2.grammers[i].dot);
        }

        for(auto y : item)
        {
            string n;
            for(int i = 0 ; i < y.second.grammers.size() ; i++)
            {
                n.append(y.second.grammers[i].left);

                for(int j = 0 ; j < y.second.grammers[i].right.size() ; j++)
                    n.append(y.second.grammers[i].right[j].var);

                for(int j = 0 ; j < y.second.grammers[i].lookahead.size() ; j++)
                    n.append(y.second.grammers[i].lookahead[j].var);

                n.append(" " + y.second.grammers[i].dot);
            }

            if( m == n)
            {
                checkEquality = true;
                destination = y.first;
                break;
            }
        }
        
        if(!checkEquality)
            item[no_of_nodes] = I2;
        
        edge e;
        e.term_nonterm = x.second;
        e.src = 0;
        e.dest = destination;

        adj[e.src].push_back(e.dest);

        no_of_nodes++;
    }

    map<int , itemSet>::iterator itr;
    itr = item.begin();
    itr++;
    while(itr != item.begin())
    {
        dots.clear();
        for(int i = 0 ; i < itr->second.grammers.size() ; i++)
        {
            int dotPos = itr->second.grammers[i].dot;
            dots[itr->second.grammers[i].right[dotPos].var] = 
                itr->second.grammers[i].right[dotPos].var;
        }

        for(auto x : dots)
        {
            itemSet I2;
            I2 = Goto(itr->second, x.second);
            closure(I2);
            lookAhead(I2);
            lookAhead(I2);

            bool checkEquality = false;
            int destination = no_of_nodes;

            string m;
            for(int i = 0 ; i <  I2.grammers.size() ; i++)
            {
                m.append(I2.grammers[i].left);

                for(int j = 0 ; j < I2.grammers[i].right.size() ; j++)
                    m.append(I2.grammers[i].right[j].var);

                for(int j = 0 ; j < I2.grammers[i].lookahead.size() ; j++)
                    m.append(I2.grammers[i].lookahead[j].var);

                m.append(" " + I2.grammers[i].dot);
            }

            for(auto y : item)
            {
                string n;
                for(int i = 0 ; i < y.second.grammers.size() ; i++)
                {
                    n.append(y.second.grammers[i].left);

                    for(int j = 0 ; j < y.second.grammers[i].right.size() ; j++)
                        n.append(y.second.grammers[i].right[j].var);

                    for(int j = 0 ; j < y.second.grammers[i].lookahead.size() ; j++)
                        n.append(y.second.grammers[i].lookahead[j].var);

                    n.append(" " + y.second.grammers[i].dot);
                }

                if( m == n)
                {
                    checkEquality = true;
                    destination = y.first;
                    break;
                }
            }

            if(!checkEquality)
                item[no_of_nodes] = I2;

            edge e;
            e.term_nonterm = x.second;
            e.src = 0;
            e.dest = destination;

            adj[e.src].push_back(e.dest);

            no_of_nodes++;
        }
    }
}