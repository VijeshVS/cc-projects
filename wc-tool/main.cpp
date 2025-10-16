#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class STDINStream {
    public:
        STDINStream() {}
        static void processStream(){
            int charCount = 0;
            int lineCount = 0;
            int byteCount = 0;

            string line;
            while(getline(cin, line)){
                charCount += line.length();
                lineCount++;
                byteCount += line.length() + 1; // +1 for newline character
            }

            cout << charCount << " " << lineCount << " " << byteCount << endl;
        }
};

class StreamCount {
    public:
        int charCount;
        int lineCount;
        int byteCount;
        StreamCount(int c, int l, int b) : charCount(c), lineCount(l), byteCount(b) {}
};

class FileStream {
    string filename;
    public:
        FileStream(const string &filename) : filename(filename) {}

        StreamCount processStream(){
            ifstream file(filename);
            if(!file.is_open()){
                cerr << "Error: Could not open file " << filename << endl;
                return StreamCount(0, 0, 0);
            }

            int charCount = 0;
            int lineCount = 0;
            int byteCount = 0;

            string line;
            while(getline(file, line)){
                charCount += line.length();
                lineCount++;
                byteCount += line.length() + 1; // +1 for newline character
            }

            file.close();
            return StreamCount(charCount, lineCount, byteCount);
        }
};

int main(int argc, char *argv[]){

    bool showChars = false, showLines = false, showBytes = false;

    if(argc > 1 && argv[1][0] == '-'){
        char flag = argv[1][1];
        if(flag == 'm') showChars = true;
        else if(flag == 'l') showLines = true;
        else if(flag == 'c') showBytes = true;
    }

    int filesStart = (argc > 1 && argv[1][0] == '-') ? 2 : 1;
    
    vector<string> files;
    for(int i = filesStart;i<argc;i++){
        files.push_back(argv[i]);
    }

    if(files.empty()){
        STDINStream::processStream();
    }
    else {
        StreamCount total(0, 0, 0);
        for(const string &file : files){
            FileStream fs(file);
            StreamCount sc = fs.processStream();
            total.charCount += sc.charCount;
            total.lineCount += sc.lineCount;
            total.byteCount += sc.byteCount;

            if(!showBytes && !showChars && !showLines){
                cout << sc.charCount << " " << sc.lineCount << " " << sc.byteCount << " " << file << endl;
            } 
            else if(showChars){
                cout << sc.charCount << " " << file << endl;
            }
            else if(showLines){
                cout << sc.lineCount << " " << file << endl;
            }
            else if(showBytes){
                cout << sc.byteCount << " " << file << endl;
            }
        }
        if(files.size() > 1) {
            if(!showBytes && !showChars && !showLines){
                cout << total.charCount << " " << total.lineCount << " " << total.byteCount << " total" << endl;
            } 
            else if(showChars){
                cout << total.charCount << " total" << endl;
            }
            else if(showLines){
                cout << total.lineCount << " total" << endl;
            }
            else if(showBytes){
                cout << total.byteCount << " total" << endl;
            }
        }
    }
}