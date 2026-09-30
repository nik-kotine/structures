#include <iostream>
#include <vector>
#define ll long long
#define ptt pair<size_t, size_t>
using namespace std;

template <typename X>
struct Node {

	size_t l, r;
	X value;
	Node* left = nullptr;
	Node* right = nullptr;
	bool can_update = false;

public:

	Node(size_t _l, size_t _r) :
		l(_l),
		r(_r)
	{}

	~Node() = default;

};

template <typename X>
class PersistentSegmentTree {

public:

	vector<Node<X>*> version_roots;
	size_t n_vers = 0;

private:

	Node<X>* _recursive_construct(const vector<X>& arr, size_t l, size_t r) {

		Node<X>* node = new Node<X>(l, r);

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

	Node<X>* _recursive_update(Node<X>* node, size_t i, X value, bool p) {

		Node<X>* new_node = new Node<X>(node->l, node->r);

		if (node->l == i && node->r == i) {
			if (p == true) {
				cout << node->value << '\n';
			}
			new_node->value = value;
			return new_node;
		}

		size_t q = (node->l + node->r)/2;

		if (i <= q) {
			new_node->left = _recursive_update(node->left, i, value, p);
			new_node->right = node->right;
		} else {
			new_node->left = node->left;
			new_node->right = _recursive_update(node->right, i, value, p);
		}

		new_node->value = new_node->left->value + new_node->right->value;

		return new_node;
			
	}

public:

	void construct(const vector<X>& arr) {

		if (arr.empty())
			return;

		Node<X>* root = _recursive_construct(arr, 0, arr.size()-1);
		version_roots.push_back(root);

	}

	void print(size_t ver) {
		_recursive_print(version_roots[ver]);
	}

	X range_sum(size_t ver, size_t l, size_t r) {
		return _recursive_range_sum(version_roots[ver], l, r);
	}

	void update(size_t ver, size_t i, X value, bool p = false) {
		Node<X>* new_root = _recursive_update(version_roots[ver], i, value, p);
		version_roots.push_back(new_root);
		++n_vers;
	}

	void range_add(size_t ver, size_t l, size_t r, X add) {
		Node<X>* new_root = _recursive_range_add(version_roots[ver], l, r, add);
		version_roots.push_back(new_root);
		++n_vers;
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
