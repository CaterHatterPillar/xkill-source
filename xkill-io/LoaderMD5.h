#ifndef XKILL_IO_LOADER_MD5_H
#define XKILL_IO_LOADER_MD5_H

#include <vector>
#include <fstream>

#include <DirectXMath.h>

#include "LoaderMD5JointDesc.h"
#include "LoaderMD5MeshDesc.h"
#include "LoaderMD5ModelDesc.h"

class LoaderMD5
{
public:
	LoaderMD5();
	~LoaderMD5();

	bool loadModel(const std::string& filename, LoaderMD5ModelDesc* modelDesc);
	bool loadAnimation(const std::string& filename);

private:
	void parseParamMD5Version();
	void parseParamCommandline();
	void parseParamNumJoints();
	void parseParamNumMeshes();
	
	void parseParamJoints();

	void parseParamMesh();
	void parseParamShader(LoaderMD5MeshDesc& mesh);
	void parseParamNumVerts(LoaderMD5MeshDesc& mesh);
	void parseParamNumTris(LoaderMD5MeshDesc& mesh);
	void parseParamNumWeights(LoaderMD5MeshDesc& mesh);

	void prepareMesh(LoaderMD5MeshDesc& mesh);
	void prepareNormals(LoaderMD5MeshDesc& mesh);

	void reset();
	int getFileLength(const std::string& filename);
	void ignoreLine(std::ifstream& file, int length);
	void removeQuotes(std::string& str);
	void computeQuaternionW(DirectX::XMFLOAT4& quaternion);

	DirectX::XMFLOAT3 normalizeVector(DirectX::XMFLOAT3 vec);
	DirectX::XMFLOAT3 rotateVector(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT4 quaternion);

	std::ifstream infile_;
	int fileLength_;

	int md5Version_;
	int numJoints_;
	int numMeshes_;

	bool hasAnimation_;

	std::vector<LoaderMD5JointDesc> joints_;
	std::vector<LoaderMD5MeshDesc>  meshes_;

	DirectX::XMFLOAT4X4 localToWorldMatrix_;
};

#endif //XKILL_IO_LOADER_MD5_H