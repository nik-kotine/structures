#include <iostream>
#include <vector>
#define ll long long
using namespace std;

struct Node {

	Node* next = nullptr;
	ll value = 0;

	Node(ll _value) :
		value(_value)
	{}

};

class TotallyPersistentStack {

	vector<Node*> versions;
	int n_vers = 0;
	ll sum = 0;

public:

	TotallyPersistentStack() {
		versions.push_back(nullptr);
	}

	void insert(size_t ver, ll _value) {
		ll old_mass = (versions[ver] == nullptr) ? 0 : versions[ver]->value;
		ll new_mass = old_mass + _value;
		Node* new_top = new Node(new_mass);
		sum += new_mass;
		new_top->next = versions[ver];
		versions.push_back(new_top);
		++n_vers;
	}

	void pop(size_t ver) {
		Node* old_top = versions[ver];
		versions.push_back(old_top->next);
		++n_vers;
		ll new_mass =
			(versions[n_vers] == nullptr) ? 0 : versions[n_vers]->value;
		sum += new_mass;
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

	ll get_sum() {
		return sum;
	}
	
};

void solve() {
	TotallyPersistentStack s;
	int n; cin >> n;
	while (n--) {
		int t, m; cin >> t >> m;
		if (m == 0) {
			s.pop(t);
		} else {
			s.insert(t, m);
		}
	}
	cout << s.get_sum();
}

int main() {
	solve();
}

