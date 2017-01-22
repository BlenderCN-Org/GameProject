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

def createTriangleList(tessfaces):
	triList = []

	for tri in triangles:
		triList.append( [tri.vertices[0], tri.vertices[1], tri.vertices[2] ] )
		if len(tri.vertices) == 4:
			triList.append( [tri.vertices[0], tri.vertices[2], tri.vertices[3] ] )

	return triList

def writeVersion_1_0(context, fw, use_selection):
	print("Writing version 1.0")
	
	objectList = None

	if(use_selection):
		objectList = context.selected_objects
	else:
		objectList = context.scene.objects

	for obj in objectList:
		if( obj.type == 'MESH'):
			print("a mesh")

			meshData = obj.data
			useVNormals = False
			useVColors = False
			useVUV = False
			padding = False

			fw(struct.pack("????", useVNormals, useVColors, useVUV, padding))
			vertices = meshData.vertices
			meshData.update(calc_tessface=True)
			triangles = createTriangleList(meshData.tessfaces)
			fw(struct.pack("II", len(vertices), len(triangles) ))

			for vert in vertices:
				fw(struct.pack("fff", vert.co[0], vert.co[1], vert.co[2]) )

			for tri in triangles:
				fw(struct.pack("III", tri[0], tri[1], tri[2]) )


		else:
			print("not a mesh")

def write(context, fw, use_selection, version):
	if(version == "VERSION_1_0"):
		writeVersion_1_0(context, fw, use_selection)
	

def save(context,
		filepath,
        *,
		use_selection=True,
		version="VERSION_1_0"):
	
	file = open(filepath, "wb")
	fw = file.write

	writeHeader(fw, version)
	write(context, fw, use_selection, version)

	file.close()

	return {'FINISHED'}