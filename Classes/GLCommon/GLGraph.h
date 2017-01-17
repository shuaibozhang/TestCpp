#ifndef __GLGRAPH_H__
#define __GLGRAPH_H__

#include "cocos2d.h"
#include "Defines.h"
USING_NS_CC;

class Neighbor
{
public:
	Neighbor(int dest) { destIdx = dest; }
	int destIdx;
};


class VerTexInfo
{
public:
	VerTexInfo() { haveVisited = false;  parNodeIdx = -1; islocked = false; }
	std::vector<Neighbor> edges;
	bool haveVisited;
	int parNodeIdx;
	bool islocked;
};


class GLGraph
{
public:
	GLGraph();
	~GLGraph();

	void addNode(Node* node);
#if (1 == CC_ENABLE_NEW_WORLD)
	//20161023 J
	void addNewNode(Node* node);
	void addNewEdges(Node* lhp, Node* rhp);
#endif
	void addEage(Node* lhp, Node* rhp);
	void removeEage(Node* lhp, Node* rhp);

	void getSeatchPath(Node* startNode, Node* endNode, std::vector<Node*>& vectorNode);
	
	//std::vector<Node*> _searchVector;
	std::vector<Node*> _nodeVector;
	std::vector<VerTexInfo> _vertexInfoVector;

#if (1 == CC_ENABLE_NEW_WORLD)
	//20161023 J
	std::vector<Node*> _newNodeVector;
	std::vector<VerTexInfo> _newVertexInfoVector;
#endif

	int numeages;
	int numVertex;
};
#endif
