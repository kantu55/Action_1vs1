#pragma once
/*
@file MeshList.h
*/
#ifndef MESHLIST_H_INCLUDED
#define MESHLIST_H_INCLUDED
#include <GL/glew.h>
#include "Geometry.h"
#include <vector>

/*
メッシュ管理クラス
*/
class MeshList
{
public:
	MeshList();
	~MeshList();

	bool Allocate();
	void Free();
	void Add(const Vertex*, const Vertex*, const GLushort*, const GLushort*);
	bool AddFromObjFile(const char* path);

	void BindVertexArray();
	const Mesh& Get(size_t index) const;

private:
	GLuint vao = 0;
	std::vector<Mesh> meshes;

	std::vector<Vertex> tmpVertices;
	std::vector<GLushort> tmpIndices;

};


#endif // !MESHLIST_H_INCLUDED
