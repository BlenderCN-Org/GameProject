#include <AssetLib/AssetLib.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

namespace AssetLib {
	struct Vert_t {
		float x, y, z;
		float u, v;
	};

	void * loadWavefrontOBJ(const char * fileName, uint32_t &dataSize) {
		std::ifstream inFile(fileName);

		std::vector<Vert_t> vertex;
		std::vector<Vert_t> uv;
		std::vector<Vert_t> vertex_ret;

		std::string line;

		int count = 0;
		int uvCount = 0;
		bool ok = true;
		bool useuv = false;

		int state = 1;

		if (!inFile.is_open()) {
			dataSize = 0;
			return nullptr;
		}

		while (ok) {
			std::getline(inFile, line);
			ok = !inFile.eof();

			if (!ok)
				break;

			if (line[0] == 'v' && line[1] == ' ') // vertex pos
			{
				if (state == 1) {
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
			}
			if (line[0] == 'v' && line[1] == 't') // vertex pos
			{
				useuv = true;
				uv.push_back(Vert_t());

				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'v'
				iss >> sub;
				uv[uvCount].u = std::stof(sub);
				iss >> sub;
				uv[uvCount].v = std::stof(sub);
				uvCount++;
			} else if (line[0] == 'f') {
				if (state == 0) {
					state = 1;
				}
				std::istringstream iss(line);
				std::string sub;

				iss >> sub; // discard 'v'

				int v = 0;
				int n = 0;
				int t = 0;
				std::getline(iss, sub, '/');
				if (sub.size()) {
					v = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, '/');
				if (sub.size()) {
					t = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, ' ');
				if (sub.size()) {
					n = std::stoi(sub) - 1;
					sub = "";
				}
				if (useuv) {
					vertex[v].u = uv[t].u;
					vertex[v].v = 1.0f - uv[t].v;
				} else {
					vertex[v].u = 0.0f;
					vertex[v].v = 0.0f;
				}
				vertex_ret.push_back(vertex[v]);

				v = 0;
				n = 0;
				t = 0;
				std::getline(iss, sub, '/');
				if (sub.size()) {
					v = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, '/');
				if (sub.size()) {
					t = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, ' ');
				if (sub.size()) {
					n = std::stoi(sub) - 1;
					sub = "";
				}

				if (useuv) {
					vertex[v].u = uv[t].u;
					vertex[v].v = 1.0f - uv[t].v;
				} else {
					vertex[v].u = 1.0f;
					vertex[v].v = 0.0f;
				}
				vertex_ret.push_back(vertex[v]);

				v = 0;
				n = 0;
				t = 0;
				std::getline(iss, sub, '/');
				if (sub.size()) {
					v = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, '/');
				if (sub.size()) {
					t = std::stoi(sub) - 1;
					sub = "";
				}
				std::getline(iss, sub, ' ');
				if (sub.size()) {
					n = std::stoi(sub) - 1;
					sub = "";
				}

				if (useuv) {
					vertex[v].u = uv[t].u;
					vertex[v].v = 1.0f - uv[t].v;
				} else {
					vertex[v].u = 0.0f;
					vertex[v].v = 1.0f;
				}
				vertex_ret.push_back(vertex[v]);
			}
		}

		size_t size = vertex_ret.size();

		if (size) {
			Vert_t* v = new Vert_t[size];
			dataSize = sizeof(Vert_t) * (unsigned int)size;

			memcpy(v, (char*)vertex_ret.data(), dataSize);

			return (void*)v;
		} else {
			return nullptr;
		}
	}
}