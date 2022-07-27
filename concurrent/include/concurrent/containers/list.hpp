#include <iostream>
#include <thread>
#include <atomic>

#include <concurrent/mutex.hpp>

namespace il {

template <typename T>
class Forward_List {
class Node {
public:
    Node(): value{} {
        next = nullptr;
    }
    Node(const T& m_value) {
        value = m_value;
    }
    Node(Node&& obj) {
        value = obj.value;
        next = obj.next;
    }
    Node(const Node& other): value{other.value}, next{other.next} {}
public:
    T value;
    Node* next;
};
private:
    Node* head;
    mutex m;
public:
    Forward_List(): head{} {}
    Forward_List(int count) {
        lock_guard<mutex> lock(m);
        head = new Node();
        auto curr = head;
        for(int i = 0; i < count - 1; ++i) {
            curr->next = new Node();
            curr = curr->next;
        }
    }
    Forward_List(int count, const T&& value) {
        lock_guard<mutex> lock(m);
        head = new Node(value);
        auto curr = head;
        for(int i = 0; i < count - 1; ++i) {
            curr->next = new Node(value);
            curr = curr->next;
        }
    }
    Forward_List(const Forward_List<T>& obj) {
        lock_guard<mutex> lock(m);
        head = new Node(obj.head->value); 
        Node* curr = head;
        Node* curr1 = obj.head->next;
        while(curr1 != nullptr) {
            curr->next = new Node(curr1->value);
            curr = curr->next;
            curr1 = curr1->next;
        }
    }
    Forward_List(Forward_List&& obj) {
        lock_guard<mutex> lock(m);
        head = obj.head;
        obj.head = nullptr;
    }
    ~Forward_List() {
        delete head;
        head = nullptr;
    }
    bool empty() const {
        return head == nullptr;
    }
    void print() {
        lock_guard<mutex> lock(m);
        auto curr = head;
        int i = 0;
        while(curr) {
            std::cout << i << ": -->" << curr->value << std::endl;
            curr = curr->next; 
        }
    }
    void clear() {
        lock_guard<mutex> lock(m);
        delete head;
        head = nullptr;
    }
    void insert(const T&& value, int pos) {
        lock_guard<mutex> lock(m);
        auto curr = head;
        int i = 0;
        while(pos != i && curr != nullptr) {
            ++i;
            curr = curr->next;
        }
        Node* tmp = curr->next;
        tmp->value = value;
        curr->next = tmp;
        tmp = nullptr;
    }
    void erase_after(int pos) {
        lock_guard<mutex> lock(m);
        auto curr = head;
        int i = 0;;
        while(i != pos && curr) {
            curr = curr->next;
            ++i;
        }
        if(curr->next) {
            Node* tmp = curr->next;
            curr->next = curr->next->next;
            tmp = nullptr;
        }
    } 
    void push_front(T value) {
        lock_guard<mutex> lock(m);
        Node* tmp = new Node(value);
        tmp->next = head;
        head = tmp;
        tmp = nullptr;
    }
    void pop_front() {
        lock_guard<mutex> lock(m);
        if(head != nullptr){
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
    void unique() {
        lock_guard<mutex> lock(m);
        auto curr = head;
        while(curr->next != nullptr) {
            if(curr->value == curr->next->value) {
                auto tmp = curr->next;
                curr->next = curr->next->next;
                delete tmp;
            }
            else {
                curr = curr->next;
            }
        }    
    }
    void sort() {
        lock_guard<mutex> lock(m);
        auto curr = head;
        int i = 0;
        while(curr != nullptr) {
            ++i;
            curr = curr->next;
        }
        curr = head;
        for(int j = i - 1 ; j > 0; j--) {
            curr = head;
            for(int k = 0; k < j; k++) {
                if(curr->value > curr->next->value){
                    T tmp = curr->value;
                    curr->value = curr->next->value;
                    curr->next->value = tmp;
                }
                curr = curr->next;
            }
        }
    }
};
}

                  // -----Testing----- //
// void foo(int a) {
//     std::cout << a;
// }

// int main() {
//     Forward_List<int> f;
//     std::thread th_1 {
//         [&] () {
//             f.push_front(8);
//         }
//     };
//     std::thread th_2 {
//         [&] () {
//             f.push_front(8);
//         }
//     };
// }