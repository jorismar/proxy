//
//  dash.h
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#ifndef __MPEGTStoMPEGDASH__dash__
#define __MPEGTStoMPEGDASH__dash__
#include <vector>
#include <gpac/isomedia.h>
#include <gpac/media_tools.h>
#include <gpac/internal/isomedia_dev.h>
#include <gpac/constants.h>
#include <gpac/scene_engine.h>
#include <gpac/filestreamer.h>
#include <gpac/mpegts.h>
#include <gpac/internal/ietf_dev.h>
#include <gpac/setup.h>
#include "types.h"

static GF_Err MP4_API_IO_Err;

class Dash
{
public:
	GF_ISOFile *file;
	GF_BitStream *stream;
	std::vector <GF_ISOFile> listfragmentos;
	Dash(GF_ISOFile *);
	~Dash();
	void mostradados();
	void fragmenta();
	void recebeudp();
	//GF_DashSegmenterInput *set_input(GF_DashSegmenterInput*, char*, u32*);
};

#endif /* defined(MPEGTS-to-MPEG-DASH__dash) */