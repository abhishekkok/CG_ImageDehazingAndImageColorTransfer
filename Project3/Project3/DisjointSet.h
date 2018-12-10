#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include<vector>

typedef struct {
	int parent;
	int rank;
	int size;
} node;

typedef std::vector <node> Forest;


class DisjointSet {
public:
	DisjointSet();

	DisjointSet(int num_elements);

	int find(int x);

	void merge(int x, int y);

	int size(int x) const;

private:
	Forest elements;
};

#endif // DISJOINTSET_H