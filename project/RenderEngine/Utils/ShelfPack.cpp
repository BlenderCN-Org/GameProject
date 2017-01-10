#include "ShelfPack.hpp"

#include "FrameAllocator_static.hpp"

ShelfPack::ShelfPack(int x, int y) {
	width = x;
	height = y;
	root = nullptr;
	counter = 0;
}

bool ShelfPack::addData(int w, int h, int id) {
	FrameAllocator* allocator = FrameAllocator_static::getFrameAllocator();

	Node* n = allocator->allocate<Node>(1);

	n->x = n->y = 0;
	n->height = h;
	n->width = w;
	n->id = id;
	n->below = n->side = nullptr;

	bool added = addNode(n);

	if ( added ) {
		counter++;
		nodes.push_back(n);
	}

	return added;
}

size_t ShelfPack::length() {
	return counter;
}

int ShelfPack::getPackedWidth() {
	if ( root )
		return root->getWidth();
	return 0;
}

int ShelfPack::getPackedHeight() {
	if ( root )
		return root->getHeight();
	return 0;
}

PackRect & ShelfPack::operator[](int id) {
	// TODO: insert return statement here
	FrameAllocator* allocator = FrameAllocator_static::getFrameAllocator();

	PackRect* r = allocator->allocate<PackRect>(1);

	Node* n = nodes[id];

	r->h = n->height;
	r->w = n->width;
	r->id = n->id;
	r->x = n->x;
	r->y = n->y;

	return *r;
}

bool ShelfPack::addNode(Node * n) {
	bool ret = false;
	if ( root ) {
		ret = root->insert(n, width, height);
	} else {
		root = n;
		ret = true;
	}
	return ret;
}

bool ShelfPack::Node::insert(Node * n, int maxX, int maxY) {
	// if we inserted
	bool inserted = false;

	// space avaible to the side of me
	int wLeft = maxX - width;
	// space avaible below me
	int hLeft = maxY - height;

	if ( side ) {
		// I can only tell the node next to me how much I know is left, and how tall I am, this limits the rect space
		inserted = side->insert(n, wLeft, height);
	} else {
		// nothing was left of us, so check if we can place the character next to us, they cannot be larger since the input is sorted largest first but in case a check is done
		if ( (n->width <= wLeft) && (n->height <= height) ) {
			inserted = true;
			side = n;
			side->x = x + width; // offset in x
			side->y = y; // we are on the same y offset
		}
	}
	//could not place node next to us so we try below
	if ( !inserted )
		if ( below ) {
			// the node below us start at the same with that I had, but the height it can place on is what I know is left
			inserted = below->insert(n, maxX, hLeft);
		} else {
			// make sure the height is enough
			if ( (n->width <= wLeft) && (n->height <= hLeft) ) {
				inserted = true;
				below = n;
				below->x = x;
				below->y = y + height;
			}
		}

	return inserted;
}

int ShelfPack::Node::getHeight() {
	if ( below )
		return below->getHeight();
	return y + height;
}

int ShelfPack::Node::getWidth() {
	int ws = 0;
	int wb = 0;
	if ( side )
		ws = side->getWidth();
	if ( below )
		wb = below->getWidth();

	if ( ws > wb )
		return ws;
	else if ( wb > ws )
		return wb;
	else if ( ws == wb && ws != 0 )
		return ws;
	else
		return x + width;
}