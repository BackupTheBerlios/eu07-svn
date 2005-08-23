/**********************************************************************
 *
 *	FILE:			Switch.cpp
 *
 *	DESCRIPTION:	Class for the OSG Switch helper objects. Through
 *					this helper object the user will be able to make
 *					use of the OSG::Switch class.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 09.04.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/
#include "OSGHelper.h"

extern ParamBlockDesc2 switch_param_blk;


class Switch:public OSGHelper{
	public:
		Switch(TSTR name) : OSGHelper(name){	pblock2 = CreateParameterBlock2(&switch_param_blk,0);};
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		Class_ID ClassID() {return SWITCH_CLASS_ID;}
		RefTargetHandle Clone(RemapDir& remap);
};

class SwitchClassDesc:public OSGHelperClassDesc {
	public:
	void *			Create(BOOL loading = FALSE) { return new Switch(_T("OSG_Switch")); }
	const TCHAR *	ClassName() { return _T("Switch"); }
	Class_ID		ClassID() { return SWITCH_CLASS_ID; }
	const TCHAR*	InternalName() { return _T("OSGSWITCH"); }	// returns fixed parsable name (scripter-visible name)
};


static SwitchClassDesc SwitchDesc;
ClassDesc2* GetSwitchDesc() { return &SwitchDesc; }


enum { 
	switch_params
};


static ParamBlockDesc2 switch_param_blk ( switch_params, _T("switch_params"),  0, &SwitchDesc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF , 
	// rollout
	IDD_SWITCH, IDS_SWITCH, 0, 0, &theHelperProc,
	// switch geometry nodes
	switch_nodes,		_T("NODES"),		TYPE_INODE_TAB,	0,	P_AUTO_UI|P_VARIABLE_SIZE,	IDS_SWITCH_NODES,
		p_sclassID,		GEOMOBJECT_CLASS_ID, // Only add geometry nodes to list.
		p_ui,			TYPE_NODELISTBOX, IDC_LIST,IDC_PICKNODE,0,IDC_SWITCH_REMNODE,
		p_prompt,		IDS_PICK_GEOM_OBJECT,
		end,
	end
	);

void Switch::BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev){	
	this->ip = ip;
	theHelperProc.setInterfacePtr(ip);
	editOb   = this;
	SwitchDesc.BeginEditParams(ip, this, flags, prev);	
}

void Switch::EndEditParams(IObjParam *ip, ULONG flags,Animatable *next){	
	editOb   = NULL;
	this->ip = NULL;
	theHelperProc.setInterfacePtr(NULL);
	SwitchDesc.EndEditParams(ip, this, flags, next);
	ClearAFlag(A_OBJ_CREATING);
}

RefTargetHandle Switch::Clone(RemapDir& remap){
	Switch* newob = new Switch(name);	
	// newob->data = data;
	newob->ReplaceReference(0, pblock2->Clone(remap));
	BaseClone(this, newob, remap);
	return(newob);
}
