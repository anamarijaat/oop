#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct File {
    string name;
    string extension;
    uint64_t byteSize;
};

string getFullFilename(File &f) {
    return f.name + '.' + f.extension;
}

struct Directory {
    string name;
    vector<File *> files;
    vector<Directory *> dirs;
    Directory *up;
};

bool addFile(Directory &d, File &f){
    for (size_t i = 0; i < d.files.size(); i++){
        if (getFullFilename(*d.files[i]) == getFullFilename(f)){
            return false;
        }
    }
    d.files.push_back(&f);
    return true;
}

bool addDirectory(Directory &d_1, &d_2){
    for(size_t i = 0; i < d_1.dirs.size(); i++){
        if (d_1.dirs->name[i] == d_2.name[i]){
            return false;
        }
    }
    d_1.dirs.push_back(&d_2);
    d_2.up = &d_1;
    return true;
}

void function_up(Directory &d){
    if(d.up){
        cout << d.up->name << endl;
    }
    else{
        cout << "/(root)" << endl;
    }
}

uint64_t getDirectorySize(Directory &d) {
    uint64_t sz = 0;
    for (size_t i = 0; i < d.files.size(); i++) {
        sz += d.files[i]->byteSize;
    }
    for (size_t i = 0; i < d.dirs.size(); i++){
        sz += getDirectorySize(*d.dirs[i]);
    }

    return sz;
}

bool moveFile(const string &filename, Directory &from, Directory &to) {
    File *file = nullptr;
    for (size_t i = 0; i < from.files.size(); i++) {
        if (filename == getFullFilename(*from.files[i])) {
            file = from.files[i];
            for (size_t j = i; j < from.files.size() - 1; j++) {
                from.files[j] = from.files[j + 1];
            }
            from.files.pop_back();

            to.files.push_back(file);
            return true;
        }
    }
    return false;
}

void printDir(Directory &d) {
    std::cout << "d: " << d.name << '\n';
    
    for (size_t i = 0; i < d.files.size(); i++) {
        std::cout << " f " << getFullFilename(*d.files[i]) << '\n';
    }
    for (size_t i = 0; i < d.dirs.size(); i++) {
        printDir(*d.dirs[i]);
    }
    std::cout << "Size: " << getDirectorySize(d) << " B\n";
}


bool Find(Directory &d, const string &target) {
    for (size_t i = 0; i < d.files.size(); i++) {
        if (getFullFilename(*d.files[i]) == target) {
            cout << "File found: " << target << endl;
            return true;
        }
    }

    for (size_t i = 0; i < d.dirs.size(); i++) {
        if (d.dirs[i]->name == target) {
            cout << "Directory found: " << target << endl;
            return true;
        }
    }

    for (size_t i = 0; i < d.dirs.size(); i++) {
        if (Find(*d.dirs[i], target)) {
            return true;
        }
    }

    return false;
}


int main() {
    Directory my;
    my.name = "documents";
    File x, y, z;
    x.name = "X";
    x.extension = "txt";
    x.byteSize = 1024;

    y.name = "Y";
    y.extension = "jpeg";
    y.byteSize = 56281022;

    z.name = "z";
    z.extension = "mov";
    z.byteSize = 1556281022;

    addFile(x);
    addFile(y);
    addFile(z);

    y.name = "YYY";

    Directory child;
    child.name = "Child One";

    addDirectory(child);
    
    Find(my, "z");
    

    printDir(my);

    moveFile(getFullFilename(z), my, child);
    cout << "AFTER MOVE\n";
    printDir(my);

    printDir(child);
    return 0;
}
