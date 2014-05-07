#
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ***** END GPL LICENCE BLOCK *****

# (c) 2012 Chad McKinney

'''
bl_info = {
    'name': 'Meta Mesh',
    'author': 'Chad McKinney, octopiansociety, (http://www.blenderaddons.com)',
    'version': (0, 1),
    "blender": (2, 6, 3),
    'location': 'View3D > Add > Mesh',
    'description': 'Create meshes that have associated meta-data to be used in a game for instance',
    'warning': '',  # used for warning icon and text in addons panel
    'category': 'Add Mesh'}
'''

import bpy
import bmesh

from bpy.props import IntProperty, FloatProperty, BoolProperty, StringProperty
from bpy_extras.object_utils import AddObjectHelper, object_data_add

def add_meta_mesh_object_data(self, context):
    meshObject = self.meshFunction()
    ob = bpy.context.active_object
    ob["PhysicsEnabled"] = self.PhysicsEnabled
    ob["ShapeType"] = self.ShapeType

def add_meta_collision_mesh_object_data(self, context):
    meshObject = self.meshFunction()
    ob = bpy.context.active_object
    ob["PhysicsEnabled"] = self.PhysicsEnabled
    ob["BodyType"] = self.BodyType
    ob["ShapeType"] = self.ShapeType
    ob["CollisionGroup"] = self.CollisionGroup
    ob["CursorType"] = self.CursorType
    ob["Mass"] = self.Mass
    ob["Hollow"] = self.Hollow


class AddSphere(bpy.types.Operator, AddObjectHelper):
    """Add a Meta Mesh Sphere Object"""
    bl_idname = "mesh.primitive_meta_sphere_add"
    bl_label = "Meta Sphere"
    bl_options = {'REGISTER', 'UNDO', 'PRESET'}
    
    bpy.types.Object.PhysicsEnabled = bpy.props.BoolProperty(name="PhysicsEnabled", default=False, description="Whether or not a physics object will be created for this mesh")
    bpy.types.Object.ShapeType = bpy.props.StringProperty(name="ShapeType", default="Box", description="The shape type: Box, Sphere, Mesh")
    PhysicsEnabled = False
    ShapeType = "Sphere"

    meshFunction = bpy.ops.mesh.primitive_ico_sphere_add
    
    def execute(self, context):
        add_meta_mesh_object_data(self, context)
        return {'FINISHED'}


class AddCube(bpy.types.Operator, AddObjectHelper):
    """Add a Meta Mesh Cube Object"""
    bl_idname = "mesh.primitive_meta_cube_add"
    bl_label = "Meta Cube"
    bl_options = {'REGISTER', 'UNDO', 'PRESET'}
    
    bpy.types.Object.PhysicsEnabled = bpy.props.BoolProperty(name="PhysicsEnabled", default=False, description="Whether or not a physics object will be created for this mesh")
    bpy.types.Object.ShapeType = bpy.props.StringProperty(name="ShapeType", default="Box", description="The shape type: Box, Sphere, Mesh")
    PhysicsEnabled = False
    ShapeType = "Box"

    meshFunction = bpy.ops.mesh.primitive_cube_add
    
    def execute(self, context):
        add_meta_mesh_object_data(self, context)
        return {'FINISHED'}


class AddCollisionCube(bpy.types.Operator, AddObjectHelper):
    """Add a Meta Mesh Collision Cube Object"""
    bl_idname = "mesh.primitive_meta_collision_cube_add"
    bl_label = "Meta Collision Cube"
    bl_options = {'REGISTER', 'UNDO', 'PRESET'}
    
    bpy.types.Object.PhysicsEnabled = bpy.props.BoolProperty(name="PhysicsEnabled", default=False, description="Whether or not a physics object will be created for this mesh")
    bpy.types.Object.BodyType = bpy.props.StringProperty(name="BodyType", default="Rigid", description="The body type for the object: Ghost, Rigid, Soft")
    bpy.types.Object.ShapeType = bpy.props.StringProperty(name="ShapeType", default="Box", description="The shape type: Box, Sphere, Mesh")
    bpy.types.Object.CollisionGroup = bpy.props.StringProperty(name="CollisionGroup", default="DefaultGroup", description="The collision group this object will be placed in. Used for collision events.")
    bpy.types.Object.CursorType = bpy.props.StringProperty(name="CursorType", default="NoCursor", description="The cursor used when the player looks at this object.")
    bpy.types.Object.Mass = bpy.props.FloatProperty(name="Mass", default=0.0, min=0.0, max=float('infinity'), description="The Mass for the object in the physics world")
    bpy.types.Object.Hollow = bpy.props.BoolProperty(name="Hollow", default=False, description="Whether or not the object (in physics) should be created as a collection of walls, ie. Hollow")
    PhysicsEnabled = True
    BodyType = "Rigid"
    ShapeType = "Box"
    CollisionGroup = "DefaultGroup"
    CursorType = "NoCursor"
    Mass = 0.0
    Hollow = False

    meshFunction = bpy.ops.mesh.primitive_cube_add
    
    def execute(self, context):
        add_meta_collision_mesh_object_data(self, context)
        return {'FINISHED'}


class AddCollisionSphere(bpy.types.Operator, AddObjectHelper):
    """Add a Meta Mesh Collision Sphere Object"""
    bl_idname = "mesh.primitive_meta_collision_sphere_add"
    bl_label = "Meta Collision Sphere"
    bl_options = {'REGISTER', 'UNDO', 'PRESET'}
    
    bpy.types.Object.PhysicsEnabled = bpy.props.BoolProperty(name="PhysicsEnabled", default=False, description="Whether or not a physics object will be created for this mesh")
    bpy.types.Object.BodyType = bpy.props.StringProperty(name="BodyType", default="Rigid", description="The body type for the object: Ghost, Rigid, Soft")
    bpy.types.Object.ShapeType = bpy.props.StringProperty(name="ShapeType", default="Box", description="The shape type: Box, Sphere, Mesh")
    bpy.types.Object.CollisionGroup = bpy.props.StringProperty(name="CollisionGroup", default="DefaultGroup", description="The collision group this object will be placed in. Used for collision events.")
    bpy.types.Object.CursorType = bpy.props.StringProperty(name="CursorType", default="NoCursor", description="The cursor used when the player looks at this object.")
    bpy.types.Object.Mass = bpy.props.FloatProperty(name="Mass", default=0.0, min=0.0, max=float('infinity'), description="The Mass for the object in the physics world")
    bpy.types.Object.Hollow = bpy.props.BoolProperty(name="Hollow", default=False, description="Whether or not the object (in physics) should be created as a collection of walls, ie. Hollow")
    PhysicsEnabled = True
    BodyType = "Rigid"
    ShapeType = "Sphere"
    CollisionGroup = "DefaultGroup"
    CursorType = "NoCursor"
    Mass = 0.0
    Hollow = False

    meshFunction = bpy.ops.mesh.primitive_ico_sphere_add
    
    def execute(self, context):
        add_meta_collision_mesh_object_data(self, context)
        return {'FINISHED'}


class AddCollisionHollowCube(bpy.types.Operator, AddObjectHelper):
    """Add a Meta Mesh Collision Hollow Cube Object"""
    bl_idname = "mesh.primitive_meta_collision_hollow_cube_add"
    bl_label = "Meta Collision Hollow Cube"
    bl_options = {'REGISTER', 'UNDO', 'PRESET'}
    
    bpy.types.Object.PhysicsEnabled = bpy.props.BoolProperty(name="PhysicsEnabled", default=False, description="Whether or not a physics object will be created for this mesh")
    bpy.types.Object.BodyType = bpy.props.StringProperty(name="BodyType", default="Rigid", description="The body type for the object: Ghost, Rigid, Soft")
    bpy.types.Object.ShapeType = bpy.props.StringProperty(name="ShapeType", default="Box", description="The shape type: Box, Sphere, Mesh")
    bpy.types.Object.CollisionGroup = bpy.props.StringProperty(name="CollisionGroup", default="DefaultGroup", description="The collision group this object will be placed in. Used for collision events.")
    bpy.types.Object.CursorType = bpy.props.StringProperty(name="CursorType", default="NoCursor", description="The cursor used when the player looks at this object.")
    bpy.types.Object.Mass = bpy.props.FloatProperty(name="Mass", default=0.0, min=0.0, max=float('infinity'), description="The Mass for the object in the physics world")
    bpy.types.Object.Hollow = bpy.props.BoolProperty(name="Hollow", default=False, description="Whether or not the object (in physics) should be created as a collection of walls, ie. Hollow")
    PhysicsEnabled = True
    BodyType = "Rigid"
    ShapeType = "Box"
    CollisionGroup = "DefaultGroup"
    CursorType = "NoCursor"
    Mass = 0.0
    Hollow = True

    meshFunction = bpy.ops.mesh.primitive_cube_add
    
    def execute(self, context):
        add_meta_collision_mesh_object_data(self, context)
        return {'FINISHED'}



'''
def menu_func(self, context):
    self.layout.operator(OBJECT_OT_add_meta_mesh.bl_idname,
                        text="MetaMesh", icon="PLUGIN")


def register():
    bpy.utils.register_class(OBJECT_OT_add_meta_mesh)
    bpy.types.INFO_MT_mesh_add.append(menu_func)


def unregister():
    bpy.utils.unregister_class(OBJECT_OT_add_meta_mesh)
    bpy.types.INFO_MT_mesh_add.remove(menu_func)

if __name__ == "__main__":
    register()
'''