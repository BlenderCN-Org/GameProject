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
	fw(bytes("SKEL", "utf-8"))

	if(version == "VERSION_1_1"):
		fw(struct.pack("H", 1))
		fw(struct.pack("H", 1))

def createBoneList(bones):
	# list of bones with id, parent index, and pointer to self
	boneList = [[i ,0, b] for i, b in enumerate(bones) ]

	for b in boneList:
		for i, p in enumerate(boneList):
			if(b[2].parent == p[2]):
				b[1] = i
				
	return boneList

def writeVersion_1_1(context, fw, objects, matrix):
	print("Writing version 1.1")
	
	dataObjectList = []

	objectList = objects

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
			print("not a skeleton")

	print("List: ", len(dataObjectList))
	fw(struct.pack("I", len(dataObjectList)))
	for data in dataObjectList:
		fw(data)

def write(context, fw, objects, version, matrix):
	if(version == "VERSION_1_0"):
		print("Skeleton not supported in version 1.0")
	elif(version == "VERSION_1_1"):
		writeHeader(fw, version)
		writeVersion_1_1(context, fw, objects, matrix)

def writeSkeleton(context, objects, filepath, version, global_matrix):
	print("writing skeleton")

	filepath = filepath[:-4]
	filepath += "skel"

	file = open(filepath, "wb")
	fw = file.write

	
	write(context, fw, objects, version, global_matrix)

	file.close()
