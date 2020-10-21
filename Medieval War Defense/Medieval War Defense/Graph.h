#pragma once

#include <vector>
#include <memory>
#include <ctime>

class Node {
	public:
        float position[3];
		std::vector<std::shared_ptr<Node>> vertices;
		Node ();
		std::shared_ptr<Node> NextTarget();
};

class Graph {
	private:
		std::shared_ptr<Node> mapPath;
	public:
		Graph ();
		std::shared_ptr<Node> genRandInit ();
		void loadMap1();
};
