#include <string>
#include <list>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
struct Variable{
    float value;
    string sim;
};
class Command {
public:
    virtual int execute(){
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
    // printList(textList);
    return textList;
}

list<string> code;
unordered_map<string,Command*> commandsMap;
unordered_map<string,Variable> localVariables;
unordered_map<string,string> simVariables;

class OpenServerCommand : public Command { //fix
public:
    int execute(){
        code.pop_front();
        code.pop_front();
        cout << "open server" << endl;
    }
};

class ConnectCommand : public Command { //fix
public:
    int execute(){
        code.pop_front();
        code.pop_front();
        cout << "connect" << endl;
    }
};

class VarCommand : public Command {
public:
    int execute(){
        code.pop_front();
        string varName = code.front();
        code.pop_front();
        string op = code.front();
        code.pop_front();
        code.pop_front();
        string sim = code.front();
        code.pop_front();
        struct Variable var {0,sim};
        localVariables.emplace(varName,var);
        //check if exists in simVars
        switch (str2int(sim.c_str())){
            case str2int("/instrumentation/airspeed-indicator/indicated-speed-kt"):
                simVariables.emplace("airspeed-indicator_indicated-speed-kt",varName);
                break;
            case str2int("/instrumentation/altimeter/indicated-altitude-ft"):
                simVariables.emplace("altimeter_indicated-altitude-ft",varName);
                break;
            case str2int("/instrumentation/altimeter/pressure-alt-ft"):
                simVariables.emplace("altimeter_pressure-alt-ft",varName);
                break;
            case str2int("/instrumentation/attitude-indicator/indicated-pitch-deg"):
                simVariables.emplace("attitude-indicator_indicated-pitch-deg",varName);
                break;
            case str2int("/instrumentation/attitude-indicator/indicated-roll-deg"):
                simVariables.emplace("attitude-indicator_indicated-roll-deg",varName);
                break;
            case str2int("/instrumentation/attitude-indicator/internal-pitch-deg"):
                simVariables.emplace("attitude-indicator_internal-pitch-deg",varName);
                break;
            case str2int("/instrumentation/attitude-indicator/internal-roll-deg"):
                simVariables.emplace("attitude-indicator_internal-roll-deg",varName);
                break;
            case str2int("/instrumentation/encoder/indicated-altitude-ft"):
                simVariables.emplace("encoder_indicated-altitude-ft",varName);
                break;
            case str2int("/instrumentation/encoder/pressure-alt-ft"):
                simVariables.emplace("encoder_pressure-alt-ft",varName);
                break;
            case str2int("/instrumentation/gps/indicated-altitude-ft"):
                simVariables.emplace("gps_indicated-altitude-ft",varName);
                break;
            case str2int("/instrumentation/gps/indicated-ground-speed-kt"):
                simVariables.emplace("gps_indicated-ground-speed-kt",varName);
                break;
            case str2int("/instrumentation/gps/indicated-vertical-speed"):
                simVariables.emplace("gps_indicated-vertical-speed",varName);
                break;
            case str2int("/instrumentation/heading-indicator/indicated-heading-deg"):
                simVariables.emplace("indicated-heading-deg",varName);
                break;
            case str2int("/instrumentation/magnetic-compass/indicated-heading-deg"):
                simVariables.emplace("magnetic-compass_indicated-heading-deg",varName);
                break;
            case str2int("/instrumentation/slip-skid-ball/indicated-slip-skid"):
                simVariables.emplace("slip-skid-ball_indicated-slip-skid",varName);
                break;
            case str2int("/instrumentation/turn-indicator/indicated-turn-rate"):
                simVariables.emplace("turn-indicator_indicated-turn-rate",varName);
                break;
            case str2int("/instrumentation/vertical-speed-indicator/indicated-speed-fpm"):
                simVariables.emplace("vertical-speed-indicator_indicated-speed-fpm",varName);
                break;
            case str2int("/controls/flight/aileron"):
                simVariables.emplace("flight_aileron",varName);
                break;
            case str2int("/controls/flight/elevator"):
                simVariables.emplace("flight_elevator",varName);
                break;
            case str2int("/controls/flight/rudder"):
                simVariables.emplace("flight_rudder",varName);
                break;
            case str2int("/controls/flight/flaps"):
                simVariables.emplace("flight_flaps",varName);
                break;
            case str2int("/controls/engines/engine/throttle"):
                simVariables.emplace("engine_throttle",varName);
                break;
            case str2int("/engines/engine/rpm"):
                simVariables.emplace("engine_rpm",varName);
                break;
            default:
                break;
        }
    }
};

class SleepCommand : public Command {
public:
    int execute(){
        code.pop_front();
        int timeToSleep = stoi(code.front());
        code.pop_front();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
    }
};

class PrintCommand : public Command {
public:
    int execute(){
        code.pop_front();
        string print = code.front();
        code.pop_front();
        cout << print << endl;
    }
};

class WhileCommand : public Command { //fix
public:
    int execute(){

    }
};

void parser(){
    while(!code.empty()){
        Command* c = commandsMap[code.front()];
        c->execute();
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

    code = lexer(argv[1]);
    parser();

    return 0;
}