#pragma once
#include "Node.h"
using namespace std;

class ColladaImporter {

public:
	Node* loadeNode(const string& path);

private:
	void processNode(aiNode* node, const aiScene* scene, vector<Node*>& lights, Node* parent);
	MeshResource* processMesh(aiNode* node, const aiScene* scene);
};