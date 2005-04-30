/**********************************************************************
 *
 *	FILE:			NodeMask.cpp
 *
 *	DESCRIPTION:	Class for the NodeMask helper objects.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 08.04.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/
#include "OSGHelper.h"

extern ParamBlockDesc2 NodeMask_param_blk;

class NodeMask:public OSGHelper{
	public:
		NodeMask(TSTR name) : OSGHelper(name){	pblock2 = CreateParameterBlock2(&NodeMask_param_blk,0);};
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		Class_ID ClassID() {return NODEMASK_CLASS_ID;}
		RefTargetHandle Clone(RemapDir& remap);
};

class NodeMaskClassDesc:public OSGHelperClassDesc {
	public:
	void *			Create(BOOL loading = FALSE) { return new NodeMask(_T("OSG_NodeMask")); }
	const TCHAR *	ClassName() { return _T("NodeMask"); }
	Class_ID		ClassID() { return NODEMASK_CLASS_ID; }
	const TCHAR*	InternalName() { return _T("OSGNodeMask"); }	// returns fixed parsable name (scripter-visible name)
};


static NodeMaskClassDesc NodeMaskDesc;
ClassDesc2* GetNodeMaskDesc() { return &NodeMaskDesc; }


enum { 
	NodeMask_params
};


static ParamBlockDesc2 NodeMask_param_blk ( NodeMask_params, _T("NodeMask_params"),  0, &NodeMaskDesc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF , 
	//rollout
	IDD_NODEMASK, IDS_NODEMASK, 0, 0, &theHelperProc,
	nodemask_mode, 		_T("Loop"),		TYPE_INT, 	P_ANIMATABLE,	IDS_NODEMASK_MODE,
		p_default, 		0, 
		p_range, 		0, 3, 
		p_ui, 			TYPE_RADIO, 4, IDC_NODEMASK_TERRAIN, IDC_NODEMASK_COLLISION, IDC_NODEMASK_NO_COLLISION, IDC_NODEMASK_CUSTOM, 
		end, 
	// custom value
	nodemask_value,	 	_T("NODEMASKVALUES"),	TYPE_STRING, 	0,	IDS_NODEMASK_VALUE,
		p_default, 		"0x000000FF", 
		p_ui, 			TYPE_EDITBOX,	IDC_NODEMASK_EDIT, 
		end,
	// NodeMask nodes
	nodemask_nodes,		_T("NODES"),		TYPE_INODE_TAB,	0,	P_AUTO_UI|P_VARIABLE_SIZE,	IDS_NODEMASK_NODES,
		p_ui,			TYPE_NODELISTBOX, IDC_LIST,IDC_PICKNODE,0,IDC_NODEMASK_REMNODE,
		p_prompt,		IDS_PICK_GEOM_OBJECT,
		end,
	end
	);

void NodeMask::BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev){	
	this->ip = ip;
	theHelperProc.setInterfacePtr(ip);
	editOb   = this;
	NodeMaskDesc.BeginEditParams(ip, this, flags, prev);	
}

void NodeMask::EndEditParams(IObjParam *ip, ULONG flags,Animatable *next){	
	editOb   = NULL;
	this->ip = NULL;
	theHelperProc.setInterfacePtr(NULL);
	NodeMaskDesc.EndEditParams(ip, this, flags, next);
	ClearAFlag(A_OBJ_CREATING);
}

RefTargetHandle NodeMask::Clone(RemapDir& remap){
	NodeMask* newob = new NodeMask(name);	
	// newob->data = data;
	newob->ReplaceReference(0, pblock2->Clone(remap));
	BaseClone(this, newob, remap);
	return(newob);
}


