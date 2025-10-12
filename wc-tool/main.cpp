#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

// Abstract classes

class CommandDispatcher {
    public:
    virtual void dispatch(const vector<string>& args) = 0;
};

class CommandExecutor {
    public:
    virtual void execute(vector<string> args) = 0;
    virtual ~CommandExecutor() = default;
};

// Factories

class CommandExecutorFactory {
    public:
    static CommandExecutor* getExecutor(const string& flag) {
        // Byte counter
        if (flag == "-c") {
            return new ByteCounter();
        }
        // Line counter
        else if(flag == "-l") {
            return new LineCounter();
        }
        // WordCounter
        else if(flag == "-w"){
            return new WordCounter();
        }
        // Character Counter
        else if(flag == "-m"){
            return new ByteCounter();
        }
        return nullptr;
    }
};

class CommandDispatcherFactory {
    public:
    static CommandDispatcher* getDispatcher(const string& name){
        if(name == "wc"){
            return new wcDispatcher();
        }

        return nullptr;
    }
};

// Parses/Utitlity class

class UserInput {
    public:
    static string getInput(){
        string cmd;
        getline(cin,cmd);
        return cmd;
    }
};

class CommandParser {
    public:
    static vector<string> parse(string cmd){
        vector<string> args;
        string arg = "";
        
        int i = 0;
        while(i < cmd.size()){
            if(cmd[i] == ' '){
                args.push_back(arg);
                arg = "";
            }
            else {
                arg += cmd[i];
            }
            i++;
        }
        args.push_back(arg);

        return args;
    }
};

// Command Executor : Concrete classes

class ByteCounter : public CommandExecutor {
    public:
    void execute(vector<string> args) override {

        ifstream file(args[0]);
        if (!file.is_open()) {
            cerr << "[File not found] " << args[0] << endl;
            return;
        }

        string line;
        long long cnt = 0;
        while (getline(file, line)) {
            cnt += line.size() + 1;
        }
        cout << cnt << " " << args[0] << endl;
        file.close();
    }
};

class LineCounter : public CommandExecutor {
    public:
    void execute(vector<string> args) override {

        ifstream file(args[0]);
        if (!file.is_open()) {
            cerr << "[File not found] " << args[0] << endl;
            return;
        }

        string line;
        long long cnt = 0;
        while (getline(file, line)) {
            cnt++;
        }
        cout << cnt << " " << args[0] << endl;
        file.close();
    }
};

class WordCounter : public CommandExecutor {
    public:
    int getNoOfWords(string line){
        int cnt = 0;
        int i = 0;

        while(i < line.length()){
            while(i < line.length() && isspace(line[i])) i++;
            if(i < line.length()) cnt++;
            while(i < line.length() && !isspace(line[i])) i++;
        }

        return cnt;
    }

    void execute(vector<string> args) override {
        ifstream file(args[0]);
        if(!file.is_open()){
            cerr << "[File not found] " << args[0] << endl;
            return;
        }

        string line;
        long long cnt = 0;
        while(getline(file,line)){
            cnt += getNoOfWords(line);
        }

        cout << cnt << " " << args[0] << endl;
        file.close();
    }
};

// Command Dispatcher : Concrete classes

class wcDispatcher : public CommandDispatcher {
    public:
    void dispatch(const vector<string>& args) override {
        if(args.size() < 1){
            cout << "[Invalid Command] wc" << endl;
            return;
        }
        
        if(args.size() == 1){
            // execute -c -w -l
            CommandExecutor* byteCounter = CommandExecutorFactory::getExecutor("-c");
            CommandExecutor* wordCounter = CommandExecutorFactory::getExecutor("-w");
            CommandExecutor* lineCounter = CommandExecutorFactory::getExecutor("-l");

            if(byteCounter && wordCounter && lineCounter){
                byteCounter->execute(args);
                wordCounter->execute(args);
                lineCounter->execute(args);
            }

            delete byteCounter;
            delete wordCounter;
            delete lineCounter;
        }
        else {
            CommandExecutor* executor = CommandExecutorFactory::getExecutor(args[0]);
            if (executor) {
                executor->execute(vector<string>(args.begin()+1, args.end()));
                delete executor;
            } else {
                cout << "[Invalid flag] " << args[0] << endl;
            }
        }
    }
};

int main(){
    string cmd = UserInput::getInput();

    // split cmd into args
    vector<string> args = CommandParser::parse(cmd);

    if(args.size() < 2){
        cout << "[Invalid Command] " << cmd << endl;
        return 1;
    }

    // get the dispatcher
    CommandDispatcher *dispatcher = CommandDispatcherFactory::getDispatcher(args[0]);
    if(dispatcher){
        dispatcher->dispatch(vector<string>(args.begin() + 1, args.end()));
    }
    else {
        cout << "[Command not found] " << cmd << endl;
    }

    delete dispatcher;
    return 0;
}