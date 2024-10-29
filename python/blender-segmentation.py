#!/usr/bin/env python3

import bpy
import re

# Use: blender --background /path/to/blender-models/segmentation-test.blend --python /path/to/blender-segmentation.py /tmp/seg
# WARNING: --background [blender_path] must come FIRST, otherwise it will load the default blender scene for python

def hsv2rgb(ls, ls_indx, h, s, v):
	# Same as from tagem/svgedit.js, but more fixes/performance improvements
	# Expects 0<=h<6, and 0<=x<=1 for all s,v
	j = int(h);
	f = h - j;
	p = v * (1.0 - s);
	q = v * (1.0 - f * s);
	t = v * (1.0 - (1.0 - f) * s); # = p + f * s * v
	ls[ls_indx+(((j+1)%6)>>1)] = v;
	ls[ls_indx+(((j+4)%6)>>1)] = p;
	ls[ls_indx+ ((7-j)%3)    ] = (j&1) * q + (1-(j&1)) * t;
	'''a:int = (((j+1)%6)>>1)
	b:int = (((j+4)%6)>>1)
	c:int =  ((7-j)%3)
	indices:list = sorted([a,b,c])
	if (indices[0] != 0) or (indices[1] != 1) or (indices[2] != 2):
		print(f"j={j}  Bad indices: {a},{b},{c}")'''


def render_internals(is_segmented:bool, outdir:str, all_camera_names:list, count_renders_to_do:bool):
	j:int = 0
	count:int = 0
	for scene_n, scene in enumerate(bpy.data.scenes):
		if not count_renders_to_do:
			bpy.context.window.scene = scene
			scene.render.resolution_percentage = 100
			#scene.render.resolution_x = w #perhaps set resolution in code
			#scene.render.resolution_y = h
			if is_segmented:
				scene.render.image_settings.file_format = 'PNG'
				scene.render.engine = "BLENDER_WORKBENCH"
				scene.display.shading.light = 'FLAT' # WARNING: Tooltip wrongly forgets .display.
				# TODO: Use this to determine border between two instances of the same tag: scene.shading.show_object_outline = True or False
				# Does nothing scene.grease_pencil_settings.antialias_threshold = 0.0
				scene.display.render_aa = "OFF"
				scene.use_nodes = False # Disable compositing
			else:
				scene.render.image_settings.file_format = 'JPEG'
				#scene.cycles.max_bounces=12
				scene.cycles.samples = 4
				#scene.cycles.caustics_reflective = False
				#scene.cycles.caustics_refractive = False
		for camera_name in all_camera_names:
			scene.camera = bpy.data.objects[camera_name]
			keyframe_ns:list = []
			if (scene.camera.animation_data is not None) and (scene.camera.animation_data.action is not None):
				for curve in scene.camera.animation_data.action.fcurves:
					for keyframe in curve.keyframe_points:
						keyframe_ns.append(int(keyframe.co[0]))
					'''
					print(curve.array_index)
					print(curve.is_empty)
					print(curve.range())
					print(dir(curve))
					print("---")'''
				keyframe_ns = sorted(list(set(keyframe_ns)))
			if len(keyframe_ns) == 0:
				keyframe_ns = [0]
			for frame_n in keyframe_ns:
				basepath:str = f"{outdir}/scene{scene_n}_{camera_name}_{frame_n}"
				j += 1
				if os.path.isfile(f"{basepath}.{'png' if is_segmented else 'jpg'}"):
					continue
				count += 1
				if not count_renders_to_do:
					bpy.context.scene.frame_set(frame_n)
					scene.render.filepath = basepath
					bpy.ops.render.render(write_still=True)
					print(f"RENDERED {j-1}: {basepath}")
	return count


def render_textured(outdir:str, obj_names:list, all_camera_names:list):
	render_internals(False, outdir, all_camera_names, False)



def render_segmentation_fullmask(outdir:str, obj_names:list, all_camera_names:list):
	for world in bpy.data.worlds:
		world.color = (0.0, 0.0, 0.0)
	
	n_layers:int = 0
	for name in obj_names:
		if bpy.data.objects[name].type in ("MESH","SURFACE"):
			n_layers += 1
	
	svgedit__colours2:list = list(range(n_layers<<2))
	for i in list(range(n_layers)):
		h = ((40*i)%360)/60.0
		s = 1.0 - (((i>>4)%3)*0.3)
		v = 0.8 - (((i>>4)%4)*0.2)
		hsv2rgb(svgedit__colours2, i<<2, h, s, v)
		svgedit__colours2[(i<<2) + 3] = 1.0
	
	layer_indx:int = 0
	for name in obj_names:
		obj = bpy.data.objects[name]
		
		# obj.type is enum in ["MESH", "CURVE", "SURFACE", "META", "FONT", "ARMATURE", "LATTICE", "EMPTY", "GPENCIL", "CAMERA", "LIGHT", "SPEAKER", "LIGHT_PROBE"]
		if obj.type in ("LIGHT","LIGHT_PROBE"):
			# Delete all lights
			obj.hide_render = True # bpy.data.objects.remove(obj, do_unlink=True)
			continue
		if obj.type in ("MESH","SURFACE"):
			# tmp: delete sphere that surrounds the world
			
			'''# Disable particle systems
			for modifier in obj.modifiers:
				modifier.show_render = False
				# OR bpy.ops.object.particle_system_remove() ?
			'''
			
			if len(obj.material_slots) == 0:
				# It should have been textured and thus have a material already
				print(f"WARNING: No material slot assigned to object: {name}")
				continue
			
			obj.active_material_index = 0 # Select the top material slot
			for x in range(len(obj.material_slots)-1):
				try:
					bpy.ops.object.material_slot_remove({'object': obj}) # Delete it
				except RuntimeError as e:
					print(f"{name}: {e.__class__.__name__}: {e}")
			
			orig_mat = obj.material_slots[0].material
			if orig_mat is None:
				# TODO: Figure out why this might be (remember, some of the removed materials might not have been None)
				obj.hide_render = True
				continue
			if orig_mat.use_nodes:
				# There are 3 inputs to the material output node: surface, volume and displacement
				material_output_node = None
				for node in orig_mat.node_tree.nodes:
					if node.type == "OUTPUT_MATERIAL":
						material_output_node = node
						break
				surface_output = material_output_node.inputs[0]
				if (not surface_output.enabled) or (not surface_output.is_linked):
					obj.hide_render = True
					continue
			
			mat = bpy.data.materials.new(name=f"MaterialName{layer_indx}") # NOT bpy.ops.material.new()
			mat.use_nodes = False
			
			mat.diffuse_color = (svgedit__colours2[(layer_indx<<2)], svgedit__colours2[(layer_indx<<2)+1], svgedit__colours2[(layer_indx<<2)+2], 1.0)
			layer_indx += 1
			print("COLOUR", list(mat.diffuse_color))
			
			mat.metallic = 0.0
			mat.specular_intensity = 0.0
			
			try:
				obj.material_slots[0].material = mat
			except AttributeError as e:
				print(f"{name}: {e.__class__.__name__}: {e}")
	
	render_internals(True, outdir, all_camera_names, False)


# multiply 3d coord list by matrix
def np_matmul_coords(coords, matrix, space=None):
	M = (space @ matrix @ space.inverted() if space else matrix).transposed()
	ones = np.ones((coords.shape[0], 1))
	coords4d = np.hstack((coords, ones))
	return np.dot(coords4d, M)[:,:-1]

def get_bounding_box_of_all_mesh_obj_in_scene():
	from mathutils import Vector
	from bpy import context
	import numpy as np
	import itertools
	
	# get the global coordinates of all object bounding box corners    
	coords = np.vstack(
		tuple(
			np_matmul_coords(np.array(o.bound_box), o.matrix_world.copy())
			for o in context.scene.objects
			if o.type == 'MESH'
		)
	)
	
	bfl = coords.min(axis=0) # bottom front left (all the mins)
	tbr = coords.max(axis=0) # top back right
	G = np.array((bfl, tbr)).T
	bounding_box_coords = [i for i in itertools.product(*G)] # the 8 combinations of bfl tbr
	
	return bounding_box_coords


def distance3(a,b,c, x,y,z):
	return ((a-x)**2 + (b-y)**2 + (c-z)**2)**0.5


def add_camera_facing_scene():
	coords = []
	if False:
		# TODO
		bounding_box_coords = get_bounding_box_of_all_mesh_obj_in_scene()
		
		radius = distance3(bounding_box_coords[0],bounding_box_coords[2],bounding_box_coords[4], bounding_box_coords[1],bounding_box_coords[3],bounding_box_coords[5])
		
		z = camera.location[2]
		angle = 2 * math.pi * random.random()
		new_camera_pos = Vector((radius * math.cos(angle), radius * math.sin(angle), z))
	
	bpy.ops.object.camera_add(enter_editmode=False, location=coords)


if __name__ == "__main__":
	import os
	
	# WARNING: Cannot use argparse because all arguments are passed into it including --python [filepath] --background and [blender_filepath]
	'''
	import argparse
	parser = argparse.ArgumentParser()
	parser.add_argument("blend_filepath") # Ignore
	parser.add_argument("--python") # Ignore
	parser.add_argument("--background", action="store_true", default=False) # Ignore
	parser.add_argument("--objname-to-hide-for-segment", action="append", default=[])
	parser.add_argument("--masks-only", action="store_true", default=False)
	parser.add_argument("outdir")
	args = parser.parse_args()'''
	
	import sys
	
	encountered_dashdash:bool = False
	out_rootdir:str = ""
	masks_only:bool = False
	blend_filepath:str = None
	gen = (arg for arg in sys.argv)
	for arg in gen:
		if not encountered_dashdash:
			if arg == "--background":
				blend_filepath = next(gen)
			if arg == "--":
				encountered_dashdash = True
			continue
		if arg == "--masks-only":
			masks_only = True
			continue
		out_rootdir = arg
	
	if out_rootdir.endswith("/"):
		out_rootdir = out_rootdir[:-1]
	outdir:str = f"{out_rootdir}/{re.search('(?:^|/)([^/]+)$', blend_filepath).group(1)}"
	
	if os.path.isdir(outdir):
		pass # raise ValueError(f"WARNING: Directory already exists: {outdir}")
	else:
		os.mkdir(outdir)
	
	all_camera_names:list = [x.name for x in bpy.data.objects if x.type == "CAMERA"]
	obj_names:list = [x.name for x in bpy.data.objects] # Indices may change during the running of the script
	
	if (len(obj_names) == 3) and (obj_names[0] == "Camera") and (obj_names[1] == "Cube") and (obj_names[2] == "Light"):
		raise ValueError("This is using the default blender scene! It hasn't opened the blender file")
	
	if render_internals(False, None, all_camera_names, True) == 0:
		exit()
	
	if len(all_camera_names) == 0:
		add_camera_facing_scene()
		# bpy.context.scene.collection.objects.link(camera)
	
	if not masks_only:
		render_textured(f"{outdir}/img", obj_names, all_camera_names) # NOTE: Do first, before scene attributes are edited
	render_segmentation_fullmask(f"{outdir}/mask", obj_names, all_camera_names)
