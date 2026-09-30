#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int N_BITS = 30;
const int SZ = 2;
const int MAX_VERS = 2*1e5+1;
const int MAX_NODES = (2*1e5) * (N_BITS + 1) + 5;
const int EMPTY = -1;

int h(char c) { return c - '0'; }

struct PersistentTrie {

    int roots[MAX_VERS];
    int next[MAX_NODES][SZ];
	int freq[MAX_NODES];

    int n_nodes = 1;
    int n_vers = 0;

    PersistentTrie() {

        fill_n(roots, MAX_VERS, EMPTY);
        roots[0] = 0;

		for (size_t i = 0; i < MAX_NODES; ++i)
			fill_n(next[i], SZ, EMPTY);

		fill_n(freq, MAX_NODES, 0);
    }

	// works for any
	int child(int u, int x) {

		if (u == EMPTY)
			return EMPTY;

		return next[u][x];

	}

	// works for any
	int clone(int u) {

    	int v = n_nodes++;

	    if (u == EMPTY) {
    	    for (int i = 0; i < SZ; ++i)
        	    next[v][i] = EMPTY;

			freq[v] = 0;

	    } else {

    	    for (int i = 0; i < SZ; ++i)
        	    next[v][i] = next[u][i];

			freq[v] = freq[u];
    	}

	    return v;
	}

	// works for any
	int add(int ver, string s) {

		int old = roots[ver];
		int new_node = clone(old);
    	int new_ver = ++n_vers;
 		roots[new_ver] = new_node;

    	int u = new_node;

    	for (char c : s) {
        	int x = h(c);
        	int v = clone(next[u][x]);
        	next[u][x] = v;
        	u = v;
        	++freq[u];
    	}

    	return new_ver;
	}

	// only for bitwise operations, set SZ = 
	void insert_binary(int ver, int num) {

        int old = roots[ver];
        int new_node = clone(old);
		int new_ver = ++n_vers;
        roots[new_ver] = new_node;

        int u = new_node;

		for (int i = N_BITS - 1; i >= 0; --i) {
			int x = ((num >> i) & 1);
			int v = clone(next[u][x]);
			next[u][x] = v;
			u = v;
			++freq[u];
		}

    }

	// works for binary trie, set SZ = 2 and use binary insert only
	int max_xor(int a, int l, int r) {

    	int res = 0;
	    int left = roots[l - 1];
    	int right = roots[r];

	    for (int i = N_BITS - 1; i >= 0; --i) {

    	    int x = (a >> i) & 1;
        	int want = x ^ 1;

	        int left_child = child(left, want);
    	    int right_child = child(right, want);

			int left_count = (left_child == EMPTY ? 0 : freq[left_child]);
			int right_count = (right_child == EMPTY ? 0 : freq[right_child]);

	        if (right_count - left_count > 0) {
    	        res |= (1 << i);
        	    left = left_child;
    	        right = right_child;
        	} else {
            	left = child(left, x);
	            right = child(right, x);
    	    }
	    }

    	return res;

	}

	// works for binary trie, set SZ = 2 and use binary insert only
	int kth_xor(int a, int l, int r, int k) {

    	int res = 0;
	    int left = roots[l - 1];
    	int right = roots[r];

	    for (int i = N_BITS - 1; i >= 0; --i) {

    	    int x = (a >> i) & 1;
        	int want = x ^ 1;

	        int left_child = child(left, want);
    	    int right_child = child(right, want);

			int left_count = (left_child == EMPTY ? 0 : freq[left_child]);
			int right_count = (right_child == EMPTY ? 0 : freq[right_child]);

	        if (right_count - left_count >= k) {
    	        res |= (1 << i);
        	    left = left_child;
    	        right = right_child;
        	} else {
				k = k - right_count + left_count;
            	left = child(left, x);
	            right = child(right, x);
    	    }
	    }

    	return res;

	}

};

PersistentTrie trie;

int main() {

	cin.tie(0) -> sync_with_stdio(false);

	int n; cin >> n;
	for (int i = 0; i < n; ++i) {
		int x; cin >> x;
		trie.insert_binary(i, x);
	}

	int q; cin >> q;
	while (q--) {
		int l, r, x, k;
		cin >> l >> r >> x >> k;
		cout << trie.kth_xor(x, l, r, (r-l+1)-k+1) << '\n';
	}


}
