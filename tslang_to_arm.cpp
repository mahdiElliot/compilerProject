#include<iostream>
#include<string>
#include<fstream>
#include <iostream>

using namespace std;

ofstream output;

bool mov(string word)   {   return word == "mov";   }

bool add(string word)   {   return word == "add";   }

bool sub(string word)   {   return word == "sub";   }

bool mul(string word)   {   return word == "mul";   }

bool div(string word)   {   return word == "div";   }

bool mod(string word)   {   return word == "mod";   }

bool cmp(string word)   {   return word == "cmp";   }

bool jmp(string word)   {   return word == "jmp";   }

bool jz(string word)    {   return word == "jz";    }

bool jnz(string word)   {   return word == "jnz";   }

bool call(string word)  {   return word == "call";  }

bool ret(string word)   {   return word == "ret";   }

bool ld(string word)    {   return word == "ld";    }

bool st(string word)    {   return word == "st";    }

bool nop(string word)   {   return word == "nop";   }

bool proc(string word)  {   return word == "proc";  }

bool iget(string word)  {   return word == "iget";  }

bool iput(string word)  {   return word == "iput";  }

bool greaterThan(string gt)
{
    return gt == ">";
}

bool lessThan(string lt)
{
    return lt == "<";
}

bool equalGt(string egt)
{
    return egt == ">=";
}

bool equalLt(string elt)
{
    return elt == "<=";
}

bool equal(string eq)
{
    return eq == "=";
}

bool isDigit(char digit)
{
    return digit == '0' || digit == '1' || digit == '2' || digit == '3' || digit == '4' || 
    digit == '5' || digit == '6' || digit == '7' || digit == '8' || digit == '9';
}

bool Main(string word)   {   return word == "main";  }

bool regs(string word)
{
    if(word.size()>3 || word.size() == 1)   return false;

    if(word[0] == 'r')
    {
        string n;
        int x;
        if(word.size() == 3)
        {
            if(!isDigit(word[1]) || !isDigit(word[2]))  return false;

            n = word[1];
            n = n + word[2];
            x = stoi(n);
        }
        else
        {
            if(!isDigit(word[1])) return false;

            n=word[1];
            x = stoi(n);
        }

        return x>=0 && x<=31;
    }

    return false;
}

bool otherRegs(string word)
{
    if(word.size() == 1)    return false;

    if(word[0] == 'r')
    {
        string n="";
        for(int i = 1 ; i<word.size(); i++)
            if(!isDigit(word[i]))   return false;
            else n = n + word[i];

        int x = stoi(n);

        return x>31;
    }

    return false;
}

bool letter(string word)
{
    bool valid = false;

    if(!mov(word) && !add(word) && !sub(word) && !mul(word) && !div(word) && !mod(word) && !cmp(word) &&
        !jmp(word) && !jz(word) && !jnz(word) && !call(word) && !ret(word) && !ld(word) && !st(word) &&
        !nop(word) && !proc(word) && !Main(word) && !regs(word) && !otherRegs(word) &&
         !iput(word) && !iget(word))
            if((int(word[0])>=65 && int(word[0])<=90) || (int(word[0])>=97 && int(word[0]<=122))
            || word[0] == '_')
            {
                if(word.size() == 1) valid=true;
                for(int i=1 ; i<word.size() ; i++)
                    if(isDigit(word[i]) || (int(word[i])>=65 && int(word[i])<=90) ||
                     (int(word[i])>=97 && int(word[i]<=122)) || word[i] == '_' )
                        valid=true;
                    else
                    return false;
            }

    return valid;

}

bool comma(char c)
{
    return c == ',';
}
 bool doubleDots(char d)
 {
     return d == ':';
 }

string nextToken(ifstream &myfile)
{
    char c;
    string token = "";

    if(!myfile.eof())
    {
        myfile.get(c);
        while((c == ' ' || c == '\n') && !myfile.eof())    myfile.get(c);

        if(!myfile.eof())
        if((c>='a' && c<= 'z') || (c>='A' && c<='Z') || (c>='0' && c<='9'))
        {
            token ="";
            
            while(((c>= 'a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')) && !myfile.eof())
            {
                token = token + c;
                myfile.get(c);
            }
            
            if(!myfile.eof())   myfile.unget();
        }
        else if(comma(c))   return ",";
        else if(doubleDots(c))   return ":";
        else if(c == '=')   return "=";  
        else if(c == '>')
        {
            if(!myfile.eof())
            {
                char c1;
                myfile.get(c1);
                if(c1 == '=')   return ">=";
            }
            myfile.unget();
            return ">";
        }
        else if(c == '<')
        {
            if(!myfile.eof())
            {
                char c1;
                myfile.get(c1);
                if(c1 == '=')   return "<=";
            }
            myfile.unget();
            return "<";
        }
    }

    return token;
}

void start(ifstream &myfile, int maxReg);
void insideMain(ifstream &myfile, ofstream &output, string token, int maxReg, string compare);
void insideFunc(ifstream &myfile, ofstream &output, string token, int maxReg, string compare);
void operations(ifstream &myfile, ofstream &output, int maxReg, string op);

int main()
{ 
    ifstream myfile;
    myfile.open("file.txt", ios::in);
    int position = myfile.beg;
    string words;
    words = nextToken(myfile);

    while(words!=""){
        int maxReg = 0;
        while (words!="ret")
        {
            if(regs(words) || otherRegs(words))
            {
                string number = "";
                for(int i = 1 ; i<words.size() ; i++)
                    number = number + words[i];

                if(stoi(number)>maxReg)   maxReg = stoi(number);
            }

            words = nextToken(myfile);
        }

        myfile.seekg(position,myfile.beg);
        start(myfile, maxReg);
        position = myfile.tellg();
        position--;
        words = nextToken(myfile);
        
        }
}

void start(ifstream &myfile, int maxReg)
{
    if(!output.is_open())
        output.open("out.txt");
    string token = nextToken(myfile);
    if(proc(token))
    {
        token = nextToken(myfile);
        if(Main(token))
        {
            token = nextToken(myfile);
            if(doubleDots(token[0]))
            {
                output<<"public main\nmain proc:\n";

                output<<"subs sp, sp, "<<(maxReg+1)*8<<";"<<endl;

                token = nextToken(myfile);
                insideMain(myfile, output,token, maxReg,"");
            }
        }
        else if(letter(token))
        {
            string funcName = token;
            token = nextToken(myfile);
            if(doubleDots(token[0]))
            {
                output<<funcName<< ":\n";
                output<<"mov r30, sp;"<<endl;
                output<<"subs sp, sp, "<<(maxReg+1)*8<<";"<<endl;
                output<<"mov r31, sp, fp;"<<endl;
                output<<"add sp, sp, r30;"<<endl;
            
                token = nextToken(myfile);
                insideFunc(myfile, output,token, maxReg,"");
            }
        }
    }
}

void operations(ifstream &myfile, ofstream &output, int maxReg, string op)
{
    string token = nextToken(myfile);
    string number = "";
    for(int i = 1 ; i<token.size() ; i++)
        number = number + token[i];
    
    output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
    token = nextToken(myfile);//comma
    token = nextToken(myfile);
    number = "";
    for(int i = 1 ; i<token.size() ; i++)
        number = number + token[i];
    
    output<<"add r2, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
    output<<"ldr r3, r2;"<<endl;
    token = nextToken(myfile); //comma
    token = nextToken(myfile);
    number = "";
    for(int i = 1 ; i<token.size() ; i++)
        number = number + token[i];

    output<<"add r4, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
    output<<"ldr r5, r4;"<<endl;
    output<<op<<" r5, r3, r5;"<<endl;
    output<<"str r5, r1;"<<endl;
    
}

void insideMain(ifstream &myfile, ofstream &output, string token, int maxReg, string compare)
{   
    if(mov(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        if(isDigit(token[0]))
        {
            output<<"str #"<<token<<", r1;"<<endl;
        }
        else {
            number = "";
            for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
            output<<"add r2, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"ldr r3, r2;"<<endl;
            output<<"str r3, r1;"<<endl;
        }

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg,compare);
    }
    else if(add(token))
    {
        operations(myfile, output, maxReg, "add");
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg,compare);
    }
    else if(sub(token))
    {
        operations(myfile, output, maxReg, "subs");
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg,compare);
    }
    else if(mul(token))
    {
        operations(myfile, output, maxReg, "mul");
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg,compare);
    }
    else if(div(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r4, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r5, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r1, r5;"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r6, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r6;"<<endl;
        output<<"cmp r2, #0;"<<endl;
        output<<"beq divide_end;"<<endl;
        output<<"mov r7, #0;"<<endl;
        output<<"mov r3, #1;"<<endl;
        output<<".startDiv"<<endl;
        output<<"cmp r2, r1;"<<endl;
        output<<"movls r2, r2, lsl#1;"<<endl;
        output<<"movls r3, r3, lsl#1;"<<endl;
        output<<"blt startDiv;"<<endl;
        output<<".nextDiv"<<endl;
        output<<"cmp r1, r2;"<<endl;
        output<<"subcs r1, r1, r2;"<<endl;
        output<<"addcs r7, r7, r3;"<<endl;
        output<<"movs r3, r3, lsr#1;"<<endl;
        output<<"movs r2, r2, lsr#1;"<<endl;
        output<<"movcc r2, r2, lsr#1;"<<endl;
        output<<"bcc nextDiv;"<<endl;
        output<<".divide_end"<<endl;
        output<<"str r7, r4;"<<endl;

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(mod(token))
    { 
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r4, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r5, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r1, r5;"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r6, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r6;"<<endl;
        output<<"cmp r2, #0;"<<endl;
        output<<"beq divide_end;"<<endl;
        output<<"mov r7, #0;"<<endl;
        output<<"mov r3, #1;"<<endl;
        output<<".startDiv"<<endl;
        output<<"cmp r2, r1;"<<endl;
        output<<"movls r2, r2, lsl#1;"<<endl;
        output<<"movls r3, r3, lsl#1;"<<endl;
        output<<"blt startDiv;"<<endl;
        output<<".nextDiv"<<endl;
        output<<"cmp r1, r2;"<<endl;
        output<<"subcs r1, r1, r2;"<<endl;
        output<<"addcs r7, r7, r3;"<<endl;
        output<<"movs r3, r3, lsr#1;"<<endl;
        output<<"movs r2, r2, lsr#1;"<<endl;
        output<<"movcc r2, r2, lsr#1;"<<endl;
        output<<"bcc nextDiv;"<<endl;
        output<<".divide_end"<<endl;
        output<<"str r1, r4;"<<endl;

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(cmp(token))
    {
        compare = nextToken(myfile);
        
        token = nextToken(myfile);
        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
    
        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r1;"<<endl;

        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
    
        output<<"add r3, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r4, r3;"<<endl;
        output<<"cmp r2,r4;"<<endl;

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(letter(token))
    {
        output<<"."+token<<endl;
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg,compare);
    }
    else if(jmp(token))
    {
        token = nextToken(myfile);
        output<<"b "<<token+";"<<endl;

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(jz(token))
    {
        token = nextToken(myfile);
        token = nextToken(myfile); // comma
        token = nextToken(myfile); //label
        if(equal(compare))
            output<<"beq "<<token+";"<<endl;
        else if(equalGt(compare))
            output<<"bgt "<<token+";"<<endl;
        else if(equalLt(compare))
            output<<"blt "<<token+";"<<endl;

        compare="";
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(jnz(token))
    {
        token = nextToken(myfile);
        token = nextToken(myfile); // comma
        token = nextToken(myfile); //label
        if(equal(compare))
            output<<"bnq "<<token+";"<<endl;
        else if(equalGt(compare))
            output<<"blt "<<token+";"<<endl;
        else if(equalLt(compare))
            output<<"bgt "<<token+";"<<endl;

        compare="";
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(nop(token))
    {
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(ld(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r2, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r3, r2;"<<endl;
        output<<"str r3, r1;"<<endl;

        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(st(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r1;"<<endl;

        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r3, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"str r2, r3;"<<endl;
        
        token = nextToken(myfile);
        insideMain(myfile, output,token, maxReg, compare);
    }
    else if(call(token))
    {
        token = nextToken(myfile);
        if(letter(token))
        {
            string label = token;
            output<<"subs fp, sp, #8;"<<endl;

            string number;
            while(!add(token) && !sub(token) && !ret(token) && !mov(token) && !mul(token) &&
            !div(token) && !mod(token) && !cmp(token) && !jnz(token) && !jz(token) && !jmp(token) &&
            !ld(token) && !st(token) && !nop(token))
            {
                token = nextToken(myfile);
                number = "";
                for(int i = 1 ; i<token.size() ; i++)
                    number = number + token[i];

                output<<"add r1, fp, "<<(maxReg+2 - stoi(number))*8<<";"<<endl;
                output<<"ldr r2, r1;"<<endl;
                output<<"str r2, sp;"<<endl;
                output<<"subs sp, sp, 8;"<<endl;
                token = nextToken(myfile); //comma
            }

            output<<"B "<<label<<";"<<endl;

        }
        else if(iput(token))
        {
            token = nextToken(myfile);//comma
            token = nextToken(myfile);

            string number = "";
            for(int i = 1 ; i<token.size() ; i++)
                number = number + token[i];

            output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"ldr r0, r1;"<<endl;
            output<<"mov r0, #STDOUT;"<<endl;

            token = nextToken(myfile);
        }
        else if(iget(token))
        {
            token = nextToken(myfile);//comma
            token = nextToken(myfile);
            string number = "";
            for(int i = 1 ; i<token.size() ; i++)
                number = number + token[i];

            output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"mov r0, #STDIN;"<<endl;
            output<<"str r0, r1;"<<endl;

            token = nextToken(myfile);
        }

        insideMain(myfile, output,token, maxReg, compare);

    }
    else if(ret(token))
    {
        output<<"add sp, sp, "<<(maxReg+1)*8<<";"<<endl;
        output<<".end\n"<<endl;
    }
}

void insideFunc(ifstream &myfile, ofstream &output, string token, int maxReg, string compare)
{
    if(mov(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        if(isDigit(token[0]))
        {
            output<<"str #"<<token<<", r1;"<<endl;
        }
        else {
            number = "";
            for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
            output<<"add r2, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"ldr r3, r2;"<<endl;
            output<<"str r3, r1;"<<endl;
        }

        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg,compare);
    }
    else if(add(token))
    {
        operations(myfile, output, maxReg, "add");
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg,compare);
    }
    else if(sub(token))
    {
        operations(myfile, output, maxReg, "subs");
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg,compare);
    }
    else if(mul(token))
    {
        operations(myfile, output, maxReg, "mul");
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg,compare);
    }
    else if(div(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r4, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r5, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r1, r5;"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r6, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r6;"<<endl;
        output<<"cmp r2, #0;"<<endl;
        output<<"beq divide_end;"<<endl;
        output<<"mov r7, #0;"<<endl;
        output<<"mov r3, #1;"<<endl;
        output<<".startDiv"<<endl;
        output<<"cmp r2, r1;"<<endl;
        output<<"movls r2, r2, lsl#1;"<<endl;
        output<<"movls r3, r3, lsl#1;"<<endl;
        output<<"blt startDiv;"<<endl;
        output<<".nextDiv"<<endl;
        output<<"cmp r1, r2;"<<endl;
        output<<"subcs r1, r1, r2;"<<endl;
        output<<"addcs r7, r7, r3;"<<endl;
        output<<"movs r3, r3, lsr#1;"<<endl;
        output<<"movs r2, r2, lsr#1;"<<endl;
        output<<"movcc r2, r2, lsr#1;"<<endl;
        output<<"bcc nextDiv;"<<endl;
        output<<".divide_end"<<endl;
        output<<"str r7, r4;"<<endl;

        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(mod(token))
    { 
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r4, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r5, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r1, r5;"<<endl;

        token= nextToken(myfile); //comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r6, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r6;"<<endl;
        output<<"cmp r2, #0;"<<endl;
        output<<"beq divide_end;"<<endl;
        output<<"mov r7, #0;"<<endl;
        output<<"mov r3, #1;"<<endl;
        output<<".startDiv"<<endl;
        output<<"cmp r2, r1;"<<endl;
        output<<"movls r2, r2, lsl#1;"<<endl;
        output<<"movls r3, r3, lsl#1;"<<endl;
        output<<"blt startDiv;"<<endl;
        output<<".nextDiv"<<endl;
        output<<"cmp r1, r2;"<<endl;
        output<<"subcs r1, r1, r2;"<<endl;
        output<<"addcs r7, r7, r3;"<<endl;
        output<<"movs r3, r3, lsr#1;"<<endl;
        output<<"movs r2, r2, lsr#1;"<<endl;
        output<<"movcc r2, r2, lsr#1;"<<endl;
        output<<"bcc nextDiv;"<<endl;
        output<<".divide_end"<<endl;
        output<<"str r1, r4;"<<endl;

        token = nextToken(myfile);
        insideFunc(myfile, output, token, maxReg, compare);
    }
    else if(cmp(token))
    {
        compare = nextToken(myfile);
        
        token = nextToken(myfile);
        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
    
        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r1;"<<endl;

        token = nextToken(myfile);//comma
        token = nextToken(myfile);
        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];
    
        output<<"add r3, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r4, r3;"<<endl;
        output<<"cmp r2,r4;"<<endl;

        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(letter(token))
    {
        output<<"."+token<<endl;
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg,compare);
    }
    else if(jmp(token))
    {
        token = nextToken(myfile);
        output<<"b "<<token+";"<<endl;

        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(jz(token))
    {
        token = nextToken(myfile);
        token = nextToken(myfile); // comma
        token = nextToken(myfile); //label
        if(equal(compare))
            output<<"beq "<<token+";"<<endl;
        else if(equalGt(compare))
            output<<"bgt "<<token+";"<<endl;
        else if(equalLt(compare))
            output<<"blt "<<token+";"<<endl;

        compare="";
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(jnz(token))
    {
        token = nextToken(myfile);
        token = nextToken(myfile); // comma
        token = nextToken(myfile); //label
        if(equal(compare))
            output<<"bnq "<<token+";"<<endl;
        else if(equalGt(compare))
            output<<"blt "<<token+";"<<endl;
        else if(equalLt(compare))
            output<<"bgt "<<token+";"<<endl;

        compare="";
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(nop(token))
    {
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(ld(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;

        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r2, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r3, r2;"<<endl;
        output<<"str r3, r1;"<<endl;

        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(st(token))
    {
        token = nextToken(myfile);
        string number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"ldr r2, r1;"<<endl;

        number = "";
        for(int i = 1 ; i<token.size() ; i++)
            number = number + token[i];

        output<<"add r3, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
        output<<"str r2, r3;"<<endl;
        
        token = nextToken(myfile);
        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(call(token))
    {
        token = nextToken(myfile);
        if(letter(token))
        {
            string label = token;
            output<<"subs fp, sp, #8;"<<endl;

            string number;
            while(!add(token) && !sub(token) && !ret(token) && !mov(token) && !mul(token) &&
            !div(token) && !mod(token) && !cmp(token) && !jnz(token) && !jz(token) && !jmp(token) &&
            !ld(token) && !st(token) && !nop(token))
            {
                token = nextToken(myfile);
                number = "";
                for(int i = 1 ; i<token.size() ; i++)
                    number = number + token[i];

                output<<"add r1, fp, "<<(maxReg+2 - stoi(number))*8<<";"<<endl;
                output<<"ldr r2, r1;"<<endl;
                output<<"str r2, sp;"<<endl;
                output<<"subs sp, sp, 8;"<<endl;
                token = nextToken(myfile); //comma
            }

            output<<"B "<<label<<";"<<endl;

        }
        else if(iput(token))
        {
            token = nextToken(myfile);//comma
            token = nextToken(myfile);

            string number = "";
            for(int i = 1 ; i<token.size() ; i++)
                number = number + token[i];

            output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"ldr r0, r1;"<<endl;
            output<<"mov r0, #STDOUT;"<<endl;

        token = nextToken(myfile);
        }
        else if(iget(token))
        {
            token = nextToken(myfile);//comma
            token = nextToken(myfile);
            output<<token;
            string number = "";
            for(int i = 1 ; i<token.size() ; i++)
                number = number + token[i];

            output<<"add r1, sp, "<<(maxReg+1 - stoi(number))*8<<";"<<endl;
            output<<"mov r0, #STDIN;"<<endl;
            output<<"str r0, r1;"<<endl;

        token = nextToken(myfile);
        }

        insideFunc(myfile, output,token, maxReg, compare);
    }
    else if(ret(token))
    {
        output<<"mov sp, fp;"<<endl;
        output<<"mov pc, lr*;"<<endl;
        output<<".end\n"<<endl;
    }
}