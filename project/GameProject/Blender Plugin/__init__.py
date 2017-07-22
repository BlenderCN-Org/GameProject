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

bl_info = {
"name": "Export Mesh (.mesh)",
'author': '',
"version" : (0,1,0),
"location": "Import-Export",
"description": "",
"category": "Import-Export"
}

if "bpy" in locals():
	import imp
	if "export_mesh" in locals():
		imp.reload(export_mesh)

import bpy
import mathutils
import os.path
import struct
import math

from bpy.props import *
from bpy_extras.io_utils import ExportHelper, ImportHelper

from bpy_extras.io_utils import (
        ImportHelper,
        ExportHelper,
        orientation_helper_factory,
        path_reference_mode,
        axis_conversion,
        )


versions = [("VERSION_1_0", "Version 1.0", "File version 1.0"),
			("VERSION_1_1", "Version 1.1", "File version 1.1")
]

IOOBJOrientationHelper = orientation_helper_factory("IOOBJOrientationHelper", axis_forward='-Z', axis_up='Y')

class ExportMesh(bpy.types.Operator, ExportHelper, IOOBJOrientationHelper):
	'''Export Mesh (.mesh)'''
	bl_idname = 'export_scene.custom_mesh'
	bl_label = 'Export Mesh'
	bl_options = {'UNDO'}

	filename_ext = '.mesh'
	filter_glob = StringProperty(default='*.mesh', options={'HIDDEN'})

	# context group
	use_selection = BoolProperty(
			name="Selection Only",
			description="Export selected objects only",
			default=False,
			)

	global_scale = FloatProperty(
            name="Scale",
            min=0.01, max=1000.0,
            default=1.0,
            )
			
	version = EnumProperty(items=versions, options=set(), default="VERSION_1_0")

	def execute(self, context):
		print('Exporting file', self.filepath)
		
		from . import export_mesh

		keywords = self.as_keywords(ignore=("axis_forward",
											"axis_up",
											"global_scale",
											"check_existing",
											"filter_glob",
											) )
		from mathutils import Matrix
		global_matrix = (Matrix.Scale(self.global_scale, 4) *
						axis_conversion(to_forward=self.axis_forward,
										to_up=self.axis_up,
										).to_4x4())

		keywords["global_matrix"] = global_matrix

		export_mesh.save(context, **keywords)

		return {'FINISHED'}


## register the operator to call the export script
def menu_func_exporter(self, context):
	self.layout.operator(ExportMesh.bl_idname,text="(.mesh)")
		
## the register function register all the UI elemets
def register():
	bpy.utils.register_module(__name__)
	
	##bpy.types.INFO_MT_file_import.append(menu_func_importer)
	bpy.types.INFO_MT_file_export.append(menu_func_exporter)

## if we disable the script we unregister
def unregister():
	bpy.utils.unregister_module(__name__)
	
	##bpy.types.INFO_MT_file_import.remove(menu_func_importer)
	bpy.types.INFO_MT_file_export.remove(menu_func_exporter)

if __name__ == "__main__":
	register()
