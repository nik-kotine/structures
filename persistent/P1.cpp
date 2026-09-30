#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int SZ = 2;
const int MAX_NODES = 10'000'000;
const int EMPTY = -1;
const int N_BYTES = 15;

int h(char c) { return c - '0'; }

struct PersistentTrie {

    int roots[MAX_NODES];
    int next[MAX_NODES][SZ];
	int freq[MAX_NODES];
    bool eow[MAX_NODES];

    int n_nodes = 1;
    int n_vers = 0;

    PersistentTrie() {

        fill_n(roots, MAX_NODES, EMPTY);
        roots[0] = 0;

		for (size_t i = 0; i < MAX_NODES; ++i)
			fill_n(next[i], SZ, EMPTY);

		fill_n(freq, MAX_NODES, 0);
        fill_n(eow, MAX_NODES, false);
    }

	void clear() {
		
		n_nodes = 1;
		n_vers = 0;
		roots[0] = 0;

	}

	int child(int u, int x) {

		if (u == EMPTY)
			return EMPTY;

		return next[u][x];

	}

	int clone(int u) {

    	int v = n_nodes++;

	    if (u == EMPTY) {
    	    for (int i = 0; i < SZ; ++i)
        	    next[v][i] = EMPTY;

			freq[v] = 0;
	        eow[v] = false;

	    } else {

    	    for (int i = 0; i < SZ; ++i)
        	    next[v][i] = next[u][i];

			freq[v] = freq[u];
	        eow[v] = eow[u];
    	}

	    return v;
	}

    void add(string s) {

        int old = roots[n_vers];
        int new_node = clone(old);
        roots[++n_vers] = new_node;
        int u = new_node;

        for (char c : s) {
            int x = h(c);
            int v = clone(next[u][x]);
            next[u][x] = v;
            u = v;
			++freq[u];
        }

        eow[u] = true;
    }

	void insert_binary(int num) {

        int old = roots[n_vers];
        int new_node = clone(old);
        roots[++n_vers] = new_node;
        int u = new_node;

		for (int i = N_BYTES - 1; i >= 0; --i) {
			int x = ((num >> i) & 1);
			int v = clone(next[u][x]);
			next[u][x] = v;
			u = v;
			++freq[u];
		}

        eow[u] = true;
    }

    void collect(int u, string& curr, vector<string>& res) {

        if (eow[u])
            res.push_back(curr);

        for (int i = 0; i < SZ; ++i) {

            if (next[u][i] == EMPTY)
                continue;

            curr.push_back('0' + i);
            collect(next[u][i], curr, res);
            curr.pop_back();
        }
    }

    vector<string> prefix(int ver, string prefix) {

        vector<string> res;
        int u = roots[ver];

        for (char c : prefix) {
            u = next[u][h(c)];
            if (u == EMPTY)
                return res;
        }

        string curr = prefix;
        collect(u, curr, res);

        return res;
    }

	int max_xor(int a, int l, int r) {

    	int res = 0;
	    int left = roots[l - 1];
    	int right = roots[r];

	    for (int i = N_BYTES - 1; i >= 0; --i) {

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

};

PersistentTrie trie;

void solve() {

	trie.clear();

	int n, q; cin >> n >> q;
	
	for (int i = 0; i < n; ++i) {
		int x; cin >> x;
		trie.insert_binary(x);
	}

	while (q--) {
		int a, l, r; cin >> a >> l >> r;
		cout << trie.max_xor(a, l, r) << '\n';
	}

}

int main() {
	
	cin.tie(0) -> sync_with_stdio(false);
	int t; cin >> t;
	while (t--)	solve();

}
