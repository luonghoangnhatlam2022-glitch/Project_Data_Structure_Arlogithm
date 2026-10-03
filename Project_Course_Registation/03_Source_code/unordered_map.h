#pragma once

template <typename K, typename V>
struct unordered_map {
    struct Node {
        K first; V second; Node* next;
        Node(const K& k) : first(k), second(), next(nullptr) {}
    };

    int cap;
    int sz;
    Node** table;

    // Cấp phát mảng tĩnh và tự động gán nullptr cho toàn bộ phần tử nhờ cặp ngoặc ()
    unordered_map() : cap(10007), sz(0), table(new Node*[10007]()) {}

    // BỔ SUNG: Hàm hủy để dọn dẹp bộ nhớ khi Bảng băm kết thúc vòng đời
    ~unordered_map() {
        for (int i = 0; i < cap; i++) {
            Node* curr = table[i];
            while (curr) {
                Node* next = curr->next;
                delete curr; // Xóa từng nút
                curr = next;
            }
        }
        delete[] table; // Xóa mảng con trỏ
    }

    int hash(const K& k) {
        long long h = 0;
        for (size_t i = 0; i < k.length(); i++) h = (h * 31 + k[i]) % cap;
        return h;
    }

    void rehash() {
        Node** old_tbl = table;
        int old_cap = cap;

        table = new Node*[cap = old_cap * 2 + 1](); // Tạo mảng mới x2 dung lượng

        for (int i = 0; i < old_cap; i++) {
            for (Node* curr = old_tbl[i]; curr;) {
                Node* next = curr->next;
                int idx = hash(curr->first);
                curr->next = table[idx];
                table[idx] = curr;
                curr = next;
            }
        }
        // BỔ SUNG: Xóa mảng cũ sau khi đã chuyển hết dữ liệu sang mảng mới
        delete[] old_tbl;
    }

    struct Iterator {
        Node* node; Node** tbl; int bkt; int cap;
        Iterator(Node* n, Node** t, int b, int c) : node(n), tbl(t), bkt(b), cap(c) {}

        bool operator==(const Iterator& o) const { return node == o.node; }
        bool operator!=(const Iterator& o) const { return node != o.node; }
        Node* operator->() { return node; }

        Iterator& operator++() {
            if (node) node = node->next;
            while (!node && ++bkt < cap) node = tbl[bkt];
            return *this;
        }
    };

    Iterator end() { return Iterator(nullptr, table, cap, cap); }

    // Tái sử dụng logic ++ để tìm phần tử đầu tiên siêu gọn
    Iterator begin() {
        Iterator it(nullptr, table, -1, cap);
        return ++it;
    }

    Iterator find(const K& k) {
        int i = hash(k);
        for (Node* curr = table[i]; curr; curr = curr->next)
            if (curr->first == k) return Iterator(curr, table, i, cap);
        return end();
    }

    V& operator[](const K& k) {
        int i = hash(k);
        for (Node* curr = table[i]; curr; curr = curr->next)
            if (curr->first == k) return curr->second;

        // Gộp lệnh tăng size và kiểm tra Load factor trên 1 dòng
        if (++sz > cap * 0.75) { rehash(); i = hash(k); }

        Node* n = new Node(k);
        n->next = table[i];
        table[i] = n;
        return n->second;
    }

    void erase(const K& k) {
        int i = hash(k);
        Node *curr = table[i], *prev = nullptr;
        while (curr) {
            if (curr->first == k) {
                if (prev) prev->next = curr->next;
                else table[i] = curr->next;

                // BỔ SUNG: Trả lại bộ nhớ của nút bị xóa thay vì chỉ ngắt liên kết
                delete curr;
                sz--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void erase(Iterator it) { if (it.node) erase(it.node->first); }
};