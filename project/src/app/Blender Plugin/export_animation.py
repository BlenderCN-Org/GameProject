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

### Test code in blender

import bpy


def verexWeight(vertex):
	numGroups = len(vertex.groups)
	
	w = []
	ind = []
	for i in range(numGroups):
		w.append(vertex.groups[i].weight)
		ind.append(vertex.groups[i].group)
	
	while(len(w) > 4):
		idx = w.index(min(w))
		w.pop(idx)
		ind.pop(idx)

		norm = [float(i)/sum(w) for i in w]
		w = norm

	while(len(w) < 4):
		w.append(0)
		ind.append(-1)

	return [w, ind]


def vGroupIndexToName(indexList, vGroups):
	
	idx = []
	name = []

	for index in indexList:
		for i in index:
			if i not in idx:
				idx.append(i)
				print(i)
				if i == -1:
					name.append(None)
				else:
					name.append(vGroups[i].name)

	return [idx, name]

def toLongList(indexList):
	idx = []
	for index in indexList:
		for i in index:
			idx.append(i)
	return list(set(idx))

weightWIdx = []

for v in bpy.data.objects['Cube'].data.vertices:
    w = verexWeight(v)
    weightWIdx.append(w[1])

print(weightWIdx)
    
nameList = vGroupIndexToName(weightWIdx, bpy.data.objects['Cube'].vertex_groups)

simpleList = toLongList(weightWIdx)

print("*"*50)
#print(simpleList)
#print(nameList)

#for i, j in enumerate(nameList):
#    print(i, j, '\n')

#inds = list(set(inds))
#print("-"*50)
#print(inds)
#print("-"*50)

for i in simpleList:
	if i == -1:
		print("root")
	else:
		print(bpy.data.objects['Cube'].vertex_groups[i].name)
		print(bpy.data.armatures['rig.001'].bones[i].name)
	print("*"*50)
	
### end test code