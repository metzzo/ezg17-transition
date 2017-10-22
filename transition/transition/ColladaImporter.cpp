//#include "ColladaImporter.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include "Node.h"
//#include "GroupNode.h"
//#include "GeometryNode.h"
//#include <iostream>
///**/
//Node* ColladaImporter::loadeNode(const string& path) {
//	Assimp::Importer import;
//	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_GenNormals*/);
//
//	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
//		return nullptr;
//	}
//
//	//TODO: Lights
//	vector<Node*> lights;
//	GroupNode* node = new GroupNode(string(scene->mRootNode->mName.C_Str()));
//	processNode(scene->mRootNode, scene, lights, node);
//	return node;
//}
//
//void ColladaImporter::processNode(aiNode* node, const aiScene* scene, vector<Node*>& lights, GroupNode* parent) {
//
//	//Alle aktuellen Meshes durchgehen und Objekte erzeugen
//	for (int i = 0; i < node->mNumMeshes; i++) {
//		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
//		MeshResource* mesh = processMesh(aiMesh, scene);
//		GeometryNode* geoNode = new GeometryNode(parent->get_name() + "_" + to_string(i), mesh);
//		parent->add_node(geoNode);
//	}
//	//Sub-Models durchgehen und erzeugen
//	for (int i = 0; i < node->mNumChildren; i++) {
//		string name = string(node->mChildren[i]->mName.C_Str());
//		bool light = false;
//		//Wenn der Name der Node gleich ist dem Namen einer der Lichter, dann muss die Transformation der Node auf das Licht angewandt werden
//		for (auto ci = lights.begin(); ci != lights.end() && !light; ci++) {
//			if ((*ci)->get_name() == name) {
//				auto nT = node->mChildren[i]->mTransformation;
//				glm::mat4 transformation = glm::mat4(nT.a1, nT.b1, nT.c1, nT.d1, nT.a2, nT.b2, nT.c2, nT.d2, nT.a3, nT.b3, nT.c3, nT.d3, nT.a4, nT.b4, nT.c4, nT.d4);
//				(*ci)->transform(transformation);	//TODO
//				parent->add_node(*ci);
//				light = true;
//			}
//		}
//		if (!light) {
//			GroupNode* sub = new GroupNode(string(node->mChildren[i]->mName.C_Str()));
//			processNode(node->mChildren[i], scene, lights, sub);
//			parent->add_node(sub);
//		}
//	}
//
//	//Transformation des Nodes
//	auto nT = node->mTransformation;
//	glm::mat4 transformation = glm::mat4(nT.a1, nT.b1, nT.c1, nT.d1, nT.a2, nT.b2, nT.c2, nT.d2, nT.a3, nT.b3, nT.c3, nT.d3, nT.a4, nT.b4, nT.c4, nT.d4);
//	parent->applyTransformation(transformation, glm::inverse(transformation));
//}
//
//MeshResource* processMesh(aiNode* node, aiScene* scene) {
//	vector<float> vertices;
//	//TODO
//	vector<GLuint> indices;
//	Material material;
//
//	//Process Vertices
//	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
//		Vertex vertex;
//		vertex.position.x = mesh->mVertices[i].x;
//		vertex.position.y = mesh->mVertices[i].y;
//		vertex.position.z = mesh->mVertices[i].z;
//		vertex.normal.x = mesh->mNormals[i].x;
//		vertex.normal.y = mesh->mNormals[i].y;
//		vertex.normal.z = mesh->mNormals[i].z;
//		if (mesh->mTextureCoords[0]) {
//			vertex.uv.x = mesh->mTextureCoords[0][i].x;
//			vertex.uv.y = mesh->mTextureCoords[0][i].y;
//		}
//		else {
//			vertex.uv = glm::vec2(0, 0);
//		}
//		if (mesh->mColors[0]) {
//			vertex.color.r = mesh->mColors[0][i].r;
//			vertex.color.g = mesh->mColors[0][i].g;
//			vertex.color.b = mesh->mColors[0][i].b;
//			vertex.color.a = mesh->mColors[0][i].a;
//		}
//		else {
//			vertex.color = glm::vec4(1, 1, 1, 1);
//		}
//		vertices.push_back(vertex);
//	}
//	//Process indices
//	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
//		aiFace face = mesh->mFaces[i];
//		//All Faces are Triangles, so mNumIndices is always 3
//		for (GLuint j = 0; j < face.mNumIndices; j++) {
//			indices.push_back(face.mIndices[j]);
//		}
//	}
//
//	//Process materials
//	if (mesh->mMaterialIndex >= 0) {
//		aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
//		vector<Texture*> diffuseMaps = loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE);
//		if (diffuseMaps.size() == 0) {
//			material.texture = nullptr;
//			material.diffuseHasTexture = false;
//		}
//		else {
//			material.texture = diffuseMaps.at(0);
//			material.diffuseHasTexture = true;
//		}
//		//Properties...
//		for (unsigned int i = 0; i < aimaterial->mNumProperties; i++) {
//			aiMaterialProperty* prop = aimaterial->mProperties[i];
//			/*if (string(prop->mKey.C_Str()) == "?mat.name" && prop->mType == aiPTI_String) {
//			char* data = reinterpret_cast<char*>(prop->mData);
//			int len = (prop->mDataLength * sizeof(char)) / sizeof(char);
//			for (int j = 0; j < len; j++) {
//			cout << data[j];
//			}
//			cout << endl;
//			}*/
//			if (string(prop->mKey.C_Str()) == "$mat.opacity" && prop->mType == aiPTI_Float) {
//				float* data = reinterpret_cast<float*>(prop->mData);
//				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
//				material.opacity = data[0];
//			}
//			else if (string(prop->mKey.C_Str()) == "$clr.ambient" && prop->mType == aiPTI_Float) {
//				float* data = reinterpret_cast<float*>(prop->mData);
//				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
//				if (len < 3) {
//					DebugLogger::log("ModelLoader: Unexpected Length in $clr.ambient: " + len);
//					material.ambientColor = glm::vec3(1, 1, 1);
//				}
//				else {
//					material.ambientColor = glm::vec3(data[0], data[1], data[2]);
//				}
//			}
//			else if (string(prop->mKey.C_Str()) == "$clr.diffuse" && prop->mType == aiPTI_Float) {
//				float* data = reinterpret_cast<float*>(prop->mData);
//				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
//				if (len < 3) {
//					DebugLogger::log("ModelLoader: Unexpected Length in $clr.diffuse: " + len);
//					material.diffuseColor = glm::vec3(1, 1, 1);
//				}
//				else {
//					material.diffuseColor = glm::vec3(data[0], data[1], data[2]);
//				}
//			}
//			else if (string(prop->mKey.C_Str()) == "$clr.specular" && prop->mType == aiPTI_Float) {
//				float* data = reinterpret_cast<float*>(prop->mData);
//				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
//				if (len < 3) {
//					DebugLogger::log("ModelLoader: Unexpected Length in $clr.specular: " + len);
//					material.specularColor = glm::vec3(0, 0, 0);
//				}
//				else {
//					material.specularColor = glm::vec3(data[0], data[1], data[2]);
//				}
//			}
//			else if (string(prop->mKey.C_Str()) == "$mat.shininess" && prop->mType == aiPTI_Float) {
//				float* data = reinterpret_cast<float*>(prop->mData);
//				int len = (prop->mDataLength * sizeof(char)) / sizeof(float);
//				if (len != 1) {
//					DebugLogger::log("ModelLoader: Unexpected Length in mat.shininess: " + len);
//					material.shininess = 1;
//				}
//				else {
//					material.shininess = data[0];
//				}
//			}
//		}
//	}
//
//	return new Mesh(glm::vec3(0, 0, 0), vertices, indices, material);
//}