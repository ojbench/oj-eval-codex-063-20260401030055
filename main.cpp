#include <bits/stdc++.h>
#include "MemoryRiver.hpp"

struct Node {
    int a;
    long long b;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MemoryRiver<Node, 2> mr("storage.bin");
    mr.initialise();

    int q; // number of operations
    if (!(std::cin >> q)) return 0;
    std::vector<int> indexes;
    while (q--) {
        int op; // 1 write, 2 update, 3 read, 4 info get, 5 info write
        std::cin >> op;
        if (op == 1) {
            Node t; std::cin >> t.a >> t.b;
            int idx = mr.write(t);
            indexes.push_back(idx);
            std::cout << idx << "\n";
        } else if (op == 2) {
            int i; Node t; std::cin >> i >> t.a >> t.b;
            int idx = indexes[i];
            mr.update(t, idx);
        } else if (op == 3) {
            int i; std::cin >> i;
            int idx = indexes[i];
            Node t; mr.read(t, idx);
            std::cout << t.a << ' ' << t.b << "\n";
        } else if (op == 4) {
            int n; std::cin >> n; int tmp = 0; mr.get_info(tmp, n); std::cout << tmp << "\n";
        } else if (op == 5) {
            int n, tmp; std::cin >> n >> tmp; mr.write_info(tmp, n);
        }
    }
    return 0;
}

