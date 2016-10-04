#ifndef SHELFPACK_HPP
#define SHELFPACK_HPP

#include <vector>

struct PackRect {
	int x, y;
	int w, h;

	int id;
};

class ShelfPack {

public:

	ShelfPack(int x, int y);

	bool addData(int w, int h, int id);

	size_t length();

	int getPackedWidth();
	int getPackedHeight();

	PackRect &operator[](int id);

private:
	
	struct Node {
		int x;
		int y;
		int height;
		int width;

		int id;

		Node* side;
		Node* below;

		bool insert(Node* n, int maxX, int maxY);

		int getHeight();
		int getWidth();
	};


	bool addNode(Node* n);

	int width;
	int height;

	size_t counter;

	Node* root;

	std::vector<Node*> nodes;
};

#endif