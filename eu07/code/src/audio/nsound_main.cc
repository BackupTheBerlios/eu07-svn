#define N_IMPLEMENTS nSound
//-------------------------------------------------------------------
//  nsound_main.cc
//  (C) 2000 RadonLabs GmbH -- A.Weissflog
//-------------------------------------------------------------------
#include "audio/nsound.h"

//-------------------------------------------------------------------
/**
    26-May-00   floh    created
*/
//-------------------------------------------------------------------
nSound::~nSound() 
{
    if (this->fname) {
        n_free((void *)this->fname);
        this->fname = NULL;
    }
}

//-------------------------------------------------------------------
//  EOF
//-------------------------------------------------------------------
