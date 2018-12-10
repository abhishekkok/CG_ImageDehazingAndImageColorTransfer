#include "Edge.h"
/**
 * compare edge weights, needed in STL - sort,
 * edges are sorted according to weights
 */
bool operator<(const edge &a, const edge &b)
{
	return a.w < b.w;
}