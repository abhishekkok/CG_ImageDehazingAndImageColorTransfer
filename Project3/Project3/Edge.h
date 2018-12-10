#pragma once
#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

/// an edge in a graph
typedef struct
{
	float w;
	int a;
	int b;
} edge;

//std::ostream& operator<<( std::ostream& ostr, const edge& e );
bool operator<(const edge &a, const edge &b);

#endif