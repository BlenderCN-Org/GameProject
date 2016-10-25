#include "OBJLoader.hpp"

#include <vector>

#include <fstream>
#include <string>
#include <sstream>

struct Vert_t
{
	float x, y, z;
	float u, v;
};

void* ObjLoader::load(const char* fileName, unsigned int &dataSize) {

	std::ifstream inFile(fileName);
	
	std::vector<Vert_t> vertex;
	std::vector<Vert_t> vertex_ret;

	std::string line;

	int count = 0;
	bool ok = true;

	int state = 1;

	while ( ok ) {

		std::getline(inFile, line);
		ok = !inFile.eof();

		if ( !ok )
			break;
		
		if ( line[0] == 'v' && line[1] == ' ' ) // vertex pos
		{
			if ( state == 1 ) {
				state = 0;
				//vertex.clear();
				//count = 0;
			}
			vertex.push_back(Vert_t());

			std::istringstream iss(line);
			std::string sub;
			iss >> sub; // discard 'v'
			iss >> sub;
			vertex[count].x = std::stof(sub);
			iss >> sub;
			vertex[count].y = std::stof(sub);
			iss >> sub;
			vertex[count].z = std::stof(sub);
			count++;
		} else if ( line[0] == 'f' ) {
			if ( state == 0 ) {
				state = 1;
			}
			std::istringstream iss(line);
			std::string sub;

			iss >> sub; // discard 'v'

			int v = 0;
			int n = 0;
			int t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex[v].u = 0;
			vertex[v].v = 0;
			vertex_ret.push_back(vertex[v]);

			v = 0;
			n = 0;
			t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex[v].u = 0;
			vertex[v].v = 1;
			vertex_ret.push_back(vertex[v]);

			v = 0;
			n = 0;
			t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex[v].u = 1;
			vertex[v].v = 0;
			vertex_ret.push_back(vertex[v]);

		}

	}

	uint64_t size = vertex_ret.size();

	Vert_t* v = new Vert_t[size];
	dataSize = sizeof(Vert_t) * (unsigned int)size;

	memcpy(v, (char*)&vertex_ret[0], dataSize);

	return (void*)v;

}

/*void* loadObj(const char* fileName, unsigned int &dataSize) {
	std::ifstream inFile(fileName);

	std::vector<Vert_t> vertex;

	std::vector<Vert_t> vertex_ret;

	std::string line;
	int count = 0;
	bool ok = true;

	int state = 1;

	while ( ok ) {
		std::getline(inFile, line);
		ok = !inFile.eof();
		if ( !ok ) {
			break;
		}

		if ( line[0] == 'v' && line[1] == ' ' ) // vertex pos
		{
			if ( state == 1 ) {
				state = 0;
				//vertex.clear();
				//count = 0;
			}
			vertex.push_back(Vert_t());

			std::istringstream iss(line);
			std::string sub;
			iss >> sub; // discard 'v'
			iss >> sub;
			vertex[count].x = std::stof(sub);
			iss >> sub;
			vertex[count].y = std::stof(sub);
			iss >> sub;
			vertex[count].z = std::stof(sub);
			count++;
		} else if ( line[0] == 'f' ) {
			if ( state == 0 ) {
				state = 1;
			}
			std::istringstream iss(line);
			std::string sub;

			iss >> sub; // discard 'v'

			int v = 0;
			int n = 0;
			int t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex_ret.push_back(vertex[v]);

			v = 0;
			n = 0;
			t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex_ret.push_back(vertex[v]);

			v = 0;
			n = 0;
			t = 0;
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				v = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, '/');
			if ( sub.size() ) {
				t = std::stoi(sub) - 1;
				sub = "";
			}
			std::getline(iss, sub, ' ');
			if ( sub.size() ) {
				n = std::stoi(sub) - 1;
				sub = "";
			}

			vertex_ret.push_back(vertex[v]);

		}

	}

	Vert_t* v = new Vert_t[vertex_ret.size()];
	dataSize = sizeof(Vert_t) * (unsigned int)vertex_ret.size();

	memcpy(v, (char*)&vertex_ret[0], dataSize);

	return (void*)v;
*/