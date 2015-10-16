//
//  dash.h
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#ifndef __MPEGTStoMPEGDASH__dash__
#define __MPEGTStoMPEGDASH__dash__
#include <gpac/isomedia.h>
#include <gpac/media_tools.h>
#include <gpac/internal/isomedia_dev.h>
#include <gpac/constants.h>
#include "types.h"

class Dash
{
public:
	GF_ISOFile *file;
	GF_BitStream *stream;
	Dash(GF_ISOFile *);
	~Dash();
	void mostradados();
	void fragmenta();
};

#endif /* defined(MPEGTS-to-MPEG-DASH__dash) */