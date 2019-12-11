#include <iostream>
#include <list>
#include <string>

using namespace std;
list<string> lexer(FILE* fp){
    //eldad
    //input : file
    //output : list of all the elements in the file acoording to the rules.
}

void parser(list<string> list){
    //functions will run across the list and execute the commands.
}

int main(int argc, char* argv[]) {
    FILE* fp;
    fp = fopen(argv[1],"r");
    list<string> list = lexer(fp);
    fclose(fp);







    return 0;
}