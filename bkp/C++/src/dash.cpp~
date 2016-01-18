//
//  dash.cpp
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#include "dash.h"
#include "types.h"

Dash::Dash(GF_ISOFile *f){
	this->file = f;
	int i = 0;
	this->file = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/ex.mp4",GF_ISOM_OPEN_READ,NULL);
	mostradados();
	fragmenta();
}

Dash::~Dash(){}

void Dash::fragmenta(){

	double file_duration = 0;
	GF_ISOFile * fragmento;
	GF_DASHSegmenter * seg;
	GF_DashSegmenterInput *dash_inputs = NULL;
	char nomeseg[] = "test";
	u32 n_entrada = 0;
	double dash_duracao = 4;
	int live = 1;
	// duração dos subsegmentos
	double interleaving_time = 4;
	GF_DashProfile dash_profile = GF_DASH_PROFILE_LIVE;
	GF_DashSwitchingMode bitstream_switching_mode = GF_DASH_BSMODE_DEFAULT;
	const char *udpts = NULL;
	const char *grab_ifce = NULL;

	//gra_live_m2ts();
	//dash_inputs = set_dash_input(dash_inputs,"ex.mp4",&n_entrada);
	//gf_dasher_new();
	//fragmento = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/segmento.mp4",GF_ISOM_OPEN_EDIT,NULL);

	//PRINT("Teste"<<fragmento->fileName);
	//if (file_duration < ((Double) gf_isom_get_media_duration(input, i+1)) / tf->TimeScale ) {
	//file_duration = ((Double) gf_isom_get_media_duration(input, i+1)) / tf->TimeScale;
	//gf_isom_start_fragment();
	//aux = gf_isom_new()
	//gf_isom_set_movie_duration(aux,3);
}

void Dash::mostradados(){
	GF_ISOFile *aux;
	aux = this->file;
	//aux = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/ex.mp4",GF_ISOM_OPEN_READ,NULL);
	PRINT("Dados do arquivo mp4");
	PRINT("Nome do arquivo "<<aux->fileName);
	PRINT(""<<aux->brand->size);
	PRINT("Dados do moov");
	PRINT("Tamanho: "<<aux->moov->size<<"\n");
	PRINT("\nDados do mvhd");
	PRINT("\nTamanho: "<<aux->moov->mvhd->size);
	PRINT("CreationTime: "<<aux->moov->mvhd->creationTime);
	PRINT("ModificationTime: "<<aux->moov->mvhd->modificationTime);
	PRINT("TimeScale: "<<aux->moov->mvhd->timeScale);
	PRINT("Duration: "<<aux->moov->mvhd->duration);
	PRINT("NextTrackID: "<<aux->moov->mvhd->nextTrackID);
	PRINT("PreferredRate: "<<aux->moov->mvhd->preferredRate);
    PRINT("PreferredVolume: "<<aux->moov->mvhd->preferredVolume);
    PRINT("Preview Time: "<<aux->moov->mvhd->previewTime);
    PRINT("Preview Duration: "<<aux->moov->mvhd->previewDuration);
    PRINT("Current Time: "<<aux->moov->mvhd->currentTime<<"\n");
    PRINT("Dados do mdat");
    PRINT("Tamanho: "<<aux->mdat->size<<"\n");
    PRINT("Duracao do track "<<gf_isom_get_media_duration(aux,3));
    PRINT("Numero de tracks "<<gf_isom_get_track_count(aux));
    PRINT("TimeScale da trak 1 "<<gf_isom_get_media_timescale(aux,1));
}



