#include "ColladaImporter.h"
#include "Node.h"
#include "GroupNode.h"
#include "GeometryNode.h"
#include "Material.h"
#include "MeshResource.h"
#include <iostream>
#include "LightNode.h"

ColladaImporter::ColladaImporter(RenderingEngine* engine) {
	this->engine_ = engine;
}

ColladaImporter::~ColladaImporter() {
}

Node* ColladaImporter::load_node(const std::string& path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_GenNormals*/);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return nullptr;
	}

	std::vector<Node*> lights;
	std::vector<TextureResource*> textures;
	processLights(scene, lights);
	GroupNode* node = new GroupNode(std::string(scene->mRootNode->mName.C_Str()));
	processNode(scene->mRootNode, scene, lights, textures, node);
	return node;
}

void ColladaImporter::processLights(const aiScene* scene, std::vector<Node*>& lights) {
	for (int i = 0; i < scene->mNumLights; i++) {
		aiLight* light = scene->mLights[i];
		if (light->mType == aiLightSource_POINT) {
			LightNode* ln = LightNode::createPointLight(
				std::string(light->mName.C_Str()),
				glm::vec3(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b),
				glm::vec3(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b),
				glm::vec3(light->mPosition.x, light->mPosition.y, light->mPosition.z),
				glm::vec3(light->mAttenuationConstant, light->mAttenuationLinear, light->mAttenuationQuadratic)
			);
			lights.push_back(ln);
		}
		else if (light->mType == aiLightSource_DIRECTIONAL) {
			LightNode* ln = LightNode::createDirectionalLight(
				std::string(light->mName.C_Str()),
				glm::vec3(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b),
				glm::vec3(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b),
				glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z)
			);
			lights.push_back(ln);
		}
	}
}

void ColladaImporter::processNode(aiNode* node, const aiScene* scene, std::vector<Node*>& lights, std::vector<TextureResource*>& textures, GroupNode* parent) {

	//Alle aktuellen Meshes durchgehen und Objekte erzeugen
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		MeshResource* mesh = processMesh(aiMesh, scene, textures);
		GeometryNode* geoNode = new GeometryNode(parent->get_name() + "_" + std::to_string(i), mesh);
		parent->add_node(geoNode);
	}
	//Sub-Models durchgehen und erzeugen
	for (int i = 0; i < node->mNumChildren; i++) {
		std::string name = std::string(node->mChildren[i]->mName.C_Str());
		bool light = false;
		//Wenn der Name der Node gleich ist dem Namen einer der Lichter, dann muss die Transformation der Node auf das Licht angewandt werden
		for (auto ci = lights.begin(); ci != lights.end() && !light; ci++) {
			if ((*ci)->get_name() == name) {
				auto nT = node->mChildren[i]->mTransformation;
				glm::mat4 transformation = glm::mat4(nT.a1, nT.b1, nT.c1, nT.d1, nT.a2, nT.b2, nT.c2, nT.d2, nT.a3, nT.b3, nT.c3, nT.d3, nT.a4, nT.b4, nT.c4, nT.d4);
				(*ci)->apply_transformation(transformation, glm::inverse(transformation));
				parent->add_node(*ci);
				light = true;
			}
		}
		if (!light) {
			GroupNode* sub = new GroupNode(std::string(node->mChildren[i]->mName.C_Str()));
			processNode(node->mChildren[i], scene, lights, textures, sub);
			parent->add_node(sub);
		}
	}

	//Transformation des Nodes
	auto nT = node->mTransformation;
	glm::mat4 transformation = glm::mat4(nT.a1, nT.b1, nT.c1, nT.d1, nT.a2, nT.b2, nT.c2, nT.d2, nT.a3, nT.b3, nT.c3, nT.d3, nT.a4, nT.b4, nT.c4, nT.d4);
	parent->apply_transformation(transformation, glm::inverse(transformation));
}

MeshResource* ColladaImporter::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TextureResource*>& textures) {
	float *vertices_positions = new float[mesh->mNumVertices*3];
	float *vertices_normals = new float[mesh->mNumVertices * 3];
	float *vertices_uvs = new float[mesh->mNumVertices * 2];
	unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];

	//Process Vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertices_positions[i*3+0] = (mesh->mVertices[i].x);
		vertices_positions[i*3+1] = (mesh->mVertices[i].y);
		vertices_positions[i*3+2] = (mesh->mVertices[i].z);
		vertices_normals[i*3+0] = (mesh->mNormals[i].x);
		vertices_normals[i*3+1] = (mesh->mNormals[i].y);
		vertices_normals[i*3+2] = (mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) {
			vertices_uvs[i*2+0] = (mesh->mTextureCoords[0][i].x);
			vertices_uvs[i*2+1] = (mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices_uvs[i * 2 + 0] = (0);
			vertices_uvs[i * 2 + 1] = (0);
		}
		//mesh->mColors is ignored for now
	}
	//Process indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		//All Faces are Triangles, so mNumIndices is always 3
		for (int j = 0; j < face.mNumIndices; j++) {
			indices[i*3+j] = (face.mIndices[j]);
		}
	}

	Material material;
	//Process materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureResource*> diffuseMaps = loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE, textures);
		if (diffuseMaps.size() != 0) {
			material.set_texture(diffuseMaps.at(0));
		}
		//Properties...
		for (unsigned int i = 0; i < aimaterial->mNumProperties; i++) {
			aiMaterialProperty* prop = aimaterial->mProperties[i];
			if (std::string(prop->mKey.C_Str()) == "$mat.opacity" && prop->mType == aiPTI_Float) {
				float* data = reinterpret_cast<float*>(prop->mData);
				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
				material.set_opacity(data[0]);
			}
			else if (std::string(prop->mKey.C_Str()) == "$clr.emissive" && prop->mType == aiPTI_Float) {
				float* data = reinterpret_cast<float*>(prop->mData);
				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
				if (len < 3) {
					std::cerr << "Unexpected Length in $clr.emissive: " << len << std::endl;
					material.set_ambient_color(glm::vec3(0, 0, 0));
				}
				else {
					material.set_ambient_color(glm::vec3(data[0], data[1], data[2]));
				}
			}
			else if (std::string(prop->mKey.C_Str()) == "$clr.ambient" && prop->mType == aiPTI_Float) {
				/*float* data = reinterpret_cast<float*>(prop->mData);
				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
				if (len < 3) {
					std::cerr << "Unexpected Length in $clr.ambient: " << len << std::endl;
					material.set_ambient_color(glm::vec3(1, 1, 1));
				}
				else {
					material.set_ambient_color(glm::vec3(data[0], data[1], data[2]));
				}*/
				//Ambient is ignored, as not supported by blender
			}
			else if (std::string(prop->mKey.C_Str()) == "$clr.diffuse" && prop->mType == aiPTI_Float) {
				if (diffuseMaps.size() != 0) {
					material.set_diffuse_color(glm::vec3(1, 1, 1));
				}
				else {
					float* data = reinterpret_cast<float*>(prop->mData);
					int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
					if (len < 3) {
						std::cerr << "ModelLoader: Unexpected Length in $clr.diffuse: " << len << std::endl;
						material.set_diffuse_color(glm::vec3(1, 1, 1));
					}
					else {
						material.set_diffuse_color(glm::vec3(data[0], data[1], data[2]));
					}
				}
			}
			else if (std::string(prop->mKey.C_Str()) == "$clr.specular" && prop->mType == aiPTI_Float) {
				float* data = reinterpret_cast<float*>(prop->mData);
				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
				if (len < 3) {
					std::cerr << "ModelLoader: Unexpected Length in $clr.specular: " << len << std::endl;
					material.set_specular_color(glm::vec3(0, 0, 0));
				}
				else {
					material.set_specular_color(glm::vec3(data[0], data[1], data[2]));
				}
			}
			else if (std::string(prop->mKey.C_Str()) == "$mat.shininess" && prop->mType == aiPTI_Float) {
				float* data = reinterpret_cast<float*>(prop->mData);
				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
				if (len != 1) {
					std::cerr << "ModelLoader: Unexpected Length in mat.shininess: " << len << std::endl;
					material.set_shininess(1);
				}
				else {
					material.set_shininess(data[0]);
				}
			}
		}
	}

	MeshResource* res = new MeshResource(vertices_positions, vertices_normals, vertices_uvs, mesh->mNumVertices, indices, mesh->mNumFaces*3, material);
	this->engine_->register_resource(res);
	return res;
}

std::vector<TextureResource*> ColladaImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<TextureResource*>& loaded_textures) {
	std::vector<TextureResource*> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = MODEL_LOADER_TEXTURE_DIRECTORY + std::string(str.C_Str());
		bool alreadyLoaded = false;
		for (int j = 0; j < loaded_textures.size() && !alreadyLoaded; j++) {
			if (loaded_textures[j]->get_texture_path() == path.c_str()) {
				textures.push_back(loaded_textures[j]);
				alreadyLoaded = true;
			}
		}
		if (!alreadyLoaded) {
			TextureResource* texture = new TextureResource(path);
			this->engine_->register_resource(texture);
			loaded_textures.push_back(texture);
			textures.push_back(texture);
		}
	}
	return textures;
}