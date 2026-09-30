#include <iostream>
#include <vector>
#define ll long long
#define ptt pair<size_t, size_t>
using namespace std;

template <typename X>
struct Node {

	X value;
	Node* left = nullptr;
	Node* right = nullptr;

};

template <typename X>
class PersistentSegmentTree {

public:

	vector<Node<X>*> version_roots;
	size_t sz;

private:

	Node<X>* _recursive_construct(const vector<X>& arr, size_t l, size_t r) {

		Node<X>* node = new Node<X>();

		if (l == r) {
			node->value = arr[l];
			return node;
		}

		size_t q = (l+r)/2;
		Node<X>* left = _recursive_construct(arr, l, q);
		Node<X>* right = _recursive_construct(arr, q+1, r);

		node->left = left;
		node->right = right;
		node->value = left->value + right->value;
		return node;

	}

	void _recursive_print(Node<X>* node, size_t indent = 0) {
		
		if (node == nullptr)
			return;

		string temp (2*indent, ' ');
		cout << temp << node->value << '(' << node->lazy << ')' << '\n';

		_recursive_print(node->left, indent + 1);
		_recursive_print(node->right, indent + 1);

	}

	void _recursive_delete(Node<X>* node) {

		if (node == nullptr)
			return;
		
		_recursive_delete(node->left);
		_recursive_delete(node->right);

		delete node;

	}

	Node<X>* _recursive_update(
		Node<X>* node, size_t l, size_t r, size_t i, X value, bool p
	) {

		Node<X>* new_node = new Node<X>();

		if (l == r) {
			if (p == true) {
				cout << node->value << '\n';
			}
			new_node->value = value;
			return new_node;
		}

		size_t q = (l+r)/2;

		if (i <= q) {
			new_node->left = _recursive_update(
				node->left, l, q, i, value, p
			);
			new_node->right = node->right;
		} else {
			new_node->left = node->left;
			new_node->right = _recursive_update(
				node->right, q+1, r, i, value, p
			);
		}

		new_node->value = new_node->left->value + new_node->right->value;

		return new_node;
			
	}

public:

	void construct(const vector<X>& arr) {

		if (arr.empty())
			return;

		Node<X>* root = _recursive_construct(arr, 0, arr.size()-1);
		sz = arr.size();
		version_roots.push_back(root);

	}

	void print(size_t ver) {
		_recursive_print(version_roots[ver]);
	}

	void update(size_t ver, size_t i, X value, bool p = false) {
		Node<X>* new_root = _recursive_update(
			version_roots[ver], 0, sz-1, i, value, p
		);
		version_roots.push_back(new_root);
	}
	
};

template <typename X>
struct PersistentQueue {

	PersistentSegmentTree<X> seg_tree;
	vector<ptt> ht;

	PersistentQueue(size_t n) {
		vector<X> temp(n, X());
		seg_tree.construct(temp);
		ht.push_back({-1, -1});
	}

	void push(size_t ver, X value) {

		seg_tree.update(ver, ht[ver].second + 1, value);
		size_t new_first = (ht[ver].first == -1) ? 0 : ht[ver].first;
		ht.push_back({ new_first, ht[ver].second + 1 });

	}

	void pop(size_t ver) {

		seg_tree.update(ver, ht[ver].first, X(), true);
		if (ht[ver].first == ht[ver].second) {
			ht.push_back({ -1, -1 });
			return;
		}
		ht.push_back({ ht[ver].first + 1, ht[ver].second });

	}

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
	size_t n; cin >> n;
	PersistentQueue<ll> q(n);
	while (n--){
		
		int op; cin >> op;

		if (op == 1) {
			size_t i; cin >> i;
			ll x; cin >> x;
			q.push(i, x);
		}
		else {
			size_t i; cin >> i;
			q.pop(i);
		}
	}
}
