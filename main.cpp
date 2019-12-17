#include <string>
#include <string.h>
#include <list>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include "ex1.h"
#include "Expression.h"
#include <unistd.h>
#include <netinet/in.h>
#include <vector>

using namespace std;
struct Var{
    float value;
    string sim;
};
class Command {
public:
    virtual int execute(list<string>* code){
        cout << "base execute" << endl;
    };
    ~Command(){

    }
};
constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}
void printList(list<string> list) {
    for (auto v : list)
        std::cout << v << "\n";
}
size_t split(const string &txt, vector<string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

list<string> lexer(string name){
    list<string> textList;
    fstream fp;
    fp.open(name);
    if(!fp){
        cout << "Unable to open file";
    }
    char ch;
    int i=0;
    string line;

    for(line; getline( fp, line ); ) // reading line by line the text
    {
        //string testline= "while rpm <= 750 {";
        string word;
        int flag=0;
        int flag2=0;
        int flag3=0;
        int flag4=0;
        //cout<<line<<endl;
        for (auto x : line)
        {
            if (x == ' ' && flag==0 ) /// means we have a space
            {
                //cout << word << endl;
                textList.push_back(word);
                word = "";
            }
            if( x== '\t'){
                cout<<"let me know you are here"<<endl;
                continue;
            }
            if(x=='('&&flag2==0){
                //cout << word << endl;
                textList.push_back(word);
                word = "";
                flag=1;
                continue;
            }
            if( x==')'&&flag2==0){
                textList.push_back(word);
                word = "";
                flag=0;
                continue;
            }
            if(x=='='){
                //cout << word << endl;
                if(flag3==1){
                    word= "<=";
                    flag3=0;
                }
                else if(flag4==1){
                    word= ">=";
                    flag4=0;
                }
                else {
                    word = "=";
                }
                textList.push_back(word);
                word ="";
                flag2=1;
                flag=1;
                continue;
            }
            if(x=='<'){
                flag3=1;
                word = word + x;
                continue;
            }
            if(x=='>'){
                flag4=1;
                word = word + x;
                continue;
            }
            else
            {
                word = word + x;
            }
        }
        if(word.compare("")!=0){
            textList.push_back(word);
            flag2=0;
            flag=0;
        }

    }
     //printList(textList);
    return textList;
}

unordered_map<string,Command*> commandsMap;
unordered_map<string,Var> localVariables;
unordered_map<string,Var> simVariables;

int serverThread(int port){
//create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        return -1;
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(port);
    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        return -2;
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        return -3;
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,(socklen_t *) &address);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        return -4;
    }
    //reading from client

    struct Var var0 {0,""};
    simVariables.emplace("/instrumentation/airspeed-indicator/indicated-speed-kt",var0);
    struct Var var1 {0,""};
    simVariables.emplace("/instrumentation/altimeter/indicated-altitude-ft",var1);
    struct Var var2 {0,""};
    simVariables.emplace("/instrumentation/altimeter/pressure-alt-ft",var2);
    struct Var var3 {0,""};
    simVariables.emplace("/instrumentation/attitude-indicator/indicated-pitch-deg",var3);
    struct Var var4 {0,""};
    simVariables.emplace("/instrumentation/attitude-indicator/indicated-roll-deg",var4);
    struct Var var5 {0,""};
    simVariables.emplace("/instrumentation/attitude-indicator/internal-pitch-deg",var5);
    struct Var var6 {0,""};
    simVariables.emplace("/instrumentation/attitude-indicator/internal-roll-deg",var6);
    struct Var var7 {0,""};
    simVariables.emplace("/instrumentation/encoder/indicated-altitude-ft",var7);
    struct Var var8 {0,""};
    simVariables.emplace("/instrumentation/encoder/pressure-alt-ft",var8);
    struct Var var9 {0,""};
    simVariables.emplace("/instrumentation/gps/indicated-altitude-ft",var9);
    struct Var var10 {0,""};
    simVariables.emplace("/instrumentation/gps/indicated-ground-speed-kt",var10);
    struct Var var11 {0,""};
    simVariables.emplace("/instrumentation/gps/indicated-vertical-speed",var11);
    struct Var var12 {0,""};
    simVariables.emplace("/instrumentation/heading-indicator/indicated-heading-deg",var12);
    struct Var var13 {0,""};
    simVariables.emplace("/instrumentation/magnetic-compass/indicated-heading-deg",var13);
    struct Var var14 {0,""};
    simVariables.emplace("/instrumentation/slip-skid-ball/indicated-slip-skid",var14);
    struct Var var15 {0,""};
    simVariables.emplace("/instrumentation/turn-indicator/indicated-turn-rate",var15);
    struct Var var16 {0,""};
    simVariables.emplace("/instrumentation/vertical-speed-indicator/indicated-speed-fpm",var16);
    struct Var var17 {0,""};
    simVariables.emplace("/controls/flight/aileron",var17);
    struct Var var18 {0,""};
    simVariables.emplace("/controls/flight/elevator",var18);
    struct Var var19 {0,""};
    simVariables.emplace("/controls/flight/rudder",var19);
    struct Var var20 {0,""};
    simVariables.emplace("/controls/flight/flaps",var20);
    struct Var var21 {0,""};
    simVariables.emplace("/controls/engines/engine/throttle",var21);
    struct Var var22 {0,""};
    simVariables.emplace("/engines/engine/rpm",var22);

    while (true) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);

        vector<string> v;
        split( buffer, v, ',');
        string values[23];
        int counter=0;
        for (auto x : v){
            values[counter]=x;
            counter++;
        }


        simVariables["/instrumentation/airspeed-indicator/indicated-speed-kt"].value=stof(values[0]);
        simVariables["/instrumentation/altimeter/indicated-altitude-ft"].value=stof(values[1]);
        simVariables["/instrumentation/altimeter/pressure-alt-ft"].value=stof(values[2]);
        simVariables["/instrumentation/attitude-indicator/indicated-pitch-deg"].value=stof(values[3]);
        simVariables["/instrumentation/attitude-indicator/indicated-roll-deg"].value=stof(values[4]);
        simVariables["/instrumentation/attitude-indicator/internal-pitch-deg"].value=stof(values[5]);
        simVariables["/instrumentation/attitude-indicator/internal-roll-deg"].value=stof(values[6]);
        simVariables["/instrumentation/encoder/indicated-altitude-ft"].value=stof(values[7]);
        simVariables["/instrumentation/encoder/pressure-alt-ft"].value=stof(values[8]);
        simVariables["/instrumentation/gps/indicated-altitude-ft"].value=stof(values[9]);
        simVariables["/instrumentation/gps/indicated-ground-speed-kt"].value=stof(values[10]);
        simVariables["/instrumentation/gps/indicated-vertical-speed"].value=stof(values[11]);
        simVariables["/instrumentation/heading-indicator/indicated-heading-deg"].value=stof(values[12]);
        simVariables["/instrumentation/magnetic-compass/indicated-heading-deg"].value=stof(values[13]);
        simVariables["/instrumentation/slip-skid-ball/indicated-slip-skid"].value=stof(values[14]);
        simVariables["/instrumentation/turn-indicator/indicated-turn-rate"].value=stof(values[15]);
        simVariables["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"].value=stof(values[16]);
        simVariables["/controls/flight/aileron"].value=stof(values[17]);
        simVariables["/controls/flight/elevator"].value=stof(values[18]);
        simVariables["/controls/flight/rudder"].value=stof(values[19]);
        simVariables["/controls/flight/flaps"].value=stof(values[20]);
        simVariables["/controls/engines/engine/throttle"].value=stof(values[21]);
        simVariables["/engines/engine/rpm"].value=stof(values[22]);

        for (auto x : simVariables){
            cout << x.second.value ;
            cout << ",";
        }
        cout << endl;
    }
}

class OpenServerCommand : public Command { //fix
public:
    int execute(list<string>* code) {
        code->pop_front();
        thread tr(serverThread,stoi(code->front()));
        code->pop_front();
        tr.detach();
    }
};

class ConnectCommand : public Command { //fix
public:
    int execute(list<string>* code){
        code->pop_front();
        code->pop_front();
        cout << "connect" << endl;
    }
};

class VarCommand : public Command {
public:
    int execute(list<string>* code){
        code->pop_front();
        string varName = code->front();
        code->pop_front();
        string op = code->front();
        code->pop_front();
        code->pop_front();
        string sim = code->front();
        code->pop_front();
        struct Var var;
        if(op.compare("<=")){
            var = simVariables[sim];
        }
        else {
            struct Var var{0, sim};
        }
        localVariables.emplace(varName,var);
    }
};

class SleepCommand : public Command {
public:
    int execute(list<string>* code){
        code->pop_front();
        int timeToSleep = stoi(code->front());
        code->pop_front();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
    }
};

class PrintCommand : public Command {
public:
    int execute(list<string>* code){
        code->pop_front();
        string print = code->front();
        code->pop_front();
        cout << print << endl;
    }
};

class WhileCommand : public Command { //fix
public:
    int execute(list<string>* code){
        cout<< "im in the while command"<<endl;
       /* list<string> whileList;
        /// building the whileList
        string front= code.front();
        while(front == "}"){
            whileList.push_front(code.front());
            code.pop_front();
            front= code.front();
        }

        code.pop_front(); ///poping the "while"
        string temp = code.front() ;
        code.pop_front(); ///poping the "var" =rpm
        string temp1= code.front();


        code.pop_front(); ///poping the "sign"
        string temp2=code.front();
        int temp2Length= temp2.size();
        temp2= temp2.substr(0, temp2.size()-1);

        code.pop_front(); ///poping the "right side of the equation"
        string equatoin = temp +temp1 +temp2;
        cout<<equatoin<<endl; /// printing the equation

        /// now we have commands we need to do inside the while, we put them in a diffrent list

        Variable* leftSideOfTheEquation = new Variable(temp, 3.0);// x2=5.0 ///localVaribals[rpm]
        Variable* rightSideOfTheEquation= new Variable(temp2,stoi(temp2));

        const char * c = temp1.c_str();
        int valueOfSigh= str2int((c));*/

       /* switch(valueOfSigh) {
            case str2int("<=") :
            cout<<"<="<<endl;
            while(leftSideOfTheEquation->calculate()<= rightSideOfTheEquation->calculate()){
                Command* c = commandsMap[code.front()];
                whileFlag=1;
                c->execute(whileList);
                /// x2=5.0 ///localVaribals[rpm]
            }
            whileFlag=0;
                break;
            case str2int(">=") :
                while(leftSideOfTheEquation->calculate()>= rightSideOfTheEquation->calculate()){
                    Command* c = commandsMap[code.front()];
                    whileFlag=1;
                    c->execute(whileList);

                }
                whileFlag=0;
                break;
            case str2int("<") :
                while(leftSideOfTheEquation->calculate()< rightSideOfTheEquation->calculate()){
                    Command* c = commandsMap[code.front()];
                    whileFlag=1;
                    c->execute(whileList);
                }
                whileFlag=0;
                break;
            case str2int(">") :
                while(leftSideOfTheEquation->calculate()> rightSideOfTheEquation->calculate()){
                    Command* c = commandsMap[code.front()];
                    whileFlag=1;
                    c->execute(whileList);
                }
                whileFlag=0;
                break;
            case str2int("==") :
                while(leftSideOfTheEquation->calculate()== rightSideOfTheEquation->calculate()){
                    Command* c = commandsMap[code.front()];
                    whileFlag=1;
                    c->execute(whileList);
                }
                whileFlag=0;
                break;
            case str2int("!=") :
                while(leftSideOfTheEquation->calculate()!= rightSideOfTheEquation->calculate()){
                    Command* c = commandsMap[code.front()];
                    whileFlag=1;
                    c->execute(whileList);
                }
                whileFlag=0;
                break;

            }*/

            //code.pop_front(); /// getting rid from the }


    }
};

void parser(list<string>* code){
    while(!code->empty()){
        Command* c = commandsMap[code->front()];
        if(c!=NULL) {
            c->execute(code);
        }
        else{ // is variable
            string varName = code->front();
            code->pop_front();
            code->pop_front();
            float value = stof(code->front());
            code->pop_front();
            localVariables[varName].value = value;
        }
        //functions?
    }

}

int main(int argc, char* argv[]) {
    OpenServerCommand c1 = OpenServerCommand();
    Command& c2 = c1;
    commandsMap.emplace("openDataServer",&c2);
    ConnectCommand c3 = ConnectCommand();
    Command& c4 = c3;
    commandsMap.emplace("connectControlClient",&c4);
    VarCommand c5 = VarCommand();
    Command& c6 = c5;
    commandsMap.emplace("var", &c6);
    PrintCommand c7 = PrintCommand();
    Command& c8 = c7;
    commandsMap.emplace("Print", &c8);
    SleepCommand c9 = SleepCommand();
    Command& c10 = c9;
    commandsMap.emplace("Sleep", &c10);
    WhileCommand c11 = WhileCommand();
    Command& c12 = c11;
    commandsMap.emplace("while", &c12);

    list<string> code = lexer(argv[1]);
    parser(&code);

    return 0;

}