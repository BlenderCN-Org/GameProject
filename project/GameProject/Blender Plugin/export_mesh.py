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

if "bpy" in locals():
	import imp
	if "export_skeleton" in locals():
		imp.reload(export_skeleton)

from . import export_skeleton

import bpy
import struct

import ctypes
c_uint8 = ctypes.c_uint8

class Mesh2_0Flags_bits(ctypes.LittleEndianStructure):
    _fields_ = [
            ("uv", c_uint8, 1),
            ("normal", c_uint8, 1),
            ("vColor", c_uint8, 1),
        ]

class Mesh2_0Flags(ctypes.Union):
    _fields_ = [("b", Mesh2_0Flags_bits),
                ("asbyte", c_uint8)]

def writeHeader(fw, version):
	fw(bytes("MESH", "utf-8"))
	if(version == "VERSION_1_0"):
		fw(struct.pack("H", 1))
		fw(struct.pack("H", 0))
	if(version == "VERSION_1_1"):
		fw(struct.pack("H", 1))
		fw(struct.pack("H", 1))
	if(version == "VERSION_2_0"):
		fw(struct.pack("H", 256));
		fw(struct.pack("H", 1));

def createTriangleList(tessfaces):
	triList = []

	for tri in tessfaces:
		triList.append([tri.vertices[0], tri.vertices[1], tri.vertices[2]])
		if len(tri.vertices) == 4:
			triList.append([tri.vertices[0], tri.vertices[2], tri.vertices[3]])

	return triList

def verexWeight(vertex):
	
	numGroups = len(vertex.groups)
	
	if numGroups == 0:
		return [[0,0,0,0], [0,0,0,0]];

	w = []
	ind = []
	for i in range(numGroups):
		w.append(vertex.groups[i].weight)
		ind.append(vertex.groups[i].group)
	
	norm = [float(i)/sum(w) for i in w]
	w = norm

	while(len(w) > 4):
		idx = w.index(min(w))
		w.pop(idx)
		ind.pop(idx)

		norm = [float(i)/sum(w) for i in w]
		w = norm

	while(len(w) < 4):
		w.append(0.0)
		ind.append(0)
		
		norm = [float(i)/sum(w) for i in w]
		w = norm

	return [w, ind]

def createMesh(meshdata):

	meshdata.update(calc_tessface=True)

	vertquad_list = [] # (vertex, uv coordinate, normal, vertex color) list
	vertmap = [None for i in range(len(meshdata.vertices))]
	vertlist = [] 
	normlist = []
	vcollist = []
	uvlist = []
	trilist = []
	weightList = []
	weightIndexList = []
	epsilon = 0.01

	for f in meshdata.tessfaces:
		f_numverts = len(f.vertices)
		if (f_numverts < 3): continue # ignore degenerate faces
		assert((f_numverts == 3) or (f_numverts == 4)) # debug
		# find (vert, uv-vert, normal, vcol) quad, and if not found, create it
		f_index = [-1] * f_numverts
		for i, fv_index in enumerate(f.vertices):
			fv = meshdata.vertices[fv_index].co
			weights = verexWeight(meshdata.vertices[fv_index])
			if f.use_smooth:
				fn = meshdata.vertices[fv_index].normal
			else:
				fn = f.normal
			
			hasUV = False
			hasCol = False

			fuv = []
			if len(meshdata.uv_textures) > 0 :
				hasUV = True
				fuv.append(getattr(meshdata.tessface_uv_textures[0].data[f.index],
						"uv%i" % (i + 1)))
			
			fcol = []
			if len(meshdata.vertex_colors) > 0:
				hasCol = True
				fcol.append(getattr(meshdata.tessface_vertex_colors[0].data[f.index],
						"color%i" % (i + 1)))
			
			vertquad = (fv, fuv, fn, fcol, weights[0], weights[1])

			f_index[i] = len(vertquad_list)
			if vertmap[fv_index]:
				for j in vertmap[fv_index]:
					#if abs(vertquad[0][0] - vertquad_list[j][0][0]) > epsilon:
					#continue
					#if abs(vertquad[0][1] - vertquad_list[j][0][1]) > epsilon:
					#continue
					#if abs(vertquad[0][2] - vertquad_list[j][0][2]) > epsilon:
					#continue
					if hasUV:
						if abs(vertquad[1][0][0] - vertquad_list[j][1][0][0]) > epsilon: continue
						if abs(vertquad[1][0][1] - vertquad_list[j][1][0][1]) > epsilon: continue
					if abs(vertquad[2][0] - vertquad_list[j][2][0]) > epsilon: continue
					if abs(vertquad[2][1] - vertquad_list[j][2][1]) > epsilon: continue
					if abs(vertquad[2][2] - vertquad_list[j][2][2]) > epsilon: continue
					if hasCol:
						if abs(vertquad[3][0].r - vertquad_list[j][3][0].r) > epsilon: continue
						if abs(vertquad[3][0].g - vertquad_list[j][3][0].g) > epsilon: continue
						if abs(vertquad[3][0].b - vertquad_list[j][3][0].b) > epsilon: continue
					#weights
					if abs(vertquad[4][0] - vertquad_list[j][4][0]) > epsilon: continue
					if abs(vertquad[4][1] - vertquad_list[j][4][1]) > epsilon: continue
					if abs(vertquad[4][2] - vertquad_list[j][4][2]) > epsilon: continue
					if abs(vertquad[4][3] - vertquad_list[j][4][3]) > epsilon: continue
					#weights index
					if abs(vertquad[5][0] - vertquad_list[j][5][0]) > epsilon: continue
					if abs(vertquad[5][1] - vertquad_list[j][5][1]) > epsilon: continue
					if abs(vertquad[5][2] - vertquad_list[j][5][2]) > epsilon: continue
					if abs(vertquad[5][3] - vertquad_list[j][5][3]) > epsilon: continue
					#if abs(vertquad[3][0].a - vertquad_list[j][3][0].a) >
					#epsilon: continue #alpha does not exist in blender 2.64
					# all tests passed: so yes, we already have it!
					f_index[i] = j
					break

			if (f_index[i] == len(vertquad_list)):
				# first: add it to the vertex map
				if not vertmap[fv_index]:
					vertmap[fv_index] = []
				vertmap[fv_index].append(len(vertquad_list))
				# new (vert, uv-vert, normal, vcol) quad: add it
				vertquad_list.append(vertquad)
				# add all data to their list
				vertlist.append(vertquad[0])
				normlist.append(vertquad[2])
				if hasCol:
					vcollist.append(vertquad[3])
				if hasUV:
					uvlist.append(vertquad[1])
				
				weightList.append(vertquad[4])
				weightIndexList.append(vertquad[5])

		for i in range(f_numverts - 2):
			if True: #TODO: #(ob_scale > 0):
				f_indexed = (f_index[0], f_index[1 + i], f_index[2 + i])
			else:
				f_indexed = (f_index[0], f_index[2 + i], f_index[1 + i])
			trilist.append(f_indexed)
	
	return [vertlist, normlist, vcollist, uvlist, trilist, weightList, weightIndexList]

def writeVersion_1_0(context, fw, objects, matrix):
	print("Writing version 1.0")
	
	dataObjectList = []

	objectList = objects

	for obj in objectList:
		if(obj.type == 'MESH'):
			
			data = bytearray()
			print("a mesh")

			mesh = obj.to_mesh(context.scene, True, 'PREVIEW', calc_tessface=False)
			mesh.transform(matrix * obj.matrix_world)
			
			meshData = createMesh(mesh)

			useVNormals = True
			useVColors = False
			useVUV = False
			padding = False

			vertlist = meshData[0]
			normlist = meshData[1]
			vcollist = meshData[2]
			uvlist = meshData[3]
			trilist = meshData[4]
			
			#print("verts", vertlist)
			#print("normal", normlist)
			#print("colors", vcollist)
			#print("uv", uvlist)
			#print("tris", trilist)

			if(len(vcollist) > 0):
				useVColors = True

			if(len(uvlist) > 0):
				useVUV = True

			#if len(meshData.vertex_colors) != 0:
			#	useVColors = True
			#	vertexColors = meshData.vertex_colors[0].data
			#	if len(meshData.vertex_colors) > 1:
			#		print("Only 1 vertex color layer is supported for file version 1.0")
			#
			##if len(meshData.uv_layers) != 0:
			##	useVUV = True
			##	vertexUV = meshData.uv_layers[0].data;
			##	if len(meshData.uv_layers) > 1:
			##		print("Only 1 uv layer is supported for file version 1.0")
			#
			data.extend(struct.pack("????", useVNormals, useVColors, useVUV, padding))
			data.extend(struct.pack("II", len(vertlist), len(trilist)))
			
			##print(len(vertices))
			##print(len(vertexColors))
			##print(len(vertexUV))
			#
			for vert in vertlist:
				data.extend(struct.pack("fff", vert[0], vert[1], vert[2]))
			
			if useVNormals:
				for vert in normlist:
					data.extend(struct.pack("fff", vert[0], vert[1], vert[2]))
			
			if useVColors:
				for col in vcollist:
					data.extend(struct.pack("ffff", col[0][0], col[0][1], col[0][2], 1.0))
			
			if useVUV:
				for uv in uvlist:
					data.extend(struct.pack("ff", uv[0][0], uv[0][1]))
					
			for tri in trilist:
				data.extend(struct.pack("III", tri[0], tri[1], tri[2]))

			dataObjectList.append(data)
			bpy.data.meshes.remove(mesh)
		else:
			print("not a mesh")

	print("List: ", len(dataObjectList))
	fw(struct.pack("I", len(dataObjectList)))
	for data in dataObjectList:
		fw(data)

def vGroupIndexToName(indexList, vGroups):

	idxNameMap = []

	for index in indexList:
		for i in index:
			idxNameMap.extend([i, vGroups[i].name])
	
	return idxNameMap

def createBoneList_2_0(bones, indexList):

	# all bone index we use
	cnt = 0
	bones2 = []
	for index in indexList:
		for i in index:
			if(bones[i] not in bones2):
				bones2.append(bones[i])
				cnt += 1
				print(bones[i])
	
	#print(cnt)
	
	# list of bones with id, parent index, and pointer to self
	boneList = [[i ,0, b] for i, b in enumerate(bones2) ]

	for b in boneList:
		for i, p in enumerate(boneList):
			if(b[2].parent == p[2]):
				b[1] = i
	

	return boneList


def createBoneList_2_1(bones, vGroups, vIdx):

	# map vertex index to bone name
	boneNames = []
	for index in vIdx:
		for i in index:
			v = [i, vGroups[i].name]
			if(v not in boneNames):
				boneNames.append(v)

	# generate full skeleton from parent info
	skeleton = []
	for index, name in boneNames:
		if bones[name].name not in skeleton:
			skeleton.append(bones[name].name)
			for p in bones[name].parent_recursive:
				if p.name not in skeleton:
					skeleton.append(p.name)

	# store group index, skeleton bone index, name and bone
	boneList = [[0 ,i , name, bones[name]] for i, name in enumerate(skeleton) ]
	bDict = {}

	for vb in boneNames:
		boneList[skeleton.index(vb[1])][0] = vb[0]
		bDict[vb[0]] = skeleton.index(vb[1])

	return [boneList, bDict]

def parentIndex(boneList, currentBone):
	index = currentBone[1]

	parentBone = currentBone[3].parent

	if parentBone is not None:
		parentName = parentBone.name
		for bone in boneList:
			if bone[2] == parentName:
				index = boneList.index(bone)
				break

	return index

def getPoseBoneLocalMatrix(poseBone):
    if poseBone.parent is None:
        return poseBone.matrix
    else:
        return poseBone.parent.matrix.inverted() * poseBone.matrix

def writeVersion_2_0(context, fw, objects, matrix):
	print("Writing version 2.0")
	dataObjectList = []

	objectList = objects

	for obj in objectList:
		if(obj.type == 'MESH'):

			data = bytearray()
			print("a mesh")

			rig = obj.find_armature()
			armPos = None
			if rig is not None:
				armPos = rig.data.pose_position
				rig.data.pose_position = 'REST'
			
			context.scene.update()

			mesh = obj.to_mesh(context.scene, True, 'PREVIEW', calc_tessface=False)
			
			if rig is not None:
				rig.data.pose_position = armPos
			
			context.scene.update()

			mesh.transform(matrix * obj.matrix_world)
			
			meshData = createMesh(mesh)

			useVNormals = True
			useVColors = False
			useVUV = False
			padding = False

			vertlist = meshData[0]
			normlist = meshData[1]
			vcollist = meshData[2]
			uvlist = meshData[3]
			trilist = meshData[4]
			weightlist = meshData[5]
			weightindexlist = meshData[6] # vertex group index



			if(len(vcollist) > 0):
				useVColors = True

			if(len(uvlist) > 0):
				useVUV = True

			flags = Mesh2_0Flags()
			flags.b.uv = useVUV
			flags.b.normal = useVNormals
			flags.b.vColor = useVColors

			print(flags.b.uv)
			print(flags.b.normal)
			print(flags.b.vColor)
			print(flags.asbyte)

			numBones = 0
			
			numAnumations = len(bpy.data.actions)

			bones = []
			bDict = []

			if(rig is not None):
				arm = rig.data.copy()
				arm.transform(matrix * obj.matrix_world)
				a = createBoneList_2_1(arm.bones, obj.vertex_groups, weightindexlist)
				bones = a[0]
				bDict = a[1]
				numBones = len(bones)

			print("Nr bones ", numBones)

			#data.extend(struct.pack("????", useVNormals, useVColors, useVUV, padding))
			data.extend(struct.pack("B", flags.asbyte))
			data.extend(struct.pack("B", numBones))
			data.extend(struct.pack("H", numAnumations))
			data.extend(struct.pack("II", len(vertlist), len(trilist)))
			
			print(len(vertlist))
			print(len(trilist))

			for vert in vertlist:
				data.extend(struct.pack("fff", vert[0], vert[1], vert[2]))
			
			if useVNormals:
				for vert in normlist:
					data.extend(struct.pack("fff", vert[0], vert[1], vert[2]))
			
			if useVColors:
				for col in vcollist:
					data.extend(struct.pack("ffff", col[0][0], col[0][1], col[0][2], 1.0))
			
			if useVUV:
				for uv in uvlist:
					data.extend(struct.pack("ff", uv[0][0], uv[0][1]))
					
			if numBones > 0:
				for w in weightlist:
					data.extend(struct.pack("ffff", w[0], w[1], w[2], w[3] ))
				for i in weightindexlist:
					data.extend(struct.pack("IIII", bDict[i[0]], bDict[i[1]], bDict[i[2]], bDict[i[3]] ))

			for tri in trilist:
				data.extend(struct.pack("III", tri[0], tri[1], tri[2]))

			dataObjectList.append(data)
			bpy.data.meshes.remove(mesh)
			if(rig is not None):

				dict = {}
				nameCount = 0

				print("Bone info")
				for b in bones:
					name = b[2]
					length = len(name)
					data.extend(struct.pack("H", length))
					data.extend(bytes(name, "utf-8"))
					dict[name] = nameCount;
					nameCount += 1

				print("Bindpose")
				for b in bones:
					id = b[1]
					pInd = parentIndex(bones, b)
					#v_tail = b[2].tail_local.xyz
					#v_head = b[2].head_local.xyz
					#print(b[2].name, ", ", v_tail.x, v_tail.y, v_tail.z)
					data.extend(struct.pack("HHHH", id, dict[b[2]], pInd, 0))
					#data.extend(struct.pack("ffffff",v_head.x, v_head.y, v_head.z, v_tail.x, v_tail.y, v_tail.z))
					
					if b[3].parent is not None:
						pMat = b[3].parent.matrix_local
						mat = pMat.inverted() * b[3].matrix_local
					else:
						mat = b[3].matrix_local
					#print(b[2], "\n", mat, "\n")
					print(b[2], "\n", mat)
					v1 = mat[0]
					v2 = mat[1]
					v3 = mat[2]
					v4 = mat[3]
					data.extend(struct.pack("ffff", v1.x, v1.y, v1.z, v1.w))
					data.extend(struct.pack("ffff", v2.x, v2.y, v2.z, v2.w))
					data.extend(struct.pack("ffff", v3.x, v3.y, v3.z, v3.w))
					data.extend(struct.pack("ffff", v4.x, v4.y, v4.z, v4.w))


				print("posebone")

				for action in bpy.data.actions:

					rig.animation_data.action = action
					frameStart = action.frame_range[0]
					frameEnd = action.frame_range[1]

					numFrames = frameEnd - frameStart

					data.extend(struct.pack("HH", 0, int(numFrames)))
					print(numFrames)

					for i in range(int(frameStart), int(frameEnd)):
						bpy.context.scene.frame_set(i)
						print(action.name, " ", i)

						pose = rig.pose

						for b in bones:
							poseBone = pose.bones[b[2]]

							mat = None

							#print("*"*5, " ", poseBone.name, " ", "*"*5)

							if poseBone.parent is not None:
								pMat = poseBone.parent.matrix
								mat = pMat.inverted() * poseBone.matrix
								#mat = poseBone.matrix
								#print("parent inv ", poseBone.parent.matrix.inverted())
							else:
								mat = matrix * poseBone.matrix

							#mat = matrix * mat

							#print("mat ", mat)

							#print(poseBone.name, "\n", mat, "\n")

							id = b[1]
							pInd = parentIndex(bones, b)
							data.extend(struct.pack("HH", id, pInd))

							v1 = mat[0]
							v2 = mat[1]
							v3 = mat[2]
							v4 = mat[3]
							data.extend(struct.pack("ffff", v1.x, v1.y, v1.z, v1.w))
							data.extend(struct.pack("ffff", v2.x, v2.y, v2.z, v2.w))
							data.extend(struct.pack("ffff", v3.x, v3.y, v3.z, v3.w))
							data.extend(struct.pack("ffff", v4.x, v4.y, v4.z, v4.w))

				bpy.data.armatures.remove(arm)
		else:
			print("not a mesh")

	print("List: ", len(dataObjectList))
#	fw(struct.pack("I", len(dataObjectList)))
	for data in dataObjectList:
		fw(data)


def write(context, fw, objects, version, matrix):
	if(version == "VERSION_1_0"):
		writeHeader(fw, version)
		writeVersion_1_0(context, fw, objects, matrix)
	elif(version == "VERSION_1_1"):
		print("version 1.1 is animation only, exporting 1.0 mesh version!")
		print("Version 1.0 does not have vertex weights!")
		writeHeader(fw, "VERSION_1_0")
		writeVersion_1_0(context, fw, objects, matrix)
	elif(version == "VERSION_2_0"):
		writeHeader(fw, "VERSION_2_0")
		writeVersion_2_0(context, fw, objects, matrix)

def writeMesh(context, objects, filepath, version, global_matrix):
	file = open(filepath, "wb")
	fw = file.write
	try:
		write(context, fw, objects, version, global_matrix)
	except:
		print("export failed!")
		print('-'*60)
		traceback.print_exc(file=sys.stdout)
		print('-'*60)
	finally:
		del fw
		file.close()

def save(context,
		filepath,
		*,
		use_selection=True,
		export_meshes_flag=True,
		export_skeleton_flag=True,
		export_animation_flag=True,
		version="VERSION_1_0",
		global_matrix=None):
	
	# get list of objects to select
	objectList = None

	if(use_selection):
		objectList = context.selected_objects
		# if only using selection selected objects might have armature on it
		for obj in objectList:
			if obj.type == 'MESH':
				arm = obj.find_armature() 
				if arm is not None:
					objectList.append(arm)
	else:
		objectList = context.scene.objects

		## no objects to export
	if len(objectList) == 0:
		return {'FINISHED'}
	if(export_meshes_flag):
		writeMesh(context, objectList, filepath, version, global_matrix)
	if(export_skeleton_flag) and (version == "VERSION_1_1"):
		export_skeleton.writeSkeleton(context, objectList, filepath, version, global_matrix)

	return {'FINISHED'}
