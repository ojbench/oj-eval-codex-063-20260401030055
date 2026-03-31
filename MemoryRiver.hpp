// MemoryRiver.hpp - minimal file-based storage with optional header info
#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

    void ensure_open(std::ios::openmode mode) {
        if (file.is_open()) file.close();
        file.open(file_name, mode | std::ios::binary);
    }

public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        // Create/overwrite file and write header ints initialized to 0
        file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Read n-th (1-based) int from header into tmp
    void get_info(int &tmp, int n) {
        if (n > info_len || n <= 0) return;
        ensure_open(std::ios::in);
        file.seekg(static_cast<std::streamoff>((n - 1) * sizeof(int)), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Write tmp into n-th (1-based) header int
    void write_info(int tmp, int n) {
        if (n > info_len || n <= 0) return;
        ensure_open(std::ios::in | std::ios::out);
        file.seekp(static_cast<std::streamoff>((n - 1) * sizeof(int)), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // Append object t to file and return its starting byte index
    int write(T &t) {
        // Open for read/write; create if not exists
        ensure_open(std::ios::in | std::ios::out);
        if (!file.good()) {
            // If opening failed (e.g., file does not exist), initialize file
            file.clear();
            file.open(file_name, std::ios::out | std::ios::binary);
            int zero = 0;
            for (int i = 0; i < info_len; ++i) file.write(reinterpret_cast<char *>(&zero), sizeof(int));
            file.close();
            ensure_open(std::ios::in | std::ios::out);
        }
        file.seekp(0, std::ios::end);
        std::streamoff pos = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
        return static_cast<int>(pos);
    }

    // Overwrite object at given index with t
    void update(T &t, const int index) {
        ensure_open(std::ios::in | std::ios::out);
        file.seekp(static_cast<std::streamoff>(index), std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    // Read object at given index into t
    void read(T &t, const int index) {
        ensure_open(std::ios::in);
        file.seekg(static_cast<std::streamoff>(index), std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    // Delete object at index. Minimal implementation without space reclamation.
    void Delete(int /*index*/) {
        // Intentionally left blank for minimal compliance. Space reclamation optional.
    }
};

#endif // BPT_MEMORYRIVER_HPP

