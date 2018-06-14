#ifndef DATA_TAGS_HPP
#define DATA_TAGS_HPP

struct Tag {
	char tag[4];

	bool operator==(const Tag& right) {
		if (tag[0] == right.tag[0] &&
			tag[1] == right.tag[1] &&
			tag[2] == right.tag[2] &&
			tag[3] == right.tag[3]) {
			return true;
		}
		return false;
	}
};
/// Constants
const Tag NULL_TAG { 'N', 'U', 'L', 'L' };
const Tag MAP_TAG { 'M', 'A', 'P', ' ' };
const Tag SKY_TAG { 'S', 'K', 'Y', ' ' };
const Tag CELL_TAG { 'C', 'E', 'L', 'L' };
const Tag OBJ_TAG { 'O', 'B', 'J', ' ' };
const Tag MESH_TAG { 'M', 'E', 'S', 'H' };

#endif