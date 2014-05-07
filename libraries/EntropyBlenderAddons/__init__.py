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
# Contributed to by
# Chad McKinney

bl_info = {
    "name": "Meta Mesh",
    "author": "Chad McKinney",
    "version": (0, 1),
    "blender": (2, 6, 1),
    "location": "View3D > Add > Mesh > Meta Mesh",
    "description": "Add meshes with meta data for use outside of Blender.",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/",
    "tracker_url": "http://projects.blender.org/tracker/index.php?",
    "category": "Add Mesh"}


if "bpy" in locals():
    import imp
    imp.reload(add_mesh_meta_mesh)
else:
    from . import add_mesh_meta_mesh
import bpy


class INFO_MT_meta_mesh_add(bpy.types.Menu):
    # Define the "Meta Mesh" menu
    bl_idname = "INFO_MT_meta_mesh_add"
    bl_label = "Meta Mesh"

    def draw(self, context):
        layout = self.layout
        layout.operator_context = 'INVOKE_REGION_WIN'
        layout.menu("INFO_MT_mesh_meta_add", text="Meta Meshes")
        layout.menu("INFO_MT_mesh_meta_collision_add", text="Meta Collision Meshes")

class INFO_MT_mesh_meta_add(bpy.types.Menu):
    # Define the "Collision" menu
    bl_idname = "INFO_MT_mesh_meta_add"
    bl_label = "Meta Meshes"

    def draw(self, context):
        layout = self.layout
        layout.operator_context = 'INVOKE_REGION_WIN'
        layout.operator("mesh.primitive_meta_cube_add", text="Meta Cube")
        layout.operator("mesh.primitive_meta_sphere_add", text="Meta Sphere")

class INFO_MT_mesh_meta_collision_add(bpy.types.Menu):
    # Define the "Collision" menu
    bl_idname = "INFO_MT_mesh_meta_collision_add"
    bl_label = "Meta Collision Meshes"

    def draw(self, context):
        layout = self.layout
        layout.operator_context = 'INVOKE_REGION_WIN'
        layout.operator("mesh.primitive_meta_collision_cube_add", text="Meta Collision Cube")
        layout.operator("mesh.primitive_meta_collision_hollow_cube_add", text="Meta Collision Hollow Cube")
        layout.operator("mesh.primitive_meta_collision_sphere_add", text="Meta Collision Sphere")


# Register all operators and panels

# Define "Meta Mesh" menu
def menu_func(self, context):
    self.layout.menu("INFO_MT_meta_mesh_add", icon="PLUGIN")


def register():
    bpy.utils.register_module(__name__)

    # Add "Extras" menu to the "Add Mesh" menu
    bpy.types.INFO_MT_mesh_add.append(menu_func)


def unregister():
    bpy.utils.unregister_module(__name__)

    # Remove "Extras" menu from the "Add Mesh" menu.
    bpy.types.INFO_MT_mesh_add.remove(menu_func)

if __name__ == "__main__":
    register()
