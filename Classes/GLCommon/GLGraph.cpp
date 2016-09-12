#include "GLGraph.h"

GLGraph::GLGraph():numeages(0),
	numVertex(0)
{

}


GLGraph::~GLGraph()
{

}


void GLGraph::addNode(Node* node)
{
	_nodeVector.push_back(node);
	_vertexInfoVector.push_back(VerTexInfo());

	numeages++;
	numVertex++;
}


void GLGraph::addEage(Node* lhp, Node* rhp)
{
	int idxl = -1;
	int idxr = -1;
	bool left = false;
	bool right = false;
	for (auto temp : _nodeVector)
	{
		if (!left)
		{
			idxl += 1;
			if (temp->getName().compare(lhp->getName()) == 0)
			{
				left = true;
			}
		}
	
		if (!right)
		{
			idxr += 1;
			if (temp->getName().compare(rhp->getName()) == 0)
			{
				right = true;
			}
		}	

		if (left && right)
		{
			break;
		}
	}
	_vertexInfoVector[idxl].edges.push_back(Neighbor(idxr));
	_vertexInfoVector[idxr].edges.push_back(Neighbor(idxl));
}


void GLGraph::removeEage(Node* lhp, Node* rhp)
{

}

void GLGraph::getSeatchPath(Node* startNode, Node* endNode, std::vector<Node*>& vectorNode)
{
	vectorNode.clear();

	int idxl = -1;
	int idxr = -1;
	bool left = false;
	bool right = false;

	for (auto temp : _nodeVector)
	{
		if (!left)
		{
			idxl += 1;
			if (temp->getName().compare(startNode->getName()) == 0)
			{
				left = true;
			}
		}

		if (!right)
		{
			idxr += 1;
			if (temp->getName().compare(endNode->getName()) == 0)
			{
				right = true;
			}
		}

		if (left && right)
		{
			break;
		}
	}


	std::queue<int> idxqueue;
	idxqueue.push(idxl);
	bool find = false;

	while (!idxqueue.empty() && find == false)
	{
		int curidx = idxqueue.front();

		idxqueue.pop();
		if (_nodeVector[curidx]->getName().compare(endNode->getName()) == 0)
		{
			find = true;
		}
		else
		{
			for (auto& temp : _vertexInfoVector[curidx].edges)
			{
				if (_vertexInfoVector[temp.destIdx].haveVisited == false && _vertexInfoVector[temp.destIdx].islocked == false)
				{
					if (_vertexInfoVector[temp.destIdx].parNodeIdx == -1)
					{
						_vertexInfoVector[temp.destIdx].parNodeIdx = curidx;
					}
					
					idxqueue.push(temp.destIdx);
				}		
			}
		}

		_vertexInfoVector[curidx].haveVisited = true;
	}
	//std::vector<Node*>  vectorNode;
	vectorNode.push_back(endNode);
	int curPIdx = _vertexInfoVector[idxr].parNodeIdx;
	while (find && curPIdx != idxl)
	{	
		vectorNode.push_back(_nodeVector[curPIdx]);
		curPIdx = _vertexInfoVector[curPIdx].parNodeIdx;
	}
	vectorNode.push_back(startNode);

	for (auto& temp : _vertexInfoVector)
	{
		temp.haveVisited = false;
		temp.parNodeIdx = -1;
	}

	//return vectorNode;
}