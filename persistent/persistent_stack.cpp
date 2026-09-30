#include <iostream>
#include <vector>
using namespace std;

struct Node {

	Node* next = nullptr;
	int value = 0;

	Node(int _value) :
		value(_value)
	{}

};

class PartiallyPersistentStack {

	vector<Node*> versions;
	int n_vers = 0;

public:

	PartiallyPersistentStack() {
		versions.push_back(nullptr);
	}

	void insert(int _value) {
		Node* new_top = new Node(_value);
		new_top->next = versions[n_vers++];
		versions.push_back(new_top);
	}

	int pop() {
		Node* old_top = versions[n_vers++];
		versions.push_back(old_top->next);
		return versions[n_vers]->value;
	}

	int top(size_t ver) {
		return versions[ver]->value;
	}

	void print_stack(size_t ver) {
		Node* curr = versions[ver];
		while (curr != nullptr) {
			cout << curr->value << '\n';
			curr = curr->next;
		}
	}
	
};

class TotallyPersistentStack {

	vector<Node*> versions;
	int n_vers = 0;

public:

	TotallyPersistentStack() {
		versions.push_back(nullptr);
	}

	void insert(size_t ver, int _value) {
		Node* new_top = new Node(_value);
		new_top->next = versions[ver];
		versions.push_back(new_top);
		++n_vers;
	}

	int pop(size_t ver) {
		Node* old_top = versions[ver];
		versions.push_back(old_top->next);
		return versions[++n_vers]->value;
	}

	int top(size_t ver) {
		return versions[ver]->value;
	}

	void print_stack(size_t ver) {
		Node* curr = versions[ver];
		while (curr != nullptr) {
			cout << curr->value << '\n';
			curr = curr->next;
		}
	}
	
};

int main() {
	TotallyPersistentStack s;

	s.insert(0, 10);
	s.insert(1, 20);
	s.insert(2, 30);
	s.pop(2);
	s.insert(4, 40);
	s.pop(3);
	s.insert(1, 50);

	s.print_stack(1);
	cout << '\n';
	s.print_stack(2);
	cout << '\n';
	s.print_stack(3);
	cout << '\n';
	s.print_stack(4);
	cout << '\n';
	s.print_stack(5);
	cout << '\n';
	s.print_stack(6);
	cout << '\n';
	s.print_stack(7);
	cout << '\n';

}
