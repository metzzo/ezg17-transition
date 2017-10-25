#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Node.h"
#include "GroupNode.h"
#include "TextureResource.h"
#include "MeshResource.h"
#include "RenderingEngine.h"

#define MODEL_LOADER_TEXTURE_DIRECTORY "assets/gfx/"

class ColladaImporter {

public:
	ColladaImporter(RenderingEngine* engine);
	//Destructor removes created textures.
	~ColladaImporter();
	Node* load_node(const std::string& path);

private:
	RenderingEngine* engine_;

	void processLights(const aiScene* scene, std::vector<Node*>& lights);
	void processNode(aiNode* node, const aiScene* scene, std::vector<Node*>& lights, std::vector<TextureResource*>& textures, GroupNode* parent);
	MeshResource* processMesh(aiMesh* node, const aiScene* scene, std::vector<TextureResource*>& textures);
	std::vector<TextureResource*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<TextureResource*>& textures);
};