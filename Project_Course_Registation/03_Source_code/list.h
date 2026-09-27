#pragma once
#include <string>

using namespace std;

struct Node {
    string data;
    Node* prev;
    Node* next;
    Node(string d = "") : data(d), prev(nullptr), next(nullptr) {}
};

struct list {
    Node* dummy;
    int sz;

    list() : sz(0) {
        dummy = new Node();
        dummy->next = dummy;
        dummy->prev = dummy;
    }

    ~list() {
        while (!empty()) pop_front();
        delete dummy;
    }

    bool empty() {
        return sz == 0;
    }

    int size() {
        return sz;
    }

    Node* push_back(const string& val) {
        Node* n = new Node(val);
        n->prev = dummy->prev;
        n->next = dummy;
        dummy->prev->next = n;
        dummy->prev = n;
        sz++;
        return n;
    }

    string& front() {
        return dummy->next->data;
    }

    void pop_front() {
        if (empty()) return;
        Node* cur = dummy->next;
        dummy->next = cur->next;
        cur->next->prev = dummy;
        delete cur;
        sz--;
    }

    void erase(Node* node) {
        if (!node || node == dummy) return;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        sz--;
    }
};