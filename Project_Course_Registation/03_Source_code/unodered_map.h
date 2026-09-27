#pragma once
#include <string>

template <typename K, typename V>
struct unordered_map {
private:
    struct Node {
        K first;
        V second;
        Node* next;
        Node(const K& k) : first(k), second(), next(nullptr) {}
    };

    static const int TABLE_SIZE = 10007;
    Node* table[TABLE_SIZE];
    int hashFunction(const K& key) {
        long long hash = 0;
        for (size_t i = 0; i < key.length(); i++) {
            hash = (hash * 31 + key[i]) % TABLE_SIZE;
        }
        return hash;
    }

public:
    unordered_map() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~unordered_map() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    unordered_map(const unordered_map&) = delete;
    unordered_map& operator=(const unordered_map&) = delete;

    struct Iterator {
        Node* current_node;

        Iterator(Node* n) {
            current_node = n;
        }

        bool operator!=(const Iterator& other) const { return current_node != other.current_node; }
        bool operator==(const Iterator& other) const { return current_node == other.current_node; }

        Node* operator->() {
            return current_node;
        }
    };

    Iterator end() {
        return Iterator(nullptr);
    }

    Iterator find(const K& key) {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->first == key) {
                return Iterator(current);
            }
            current = current->next;
        }
        return end();
    }

    V& operator[](const K& key) {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->first == key) {
                return current->second;
            }
            current = current->next;
        }
        Node* new_node = new Node(key);
        new_node->next = table[index];
        table[index] = new_node;

        return table[index]->second;
    }

    void erase(const K& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->first == key) {
                if (previous == nullptr) {
                    table[index] = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

    void erase(Iterator it) {
        if (it.current_node != nullptr) {
            erase(it.current_node->first);
        }
    }
};
