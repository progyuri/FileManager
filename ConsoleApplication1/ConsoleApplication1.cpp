//Реализовать простейший файловый менеджер с использованием ООП(классы,
//    наследование и т.п.).
//    Файловый менеджер должен иметь следующие возможности :
//■ показывать содержимое дисков;
//■ создавать папки / файлы;
//■ удалять папки / файлы;
//■ переименовывать папки / файлы;
//■ копировать / переносить папки / файлы;
//■ вычислять размер папки / файла;
//■ выполнять поиск по маске(с поиском в подпапках)

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <string>

using namespace std;
namespace fs = std::filesystem;





class FileManager {
public:
    FileManager(const fs::path& startPath = fs::current_path()) : currentPath(startPath) {}

    void listDirectories() const {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            std::cout << (entry.is_directory() ? "DIR: " : "FILE: ") << entry.path().filename() << std::endl;
        }
    }

    void createDirectory(const std::string& name) {
        fs::create_directory(currentPath / name);
    }

    void createFile(const std::string& name, const std::string& content = "") {
        std::ofstream(currentPath / name) << content;
    }

    void deletePath(const std::string& name) {
        fs::remove_all(currentPath / name);
    }

    void rename(const std::string& oldName, const std::string& newName) {
        fs::rename(currentPath / oldName, currentPath / newName);
    }

    void move(const std::string& name, const fs::path& newPath) {
        fs::rename(currentPath / name, newPath / name);
    }

    void copy(const std::string& name, const fs::path& newPath) {
        const auto& srcPath = currentPath / name;
        if (fs::is_directory(srcPath)) {
            fs::copy(srcPath, newPath / name, fs::copy_options::recursive);
        }
        else {
            fs::copy_file(srcPath, newPath / name);
        }
    }

    void getSize(const std::string& name) {
        auto path = currentPath / name;
        uintmax_t size = 0;
        if (fs::is_directory(path)) {
            for (const auto& file : fs::recursive_directory_iterator(path)) {
                if (fs::is_regular_file(file)) {
                    size += fs::file_size(file);
                }
            }
        }
        else {
            size = fs::file_size(path);
        }
        std::cout << "Total size: " << size << " bytes" << std::endl;
    }

    void search(const std::string& pattern) {
        for (const auto& file : fs::recursive_directory_iterator(currentPath)) {
            if (file.path().filename().string().find(pattern) != std::string::npos) {
                std::cout << file.path().relative_path() << std::endl;
            }
        }
    }

private:
    fs::path currentPath;
};

void main() {
    FileManager fm;
    std::string input;
    std::string name, newName;
    std::string path;
    std::string pattern;

    while (true) {
        std::cout << "\n--- File Manager Menu ---\n";
        std::cout << "1. List Directories and Files\n";
        std::cout << "2. Create Directory\n";
        std::cout << "3. Create File\n";
        std::cout << "4. Delete Path\n";
        std::cout << "5. Rename Path\n";
        std::cout << "6. Move Path\n";
        std::cout << "7. Copy Path\n";
        std::cout << "8. Get Size\n";
        std::cout << "9. Search\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::getline(std::cin, input);
        int choice = std::stoi(input);

        switch (choice) {
        case 1:
            fm.listDirectories();
            break;
        case 2:
            std::cout << "Enter directory name: ";
            std::getline(std::cin, name);
            fm.createDirectory(name);
            break;
        case 3:
            std::cout << "Enter file name: ";
            std::getline(std::cin, name);
            std::cout << "Enter content (optional): ";
            std::string content;
            std::getline(std::cin, content);
            fm.createFile(name, content);
            break;
        case 4:
            std::cout << "Enter path name to delete: ";
            std::getline(std::cin, name);
            fm.deletePath(name);
            break;
        case 5:
            std::cout << "Enter old name: ";
            std::getline(std::cin, name);
            std::cout << "Enter new name: ";
            std::getline(std::cin, newName);
            fm.rename(name, newName);
            break;
        case 6:
            std::cout << "Enter path name to move: ";
            std::getline(std::cin, name);
            std::cout << "Enter new path: ";
            std::getline(std::cin, path);
            fm.move(name, path);
            break;
        case 7:
            std::cout << "Enter path name to copy: ";
            std::getline(std::cin, name);
            std::cout << "Enter new path: ";
            std::getline(std::cin, path);
            fm.copy(name, path);
            break;
        case 8:
            std::cout << "Enter path name to get size: ";
            std::getline(std::cin, name);
            fm.getSize(name);
            break;
        case 9:
            std::cout << "Enter search pattern: ";
            std::getline(std::cin, pattern);
            fm.search(pattern);
            break;
        case 0:
            std::cout << "Exiting...";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

   
}