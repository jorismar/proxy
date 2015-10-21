#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include <cstdio>
#include <thread>    

extern "C" {
    #include <gpac/mpegts.h>
    #include <gpac/dvb_mpe.h>
}

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234
/*
int main () {
    GF_M2TS_Demuxer * ts;
    GF_Err err;
    char str[188];

    ts = gf_m2ts_demux_new();
    
    gf_m2ts_process_data(ts, str, 188);
    
    return 0;
}
*/
typedef struct
{
    FILE *ts_file;
    GF_M2TS_Demuxer *ts_demux;
} MPEDemux;

bool isAlive = false;
Buffer * udp_buffer = new Buffer(BUFFER_SIZE, PACKET_SIZE);
Buffer * ts_buffer = new Buffer(BUFFER_SIZE * 7, 188);

static void mpedemux_on_event(GF_M2TS_Demuxer *ts, u32 evt_type, void *param)
{
	MPEDemux *mpedemux= (MPEDemux *) ts->user;
	switch (evt_type) {
	case GF_M2TS_EVT_PAT_FOUND:
		/* called when the first PAT is fully parsed */
		break;
	case GF_M2TS_EVT_SDT_FOUND:
		/* called when the first SDT is fully parsed */
		break;
	case GF_M2TS_EVT_PMT_FOUND:
		/* called when a first PMT is fully parsed */
		break;
	case GF_M2TS_EVT_INT_FOUND:
		/* called when a first INT is fully parsed */
		/* TODO: create socket for each target in the IP platform */
		break;
	case GF_M2TS_EVT_PAT_UPDATE:
	case GF_M2TS_EVT_PMT_UPDATE:
	case GF_M2TS_EVT_SDT_UPDATE:
		/* called when a new version of the table is parsed */
		break;
	case GF_M2TS_EVT_PES_PCK:
		/* called when a PES packet is parsed */
		break;
	case GF_M2TS_EVT_SL_PCK:
		/* called when an MPEG-4 SL-packet is parsed */
		break;
	case GF_M2TS_EVT_IP_DATAGRAM:
		/* called when an IP packet is parsed 
			TODO: send this packet on the right socket */		
		break;
	}
}

static void usage() 
{
    fprintf(stdout, "mpedemux input.ts\n");
}

void tsDemux() {
    GF_Err err;
    int pos = 0;
    MPEDemux *mpedemux;
    DataPacket * udp_packet = new DataPacket(PACKET_SIZE);
    DataPacket * ts_packet = new DataPacket(188);

    GF_SAFEALLOC(mpedemux, MPEDemux);

    mpedemux->ts_demux = gf_m2ts_demux_new();
    mpedemux->ts_demux->on_event = mpedemux_on_event;
    mpedemux->ts_demux->user = mpedemux;
    
    udp_packet = udp_buffer->next();
    
    for(int i = 0; i < 7; i++) {
        pos = ts_packet->copy(udp_packet, pos);
        err = gf_m2ts_process_data(mpedemux->ts_demux, ts_packet->get(), 188);
        
        //std::cout << "Duration: " << mpedemux->ts_demux->duration << std::endl;
        
        //gf_m2ts_print_mpe_info(mpedemux->ts_demux);
        
        if(err != GF_OK)
            std::cout << "ERRO(demux): " << gf_error_to_string(err) << std::endl;
            
        //gf_m2ts_demuxer_play(mpedemux->ts_demux);
        //http://download.tsi.telecom-paristech.fr/gpac/doc/libgpac/structtag__m2ts__demux.html
        
        if(err != GF_OK)
            std::cout << "ERRO(play): " << gf_error_to_string(err) << std::endl;
    }
}

void udpServer() {
    DataPacket * udp_packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * server = new DatagramSocket(UDP_PORT);
    
    server->Bind();

    PRINT("Waiting for connection...");
    
    while(true) {
        server->receive(udp_packet);
        udp_buffer->add(udp_packet);
        
        tsDemux();
    }
}

int main() {
    std::thread svr(udpServer);
    svr.join();

    return 0;
}

