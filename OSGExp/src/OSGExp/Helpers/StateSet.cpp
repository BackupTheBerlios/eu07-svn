/**********************************************************************
 *
 *	FILE:			StateSet.cpp
 *
 *	DESCRIPTION:	Class for the StateSet helper objects. Through
 *					this helper object the user will be able to set
 *					all the properties of an osg::StateSet.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 11.02.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/
#include "OSGHelper.h"

extern ParamBlockDesc2 stateset_param_blk;

class StateSet:public OSGHelper{
	public:
		StateSet(TSTR name) : OSGHelper(name){	pblock2 = CreateParameterBlock2(&stateset_param_blk,0);};
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		Class_ID ClassID() {return STATESET_CLASS_ID;}
		RefTargetHandle Clone(RemapDir& remap);
};

class StateSetClassDesc:public OSGHelperClassDesc {
	public:
	void *			Create(BOOL loading = FALSE) { return new StateSet(_T("OSG_StateSet")); }
	const TCHAR *	ClassName() { return _T("StateSet"); }
	Class_ID		ClassID() { return STATESET_CLASS_ID; }
	const TCHAR*	InternalName() { return _T("OSGStateSet"); }	// returns fixed parsable name (scripter-visible name)
};


static StateSetClassDesc StateSetDesc;
ClassDesc2* GetStateSetDesc() { return &StateSetDesc; }


enum { 
	stateset_params
};


static ParamBlockDesc2 stateset_param_blk ( stateset_params, _T("stateset_params"),  0, &StateSetDesc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF , 
	// rollout
	IDD_STATESET, IDS_STATESET, 0, 0, &theHelperProc,
	// render bin number
	render_bin_num,		_T("RenderBinNumber"),	TYPE_INT, 	P_ANIMATABLE, 	IDS_STATESET_BIN_NUMBER, 
		p_default, 		1, 
		p_range, 		-100,100, 
		p_ui, 			TYPE_SPINNER,	EDITTYPE_INT, IDC_STATESET_BIN_EDIT,	IDC_STATESET_BIN_SPIN, 1.0f, 
		end,
	// render bin name
	render_bin_name,	_T("RenderBinName"),	TYPE_STRING, 	P_ANIMATABLE, 	IDS_STATESET_BIN_NAME, 
		p_default, 		_T("RenderBin"), 
		p_ui, 			TYPE_EDITBOX,	IDC_STATESET_BIN_NAME,
		end,
	// render bin mode
	render_bin_mode,	_T("RenderBinMode"),	TYPE_INT, 	P_ANIMATABLE,	IDS_STATESET_RENDER_MODE,
		p_default, 		1, 
		p_range, 		0, 3, 
		p_ui, 			TYPE_RADIO, 4, IDC_RENDER_INHERIT, IDC_RENDER_USE, IDC_RENDER_OVERRIDE, IDC_RENDER_ENCLOSE,
		end,
	// glmode cullface
	glmode_cullface,	_T("GLModeCullFace"),	TYPE_BOOL,	0,	IDS_STATESET_CULLFACE, 	
		p_default, 		TRUE,
		p_ui, 			TYPE_SINGLECHEKBOX, IDC_GLMODE_CULLFACE,
		end,
	// glmode normalize
	glmode_normalize,	_T("GLModeNormalize"),	TYPE_BOOL,	0,	IDS_STATESET_NORMALIZE,
		p_default, 		TRUE,
		p_ui, 			TYPE_SINGLECHEKBOX, IDC_GLMODE_NORMALIZE,
		end, 
	// geometry nodes to use the stateset on
	stateset_nodes,		_T("NODES"),	TYPE_INODE_TAB,	0,	P_AUTO_UI|P_VARIABLE_SIZE,	IDS_STATESET_NODES,
		p_sclassID,		GEOMOBJECT_CLASS_ID, // Only add geometry nodes to list.
		p_ui,			TYPE_NODELISTBOX, IDC_STATESET_NODELIST,IDC_PICKNODE,0,IDC_STATESET_REMOVENODE,
		p_prompt,		IDS_PICK_GEOM_OBJECT,
		end,

	end
	);

void StateSet::BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev){	
	this->ip = ip;
	theHelperProc.setInterfacePtr(ip);
	editOb   = this;
	StateSetDesc.BeginEditParams(ip, this, flags, prev);	
}

void StateSet::EndEditParams(IObjParam *ip, ULONG flags,Animatable *next){	
	editOb   = NULL;
	this->ip = NULL;
	theHelperProc.setInterfacePtr(NULL);
	StateSetDesc.EndEditParams(ip, this, flags, next);
	ClearAFlag(A_OBJ_CREATING);
}

RefTargetHandle StateSet::Clone(RemapDir& remap){
	StateSet* newob = new StateSet(name);	
	// newob->data = data;
	newob->ReplaceReference(0, pblock2->Clone(remap));
	BaseClone(this, newob, remap);
	return(newob);
}