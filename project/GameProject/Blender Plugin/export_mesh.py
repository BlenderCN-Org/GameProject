#!/usr/bin/python3
# -*- coding: utf-8 -*-

# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

import bpy
import struct

def writeHeader(fw, version):
	fw(bytes("MESH", "utf-8"))

	if(version == "VERSION_1_0"):
		fw(struct.pack("H", 1))
		fw(struct.pack("H", 0))
	if(version == "VERSION_1_1"):
		fw(struct.pack("H", 1))
		fw(struct.pack("H", 1))

def createTriangleList(tessfaces):
	triList = []

	for tri in tessfaces:
		triList.append( [tri.vertices[0], tri.vertices[1], tri.vertices[2] ] )
		if len(tri.vertices) == 4:
			triList.append( [tri.vertices[0], tri.vertices[2], tri.vertices[3] ] )

	return triList

def createBoneList(bones):
	# list of bones with id, parent index, and pointer to self
	boneList = [[i ,0, b] for i, b in enumerate(bones) ]

	for b in boneList:
		for i, p in enumerate(boneList):
			if(b[2].parent == p[2]):
				b[1] = i
				
	return boneList

def writeVersion_1_0(context, fw, use_selection, matrix):
	print("Writing version 1.0")
	
	dataObjectList = []

	objectList = None

	if(use_selection):
		objectList = context.selected_objects
	else:
		objectList = context.scene.objects

	for obj in objectList:
		if( obj.type == 'MESH'):
			
			data = bytearray()
			print("a mesh")

			meshData = obj.to_mesh(context.scene, True, 'PREVIEW', calc_tessface=False)
			meshData.transform(matrix * obj.matrix_world)
			useVNormals = True
			useVColors = False
			useVUV = False
			padding = False

			vertexColors = None
			vertexUV = None
			
			#if len(meshData.vertex_colors) != 0:
			#	useVColors = True
			#	vertexColors = meshData.vertex_colors[0].data;
			#	if len(meshData.vertex_colors) > 1:
			#		print("Only 1 vertex color layer is supported for file version 1.0")
			#
			#if len(meshData.uv_layers) != 0:
			#	useVUV = True
			#	vertexUV = meshData.uv_layers[0].data;
			#	if len(meshData.uv_layers) > 1:
			#		print("Only 1 uv layer is supported for file version 1.0")
			
			data.extend(struct.pack("????", useVNormals, useVColors, useVUV, padding))
			vertices = meshData.vertices
			meshData.update(calc_tessface=True)
			triangles = createTriangleList(meshData.tessfaces)

			data.extend(struct.pack("II", len(vertices), len(triangles) ))

			#print(len(vertices))
			#print(len(vertexColors))
			#print(len(vertexUV))
			
			for vert in vertices:
				data.extend(struct.pack("fff", vert.co[0], vert.co[1], vert.co[2]))

			if useVNormals:
				for vert in vertices:
					data.extend(struct.pack("fff", vert.normal[0], vert.normal[1], vert.normal[2]))

			#if useVColors:
			#	for col in vertexColors:
			#		data.extend(struct.pack("ffff", col.color[0], col.color[1], col.color[2], 1.0))
			#
			#if useVUV:
			#	for uv in vertexUV:
			#		data.extend(struct.pack("ff", uv.uv[0], uv.uv[1]))
					
			for tri in triangles:
				data.extend(struct.pack("III", tri[0], tri[1], tri[2]))

			dataObjectList.append(data)
			bpy.data.meshes.remove(meshData)
		else:
			print("not a mesh")

	print("List: ", len(dataObjectList))
	fw(struct.pack("I", len(dataObjectList)))
	for data in dataObjectList:
		fw(data)

def writeVersion_1_1(context, fw, use_selection, matrix):
	print("Writing version 1.1")
	
	dataObjectList = []

	objectList = None

	if(use_selection):
		objectList = context.selected_objects
	else:
		objectList = context.scene.objects

	for obj in objectList:
		if( obj.type == 'ARMATURE'):
			print("Armature")

			arm = obj.data.copy()

			arm.transform(matrix * obj.matrix_world)

			data = bytearray()

			bones = createBoneList(arm.bones)
			nrBones = len(arm.bones)
			print("Nr bones ", nrBones)
			data.extend(struct.pack("I", nrBones))
			for b in bones:
				id = b[0]
				pInd = b[1]
				v_tail = b[2].tail_local.xyz
				v_head = b[2].head_local.xyz
				print(b[2].name, ", ", v_tail.x, v_tail.y, v_tail.z)
				data.extend(struct.pack("IIfff", id, pInd, v_head.x, v_head.y, v_head.z))
				data.extend(struct.pack("fff", v_tail.x, v_tail.y, v_tail.z))

			dataObjectList.append(data)
			bpy.data.armatures.remove(arm)
		else:
			print("not a mesh")

	print("List: ", len(dataObjectList))
	fw(struct.pack("I", len(dataObjectList)))
	for data in dataObjectList:
		fw(data)


def write(context, fw, use_selection, version, matrix):
	if(version == "VERSION_1_0"):
		writeVersion_1_0(context, fw, use_selection, matrix)
	elif(version == "VERSION_1_1"):
		writeVersion_1_1(context, fw, use_selection, matrix)

def save(context,
		filepath,
        *,
		use_selection=True,
		version="VERSION_1_0",
		global_matrix=None):
	
	file = open(filepath, "wb")
	fw = file.write

	writeHeader(fw, version)
	write(context, fw, use_selection, version, global_matrix)

	file.close()

	return {'FINISHED'}