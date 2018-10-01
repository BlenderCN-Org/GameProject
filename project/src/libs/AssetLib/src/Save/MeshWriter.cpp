#include <AssetLib/AssetLib.hpp>

#include "../CustomMesh/MeshFormat.hpp"

#include "WriteVeresion.hpp"

namespace AssetLib {


	void saveMesh(const char* fileName, const Model& mdl, const SaveFlags& rd_only_flags) {

		switch (rd_only_flags.version) {
		case ModelVersion::VERSION_AUTO:
		case ModelVersion::VERSION_2_1:
			saveMeshV2_1(fileName, mdl, rd_only_flags);
			break;
		case ModelVersion::VERSION_2_0:
			saveMeshV2_0(fileName, mdl, rd_only_flags);
			break;
		default:
			break;
		}

		

	}

}