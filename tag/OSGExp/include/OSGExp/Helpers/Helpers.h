/**********************************************************************
 *
 *	FILE:			OSGHelper.h
 *
 *	DESCRIPTION:	Include files for other classes which are going to
 *					use the OSG helper objects.
 *
 *  CREATED BY:		Rune Schmidt jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 06.02.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/


#define OSGHELPER_CLASS_ID	Class_ID(0x2e49ce6c, 0x322f8ff2)
#define BILLBOARD_CLASS_ID	Class_ID(0x37e33d34, 0x411f89e1)
#define LOD_CLASS_ID		Class_ID(0x5c06d205, 0x2264c9e7)
#define SEQUENCE_CLASS_ID	Class_ID(0x5cf57ee1, 0x1fa8de70)
#define STATESET_CLASS_ID	Class_ID(0x5954f484, 0x52216d13)
#define NODEMASK_CLASS_ID	Class_ID(0x12f2305e, 0x21ff5adf)
#define SWITCH_CLASS_ID		Class_ID(0x63c70cf7, 0x622b4eb1)
#define IMPOSTOR_CLASS_ID	Class_ID(0x2e3b4f86, 0x69d16a99)
#define OCCLUDER_CLASS_ID	Class_ID(0x27982df8, 0x4b013d18)

// When new classes are added then remember to changed the DllEntry.
//extern ClassDesc2* GetOSGHelperDesc();
extern ClassDesc2* GetBillboardDesc();
extern ClassDesc2* GetStateSetDesc();
extern ClassDesc2* GetLODDesc();
extern ClassDesc2* GetSequenceDesc();
extern ClassDesc2* GetNodeMaskDesc();
extern ClassDesc2* GetSwitchDesc();
extern ClassDesc2* GetImpostorDesc();
extern ClassDesc2* GetOccluderDesc();

#define PLANE_CLASS_ID Class_ID(0x81f1dfc, 0x77566f65)

enum { 
	bilbo_mode,
	bilbo_axis_x,
	bilbo_axis_y,
	bilbo_axis_z,
	bilbo_normal_x,
	bilbo_normal_y,
	bilbo_normal_z,
	bilbo_nodes
};

enum{
	render_bin_num,
	render_bin_name,
	render_bin_mode,
	glmode_cullface,
	glmode_normalize,
	stateset_nodes
};

#define NUM_LOD_OBJECTS 5
enum{
	lod_center_x,
	lod_center_y,
	lod_center_z,
	lod_min,
	lod_max,
	lod_usepaged,
	lod_paged_filename,
	lod_node
};

#define NUM_SEQUENCE_OBJECTS 5
enum{
	sequence_begin,
	sequence_end,
	sequence_loop,
	sequence_speed,
	sequence_reps,
	sequence_mode,
	sequence_approach,
	sequence_sample_node,
	sequence_frame_per_sample,
	sequence_time,
	sequence_node
};

enum{
	nodemask_mode,
	nodemask_value,
	nodemask_nodes
};

enum{
	switch_nodes
};

enum{
	threshold_value,
	impostor_nodes
};

enum{
	occluder_planes,
};