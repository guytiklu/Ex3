#include <string>
#include <string.h>
#include <list>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include "ex1.h"
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mutex>
#include <algorithm>


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
string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

list<string>lexer(string name){
    list<string> textList;
    fstream fp;
    fp.open(name);
    string line;
    if(!fp){
        cout << "Unable to open file";
    }
    for(line; getline( fp, line ); ){ /// reading line by line the text
        string testline;
        string word;
        testline= removeSpaces(testline);
        int flag=0;
        int flag1=0;
        int flag2=0;
        int flagprint=0;


        for (auto x : line){ /// now we will read letter by letter
            if((x=='('|| x== ')')&&flag2==0){
                if(x=='('&&word.length()!=0) {
                    textList.push_back(word);
                    word = "";
                }
                else if(x==')'){
                    textList.push_back(word);
                    word = "";
                }
            }
            else if(x=='='){
                flag2=1;
                if(word.length()==0) {
                    word = '=';
                    textList.push_back(word);
                    word = "";
                }
                else if(word.length()==1){
                    word= word+x;
                    textList.push_back(word);
                    word = "";
                }
                else{
                    textList.push_back(word);
                    word= '=';
                    textList.push_back(word);
                    word="";

                }
            }
            else if(x==' ' && flag==0 &&flag2==0){
                if(word.length()!=0) {
                    textList.push_back(word);
                    word = "";
                }
            }
            else if(x==' ' && flag==0 &&flag2==1){
                continue;
            }
            else if(x==','){
                textList.push_back(word);
                word="";
            }
            else if(x=='"'&& flagprint==1){
                word = word + x;
                flag=1;
            }
            else if(x=='"'&& flag==0){ /// for the first "
                word="";
                flag=1;
            }
            else if(x=='"'&& flag==1){ /// for the second "
                flag=0;
            }
            else if(x=='\t'){
                continue;
            }
            else if(x=='{'){
                continue;
            }

            else {
                word = word + x;
            }
            if(textList.size()!=0){
                string front= textList.back();
                if(front=="Print"){
                    flagprint=1;
                }
            }

        }
        if(word.length()!=0) {
            textList.push_back(word);
            word = "";
        }
    }

    //printList(textList);
    return textList;
}

mutex mutexlock;
unordered_map<string,Command*> commandsMap;
unordered_map<string,Var*> localVariables;
unordered_map<string,Var> simVariables;
bool connected = false;
list<string> messagesToServer;

void createExpression(Expression** exp, string str){
    Interpreter* inter = new Interpreter();
    for(auto x : localVariables){
        string setVariable = x.first;
        setVariable += "=";
        setVariable += to_string((double)x.second->value);
        inter->setVariables(setVariable);
    }
    *exp=inter->interpret(str);
}
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
    }

    cout<< "Waiting for server to connect . . ."<<endl;
    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,(socklen_t *) &address);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        return -4;
    }
    connected=true;

    struct Var var0 {0,"/instrumentation/airspeed-indicator/indicated-speed-kt"};
    simVariables.emplace("/instrumentation/airspeed-indicator/indicated-speed-kt",var0);
    struct Var var1 {0,"/instrumentation/altimeter/indicated-altitude-ft"};
    simVariables.emplace("/instrumentation/altimeter/indicated-altitude-ft",var1);
    struct Var var2 {0,"/instrumentation/altimeter/pressure-alt-ft"};
    simVariables.emplace("/instrumentation/altimeter/pressure-alt-ft",var2);
    struct Var var3 {0,"/instrumentation/attitude-indicator/indicated-pitch-deg"};
    simVariables.emplace("/instrumentation/attitude-indicator/indicated-pitch-deg",var3);
    struct Var var4 {0,"/instrumentation/attitude-indicator/indicated-roll-deg"};
    simVariables.emplace("/instrumentation/attitude-indicator/indicated-roll-deg",var4);
    struct Var var5 {0,"/instrumentation/attitude-indicator/internal-pitch-deg"};
    simVariables.emplace("/instrumentation/attitude-indicator/internal-pitch-deg",var5);
    struct Var var6 {0,"/instrumentation/attitude-indicator/internal-roll-deg"};
    simVariables.emplace("/instrumentation/attitude-indicator/internal-roll-deg",var6);
    struct Var var7 {0,"/instrumentation/encoder/indicated-altitude-ft"};
    simVariables.emplace("/instrumentation/encoder/indicated-altitude-ft",var7);
    struct Var var8 {0,"/instrumentation/encoder/pressure-alt-ft"};
    simVariables.emplace("/instrumentation/encoder/pressure-alt-ft",var8);
    struct Var var9 {0,"/instrumentation/gps/indicated-altitude-ft"};
    simVariables.emplace("/instrumentation/gps/indicated-altitude-ft",var9);
    struct Var var10 {0,"/instrumentation/gps/indicated-ground-speed-kt"};
    simVariables.emplace("/instrumentation/gps/indicated-ground-speed-kt",var10);
    struct Var var11 {0,"/instrumentation/gps/indicated-vertical-speed"};
    simVariables.emplace("/instrumentation/gps/indicated-vertical-speed",var11);
    struct Var var12 {0,"/instrumentation/heading-indicator/indicated-heading-deg"};
    simVariables.emplace("/instrumentation/heading-indicator/indicated-heading-deg",var12);
    struct Var var13 {0,"/instrumentation/magnetic-compass/indicated-heading-deg"};
    simVariables.emplace("/instrumentation/magnetic-compass/indicated-heading-deg",var13);
    struct Var var14 {0,"/instrumentation/slip-skid-ball/indicated-slip-skid"};
    simVariables.emplace("/instrumentation/slip-skid-ball/indicated-slip-skid",var14);
    struct Var var15 {0,"/instrumentation/turn-indicator/indicated-turn-rate"};
    simVariables.emplace("/instrumentation/turn-indicator/indicated-turn-rate",var15);
    struct Var var16 {0,"/instrumentation/vertical-speed-indicator/indicated-speed-fpm"};
    simVariables.emplace("/instrumentation/vertical-speed-indicator/indicated-speed-fpm",var16);
    struct Var var17 {0,"/controls/flight/aileron"};
    simVariables.emplace("/controls/flight/aileron",var17);
    struct Var var18 {0,"/controls/flight/elevator"};
    simVariables.emplace("/controls/flight/elevator",var18);
    struct Var var19 {0,"/controls/flight/rudder"};
    simVariables.emplace("/controls/flight/rudder",var19);
    struct Var var20 {0,"/controls/flight/flaps"};
    simVariables.emplace("/controls/flight/flaps",var20);
    struct Var var21 {0,"/controls/engines/engine/throttle"};
    simVariables.emplace("/controls/engines/engine/throttle",var21);
    struct Var var22 {0,"/engines/engine/rpm"};
    simVariables.emplace("/engines/engine/rpm",var22);
    struct Var var23 {0,"/instrumentation/heading-indicator/offset-deg"};
    simVariables.emplace("/instrumentation/heading-indicator/offset-deg",var23);

    while (true) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);

        vector<string> v;
        split( buffer, v, ',');
        string values[24];
        int counter=0;
        for (auto x : v){
            values[counter]=x;
            counter++;
        }
        simVariables["/instrumentation/airspeed-indicator/indicated-speed-kt"].value=stof(values[0]);
        simVariables["/instrumentation/heading-indicator/offset-deg"].value=stof(values[1]);
        simVariables["/instrumentation/altimeter/indicated-altitude-ft"].value=stof(values[2]);
        simVariables["/instrumentation/altimeter/pressure-alt-ft"].value=stof(values[3]);
        simVariables["/instrumentation/attitude-indicator/indicated-pitch-deg"].value=stof(values[4]);
        simVariables["/instrumentation/attitude-indicator/indicated-roll-deg"].value=stof(values[5]);
        simVariables["/instrumentation/attitude-indicator/internal-pitch-deg"].value=stof(values[6]);
        simVariables["/instrumentation/attitude-indicator/internal-roll-deg"].value=stof(values[7]);
        simVariables["/instrumentation/encoder/indicated-altitude-ft"].value=stof(values[8]);
        simVariables["/instrumentation/encoder/pressure-alt-ft"].value=stof(values[9]);
        simVariables["/instrumentation/gps/indicated-altitude-ft"].value=stof(values[10]);
        simVariables["/instrumentation/gps/indicated-ground-speed-kt"].value=stof(values[11]);
        simVariables["/instrumentation/gps/indicated-vertical-speed"].value=stof(values[12]);
        simVariables["/instrumentation/heading-indicator/indicated-heading-deg"].value=stof(values[13]);
        simVariables["/instrumentation/magnetic-compass/indicated-heading-deg"].value=stof(values[14]);
        simVariables["/instrumentation/slip-skid-ball/indicated-slip-skid"].value=stof(values[15]);
        simVariables["/instrumentation/turn-indicator/indicated-turn-rate"].value=stof(values[16]);
        simVariables["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"].value=stof(values[17]);
        simVariables["/controls/flight/aileron"].value=stof(values[18]);
        simVariables["/controls/flight/elevator"].value=stof(values[19]);
        simVariables["/controls/flight/rudder"].value=stof(values[20]);
        simVariables["/controls/flight/flaps"].value=stof(values[21]);
        simVariables["/controls/engines/engine/throttle"].value=stof(values[22]);
        simVariables["/engines/engine/rpm"].value=stof(values[23]);

        /*for (auto x : simVariables){
            cout << x.second.value ;
            cout << ",";
        }
        cout << endl;*/
    }
}
int clientThread(string address1, int port){

    const char * address2 = address1.c_str();
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(address2);  //the localhost address
    address.sin_port = htons(port);
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    // Requesting a connection with the server on local host with port 8081
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server"<<std::endl;
        return -2;
    } else {
        cout<<"Connected to server!"<<endl;
    }
    //if here we made a connection

    while(true){
        if(!messagesToServer.empty()){
            mutexlock.lock();
            string message = messagesToServer.front();
            message = message + "\r\n";
            messagesToServer.pop_front();
            int is_sent = send(client_socket, message.c_str(), message.length(), 0);
            mutexlock.unlock();
            if (is_sent == -1) {
                std::cout << "Error sending message in server function" << std::endl;
            }
        }
    }
}
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
            Expression* exp = nullptr;
            createExpression(&exp,code->front()); //##################################
            code->pop_front();
            float value = exp->calculate();
            localVariables[varName]->value = value;
            string command = "set ";
            command += localVariables[varName]->sim;
            command += " ";
            command += to_string(value);
            mutexlock.lock();
            messagesToServer.push_front(command);
            mutexlock.unlock();
        }
    }

}

class OpenServerCommand : public Command {
public:
    int execute(list<string>* code) {
        code->pop_front();
        string pop = code->front();
        Expression* exp = nullptr;
        createExpression(&exp,pop);
        thread tr(serverThread,exp->calculate());
        code->pop_front();
        tr.detach();
        while(!connected){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

class ConnectCommand : public Command { //fix
public:
    int execute(list<string>* code){
        code->pop_front();
        string address1 = code->front();
        code->pop_front();
        Expression* exp = nullptr;
        createExpression(&exp,code->front());
        int port = (int)exp->calculate();
        code->pop_front();
        thread tr(clientThread,address1,port);
        tr.detach();
    }
};

class VarCommand : public Command { // fix =, maybe make class
public:
    int execute(list<string>* code) {

        code->pop_front();
        string varName = code->front();
        code->pop_front();
        string op = code->front();
        code->pop_front();
        string sim;
        float value;
        Var* var;
        if (op.compare("=") == 0) { //new var
            sim = localVariables[code->front()]->sim;
            value = localVariables[code->front()]->value;
            code->pop_front();
            var = new Var();
            var->sim=sim;
            var->value=value;
        } else {
            code->pop_front();
            sim = code->front();
            code->pop_front();
            value = 0;
            if (op.compare("<-") == 0) { //pointer to sim var X
                var = &simVariables[sim];
            } else { // -> // new var X
                var = new Var();
                var->value=0;
                var->sim=sim;
            }
        }
        localVariables.emplace(varName, var);
        }
};

class SleepCommand : public Command {
public:
    int execute(list<string>* code){
        code->pop_front();
        Expression* exp = nullptr;
        createExpression(&exp,code->front());
        int timeToSleep = exp->calculate();
        code->pop_front();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
        cout<<"omo"<<endl;
    }
};

class PrintCommand : public Command {
public:
    int execute(list<string>* code){
        code->pop_front();
        string print = code->front();
        code->pop_front();
        if(print.at(0)=='"'){ //tziteta
            cout<<print<<endl;
        } else { //expression
            Expression* exp = nullptr;
            createExpression(&exp,print);
            cout<<exp->calculate()<<endl;
        }
    }
};

class WhileCommand : public Command {
public:
    int execute(list<string>* code){

        /// we will get the equation in the while:

        code->pop_front(); ///poping the "while"
        string left = code->front() ;
        code->pop_front(); ///poping the "var" =rpm
        string op= code->front();

        code->pop_front(); ///poping the "sign"
        string right=code->front();
        int temp2Length= right.size();

        code->pop_front(); ///poping the "right side of the equation"
        string equatoin = left +op +right;

        list<string> whileList;
        /// building the whileList
        string front= code->front();
        while(front != "}"){
            whileList.push_back(code->front());
            code->pop_front();
            front= code->front();
        }
        list<string> tempList=whileList;
        /// now we have commands we need to do inside the while, we put them in a diffrent list

        Expression* leftSideOfTheEquation = nullptr;
        createExpression(&leftSideOfTheEquation,left);
        Expression* rightSideOfTheEquation = nullptr;
        createExpression(&rightSideOfTheEquation,right);

        const char * c = op.c_str();
        int valueOfSigh= str2int((c));

        switch(valueOfSigh) {
            case str2int("<=") :
                while(leftSideOfTheEquation->calculate()<= rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;
            case str2int(">=") :
                while(leftSideOfTheEquation->calculate()>= rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;
            case str2int("<") :
                while(leftSideOfTheEquation->calculate()< rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;
            case str2int(">") :
                while(leftSideOfTheEquation->calculate()> rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;
            case str2int("==") :
                while(leftSideOfTheEquation->calculate()== rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;
            case str2int("!=") :
                while(leftSideOfTheEquation->calculate()!= rightSideOfTheEquation->calculate()){
                    parser(&tempList);
                    tempList=whileList;
                    createExpression(&leftSideOfTheEquation,left);
                    createExpression(&rightSideOfTheEquation,right);
                }
                break;

        }
        code->pop_front(); /// getting rid from the }

        /*messagesToServer.push_front("set controls/flight/rudder -1");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        messagesToServer.push_front("set controls/flight/rudder 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));*/
    }
};

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