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

	void process_lights(const aiScene* scene, std::vector<Node*>& lights);
	void process_node(aiNode* node, const aiScene* scene, std::vector<Node*>& lights, std::vector<TextureResource*>& textures, GroupNode* parent);
	MeshResource* process_mesh(aiMesh* node, const aiScene* scene, std::vector<TextureResource*>& textures);
	std::vector<TextureResource*> load_material_textures(aiMaterial* mat, aiTextureType type, std::vector<TextureResource*>& textures);
};