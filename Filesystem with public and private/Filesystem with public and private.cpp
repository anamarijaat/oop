#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct File {
private:
    string name;
    string extension;
    uint64_t byteSize;
public:
    string getFullFilename(File &f) {
        return f.name + '.' + f.extension;
    }
    void setbyteSize(uint64_t n){
        byteSize = n;
    }
    void setname(const string n){
        name = n;
    }
    void setextension(const string n){
        extension = n;
    }
    uint64_t getbyteSize(){
        return byteSize;
    }
};


struct Directory {
private:
    string name;
    vector<File *> files;
    vector<Directory *> dirs;
    Directory *up;
public:
    void setname(const string n){
        name = n;
    }
    
    bool addFile(Directory &d, File &f){
        for (size_t i = 0; i < d.files.size(); i++){
            if (f.getFullFilename(*d.files[i]) == f.getFullFilename(f)){
                return false;
            }
        }
        d.files.push_back(&f);
        return true;
    }

    bool addDirectory(Directory &d_1,Directory &d_2){
        for(size_t i = 0; i < d_1.dirs.size(); i++){
            if (d_1.dirs[i]->name == d_2.name){
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
            sz += d.files[i]->getbyteSize();
        }
        for (size_t i = 0; i < d.dirs.size(); i++){
            sz += getDirectorySize(*d.dirs[i]);
        }

        return sz;
    }

    bool moveFile(const string &filename, Directory &from, Directory &to) {
        File *file = nullptr;
        for (size_t i = 0; i < from.files.size(); i++) {
            if (filename == from.files[i]->getFullFilename(*from.files[i])) {
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
            std::cout << " f " << d.files[i]->getFullFilename(*d.files[i]) << '\n';
        }
        for (size_t i = 0; i < d.dirs.size(); i++) {
            printDir(*d.dirs[i]);
        }
        std::cout << "Size: " << getDirectorySize(d) << " B\n";
    }


    bool Find(Directory &d, const string &target) {
        for (size_t i = 0; i < d.files.size(); i++) {
            if (d.files[i]->getFullFilename(*d.files[i]) == target) {
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

};



int main() {
    Directory my;
    my.setname("documents");
    File x, y, z;
    x.setname("X");
    x.setextension("txt");
    x.setbyteSize(1024);

    y.setname("Y");
    y.setextension("jpeg");
    y.setbyteSize(56281022);

    z.setname("z");
    z.setextension("mov");
    z.setbyteSize(1556281022);

    my.addFile(my,x);
    my.addFile(my,y);
    my.addFile(my,z);

    y.setname("YYY");

    Directory child;
    child.setname("Child One");

    my.addDirectory(my,child);
    
    my.Find(my, "z");
    

    my.printDir(my);

    my.moveFile(z.getFullFilename(z), my, child);
    cout << "AFTER MOVE\n";
    my.printDir(my);

    my.printDir(child);
    return 0;
}



