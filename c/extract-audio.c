#include <stdio.h> // for printf

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

enum {
	false,
	true
};

enum {
	ERR_SUCCESS,
	ERR_CANNOT_ALLOC_INPUT_CONTEXT,
	ERR_CANNOT_OPEN_FILE,
	ERR_CANNOT_FIND_STREAM_INFO,
	ERR_CANNOT_FIND_AUDIO,
	ERR_CANNOT_GUESS_FORMAT,
	ERR_CANNOT_ALLOC_OUTPUT_CONTEXT,
	ERR_CANNOT_ALLOCATE_AUDIO_OUTPUT_STREAM,
	ERR_CANNOT_COPY_CODEC_PARAMS,
	ERR_CANNOT_OPEN_OUTPUT_FILE,
	ERR_CANNOT_WRITE_HEADER,
	ERR_INTERLEAVED_WRITE_FAILED
};

#define MAP_ENUM(name) [ name ] = #name

void make_filename(char* const buf,  const char* const basename,  const int codec_id){
	const char* file_ext;
	switch(codec_id){
		case AV_CODEC_ID_OPUS:
			file_ext = ".opus";
			break;
		case AV_CODEC_ID_AAC:
			file_ext = ".m4a";
			break;
		case AV_CODEC_ID_VORBIS:
			file_ext = ".ogg";
			break;
		case AV_CODEC_ID_VP9:
			file_ext = ".webm";
			break;
		default: {
			static const char* map[] = {
				MAP_ENUM(AV_CODEC_ID_NONE),
				MAP_ENUM(AV_CODEC_ID_MPEG1VIDEO),
				MAP_ENUM(AV_CODEC_ID_MPEG2VIDEO),
				MAP_ENUM(AV_CODEC_ID_H261),
				MAP_ENUM(AV_CODEC_ID_H263),
				MAP_ENUM(AV_CODEC_ID_RV10),
				MAP_ENUM(AV_CODEC_ID_RV20),
				MAP_ENUM(AV_CODEC_ID_MJPEG),
				MAP_ENUM(AV_CODEC_ID_MJPEGB),
				MAP_ENUM(AV_CODEC_ID_LJPEG),
				MAP_ENUM(AV_CODEC_ID_SP5X),
				MAP_ENUM(AV_CODEC_ID_JPEGLS),
				MAP_ENUM(AV_CODEC_ID_MPEG4),
				MAP_ENUM(AV_CODEC_ID_RAWVIDEO),
				MAP_ENUM(AV_CODEC_ID_MSMPEG4V1),
				MAP_ENUM(AV_CODEC_ID_MSMPEG4V2),
				MAP_ENUM(AV_CODEC_ID_MSMPEG4V3),
				MAP_ENUM(AV_CODEC_ID_WMV1),
				MAP_ENUM(AV_CODEC_ID_WMV2),
				MAP_ENUM(AV_CODEC_ID_H263P),
				MAP_ENUM(AV_CODEC_ID_H263I),
				MAP_ENUM(AV_CODEC_ID_FLV1),
				MAP_ENUM(AV_CODEC_ID_SVQ1),
				MAP_ENUM(AV_CODEC_ID_SVQ3),
				MAP_ENUM(AV_CODEC_ID_DVVIDEO),
				MAP_ENUM(AV_CODEC_ID_HUFFYUV),
				MAP_ENUM(AV_CODEC_ID_CYUV),
				MAP_ENUM(AV_CODEC_ID_H264),
				MAP_ENUM(AV_CODEC_ID_INDEO3),
				MAP_ENUM(AV_CODEC_ID_VP3),
				MAP_ENUM(AV_CODEC_ID_THEORA),
				MAP_ENUM(AV_CODEC_ID_ASV1),
				MAP_ENUM(AV_CODEC_ID_ASV2),
				MAP_ENUM(AV_CODEC_ID_FFV1),
				MAP_ENUM(AV_CODEC_ID_4XM),
				MAP_ENUM(AV_CODEC_ID_VCR1),
				MAP_ENUM(AV_CODEC_ID_CLJR),
				MAP_ENUM(AV_CODEC_ID_MDEC),
				MAP_ENUM(AV_CODEC_ID_ROQ),
				MAP_ENUM(AV_CODEC_ID_INTERPLAY_VIDEO),
				MAP_ENUM(AV_CODEC_ID_XAN_WC3),
				MAP_ENUM(AV_CODEC_ID_XAN_WC4),
				MAP_ENUM(AV_CODEC_ID_RPZA),
				MAP_ENUM(AV_CODEC_ID_CINEPAK),
				MAP_ENUM(AV_CODEC_ID_WS_VQA),
				MAP_ENUM(AV_CODEC_ID_MSRLE),
				MAP_ENUM(AV_CODEC_ID_MSVIDEO1),
				MAP_ENUM(AV_CODEC_ID_IDCIN),
				MAP_ENUM(AV_CODEC_ID_8BPS),
				MAP_ENUM(AV_CODEC_ID_SMC),
				MAP_ENUM(AV_CODEC_ID_FLIC),
				MAP_ENUM(AV_CODEC_ID_TRUEMOTION1),
				MAP_ENUM(AV_CODEC_ID_VMDVIDEO),
				MAP_ENUM(AV_CODEC_ID_MSZH),
				MAP_ENUM(AV_CODEC_ID_ZLIB),
				MAP_ENUM(AV_CODEC_ID_QTRLE),
				MAP_ENUM(AV_CODEC_ID_TSCC),
				MAP_ENUM(AV_CODEC_ID_ULTI),
				MAP_ENUM(AV_CODEC_ID_QDRAW),
				MAP_ENUM(AV_CODEC_ID_VIXL),
				MAP_ENUM(AV_CODEC_ID_QPEG),
				MAP_ENUM(AV_CODEC_ID_PNG),
				MAP_ENUM(AV_CODEC_ID_PPM),
				MAP_ENUM(AV_CODEC_ID_PBM),
				MAP_ENUM(AV_CODEC_ID_PGM),
				MAP_ENUM(AV_CODEC_ID_PGMYUV),
				MAP_ENUM(AV_CODEC_ID_PAM),
				MAP_ENUM(AV_CODEC_ID_FFVHUFF),
				MAP_ENUM(AV_CODEC_ID_RV30),
				MAP_ENUM(AV_CODEC_ID_RV40),
				MAP_ENUM(AV_CODEC_ID_VC1),
				MAP_ENUM(AV_CODEC_ID_WMV3),
				MAP_ENUM(AV_CODEC_ID_LOCO),
				MAP_ENUM(AV_CODEC_ID_WNV1),
				MAP_ENUM(AV_CODEC_ID_AASC),
				MAP_ENUM(AV_CODEC_ID_INDEO2),
				MAP_ENUM(AV_CODEC_ID_FRAPS),
				MAP_ENUM(AV_CODEC_ID_TRUEMOTION2),
				MAP_ENUM(AV_CODEC_ID_BMP),
				MAP_ENUM(AV_CODEC_ID_CSCD),
				MAP_ENUM(AV_CODEC_ID_MMVIDEO),
				MAP_ENUM(AV_CODEC_ID_ZMBV),
				MAP_ENUM(AV_CODEC_ID_AVS),
				MAP_ENUM(AV_CODEC_ID_SMACKVIDEO),
				MAP_ENUM(AV_CODEC_ID_NUV),
				MAP_ENUM(AV_CODEC_ID_KMVC),
				MAP_ENUM(AV_CODEC_ID_FLASHSV),
				MAP_ENUM(AV_CODEC_ID_CAVS),
				MAP_ENUM(AV_CODEC_ID_JPEG2000),
				MAP_ENUM(AV_CODEC_ID_VMNC),
				MAP_ENUM(AV_CODEC_ID_VP5),
				MAP_ENUM(AV_CODEC_ID_VP6),
				MAP_ENUM(AV_CODEC_ID_VP6F),
				MAP_ENUM(AV_CODEC_ID_TARGA),
				MAP_ENUM(AV_CODEC_ID_DSICINVIDEO),
				MAP_ENUM(AV_CODEC_ID_TIERTEXSEQVIDEO),
				MAP_ENUM(AV_CODEC_ID_TIFF),
				MAP_ENUM(AV_CODEC_ID_GIF),
				MAP_ENUM(AV_CODEC_ID_DXA),
				MAP_ENUM(AV_CODEC_ID_DNXHD),
				MAP_ENUM(AV_CODEC_ID_THP),
				MAP_ENUM(AV_CODEC_ID_SGI),
				MAP_ENUM(AV_CODEC_ID_C93),
				MAP_ENUM(AV_CODEC_ID_BETHSOFTVID),
				MAP_ENUM(AV_CODEC_ID_PTX),
				MAP_ENUM(AV_CODEC_ID_TXD),
				MAP_ENUM(AV_CODEC_ID_VP6A),
				MAP_ENUM(AV_CODEC_ID_AMV),
				MAP_ENUM(AV_CODEC_ID_VB),
				MAP_ENUM(AV_CODEC_ID_PCX),
				MAP_ENUM(AV_CODEC_ID_SUNRAST),
				MAP_ENUM(AV_CODEC_ID_INDEO4),
				MAP_ENUM(AV_CODEC_ID_INDEO5),
				MAP_ENUM(AV_CODEC_ID_MIMIC),
				MAP_ENUM(AV_CODEC_ID_RL2),
				MAP_ENUM(AV_CODEC_ID_ESCAPE124),
				MAP_ENUM(AV_CODEC_ID_DIRAC),
				MAP_ENUM(AV_CODEC_ID_BFI),
				MAP_ENUM(AV_CODEC_ID_CMV),
				MAP_ENUM(AV_CODEC_ID_MOTIONPIXELS),
				MAP_ENUM(AV_CODEC_ID_TGV),
				MAP_ENUM(AV_CODEC_ID_TGQ),
				MAP_ENUM(AV_CODEC_ID_TQI),
				MAP_ENUM(AV_CODEC_ID_AURA),
				MAP_ENUM(AV_CODEC_ID_AURA2),
				MAP_ENUM(AV_CODEC_ID_V210X),
				MAP_ENUM(AV_CODEC_ID_TMV),
				MAP_ENUM(AV_CODEC_ID_V210),
				MAP_ENUM(AV_CODEC_ID_DPX),
				MAP_ENUM(AV_CODEC_ID_MAD),
				MAP_ENUM(AV_CODEC_ID_FRWU),
				MAP_ENUM(AV_CODEC_ID_FLASHSV2),
				MAP_ENUM(AV_CODEC_ID_CDGRAPHICS),
				MAP_ENUM(AV_CODEC_ID_R210),
				MAP_ENUM(AV_CODEC_ID_ANM),
				MAP_ENUM(AV_CODEC_ID_BINKVIDEO),
				MAP_ENUM(AV_CODEC_ID_IFF_ILBM),
				MAP_ENUM(AV_CODEC_ID_KGV1),
				MAP_ENUM(AV_CODEC_ID_YOP),
				MAP_ENUM(AV_CODEC_ID_VP8),
				MAP_ENUM(AV_CODEC_ID_PICTOR),
				MAP_ENUM(AV_CODEC_ID_ANSI),
				MAP_ENUM(AV_CODEC_ID_A64_MULTI),
				MAP_ENUM(AV_CODEC_ID_A64_MULTI5),
				MAP_ENUM(AV_CODEC_ID_R10K),
				MAP_ENUM(AV_CODEC_ID_MXPEG),
				MAP_ENUM(AV_CODEC_ID_LAGARITH),
				MAP_ENUM(AV_CODEC_ID_PRORES),
				MAP_ENUM(AV_CODEC_ID_JV),
				MAP_ENUM(AV_CODEC_ID_DFA),
				MAP_ENUM(AV_CODEC_ID_WMV3IMAGE),
				MAP_ENUM(AV_CODEC_ID_VC1IMAGE),
				MAP_ENUM(AV_CODEC_ID_UTVIDEO),
				MAP_ENUM(AV_CODEC_ID_BMV_VIDEO),
				MAP_ENUM(AV_CODEC_ID_VBLE),
				MAP_ENUM(AV_CODEC_ID_DXTORY),
				MAP_ENUM(AV_CODEC_ID_V410),
				MAP_ENUM(AV_CODEC_ID_XWD),
				MAP_ENUM(AV_CODEC_ID_CDXL),
				MAP_ENUM(AV_CODEC_ID_XBM),
				MAP_ENUM(AV_CODEC_ID_ZEROCODEC),
				MAP_ENUM(AV_CODEC_ID_MSS1),
				MAP_ENUM(AV_CODEC_ID_MSA1),
				MAP_ENUM(AV_CODEC_ID_TSCC2),
				MAP_ENUM(AV_CODEC_ID_MTS2),
				MAP_ENUM(AV_CODEC_ID_CLLC),
				MAP_ENUM(AV_CODEC_ID_MSS2),
				MAP_ENUM(AV_CODEC_ID_VP9),
				MAP_ENUM(AV_CODEC_ID_AIC),
				MAP_ENUM(AV_CODEC_ID_ESCAPE130),
				MAP_ENUM(AV_CODEC_ID_G2M),
				MAP_ENUM(AV_CODEC_ID_WEBP),
				MAP_ENUM(AV_CODEC_ID_HNM4_VIDEO),
				MAP_ENUM(AV_CODEC_ID_HEVC),
				MAP_ENUM(AV_CODEC_ID_FIC),
				MAP_ENUM(AV_CODEC_ID_ALIAS_PIX),
				MAP_ENUM(AV_CODEC_ID_BRENDER_PIX),
				MAP_ENUM(AV_CODEC_ID_PAF_VIDEO),
				MAP_ENUM(AV_CODEC_ID_EXR),
				MAP_ENUM(AV_CODEC_ID_VP7),
				MAP_ENUM(AV_CODEC_ID_SANM),
				MAP_ENUM(AV_CODEC_ID_SGIRLE),
				MAP_ENUM(AV_CODEC_ID_MVC1),
				MAP_ENUM(AV_CODEC_ID_MVC2),
				MAP_ENUM(AV_CODEC_ID_HQX),
				MAP_ENUM(AV_CODEC_ID_TDSC),
				MAP_ENUM(AV_CODEC_ID_HQ_HQA),
				MAP_ENUM(AV_CODEC_ID_HAP),
				MAP_ENUM(AV_CODEC_ID_DDS),
				MAP_ENUM(AV_CODEC_ID_DXV),
				MAP_ENUM(AV_CODEC_ID_SCREENPRESSO),
				MAP_ENUM(AV_CODEC_ID_RSCC),
				MAP_ENUM(AV_CODEC_ID_Y41P),
				MAP_ENUM(AV_CODEC_ID_AVRP),
				MAP_ENUM(AV_CODEC_ID_012V),
				MAP_ENUM(AV_CODEC_ID_AVUI),
				MAP_ENUM(AV_CODEC_ID_AYUV),
				MAP_ENUM(AV_CODEC_ID_TARGA_Y216),
				MAP_ENUM(AV_CODEC_ID_V308),
				MAP_ENUM(AV_CODEC_ID_V408),
				MAP_ENUM(AV_CODEC_ID_YUV4),
				MAP_ENUM(AV_CODEC_ID_AVRN),
				MAP_ENUM(AV_CODEC_ID_CPIA),
				MAP_ENUM(AV_CODEC_ID_XFACE),
				MAP_ENUM(AV_CODEC_ID_SNOW),
				MAP_ENUM(AV_CODEC_ID_SMVJPEG),
				MAP_ENUM(AV_CODEC_ID_APNG),
				MAP_ENUM(AV_CODEC_ID_DAALA),
				MAP_ENUM(AV_CODEC_ID_CFHD),
				MAP_ENUM(AV_CODEC_ID_TRUEMOTION2RT),
				MAP_ENUM(AV_CODEC_ID_M101),
				MAP_ENUM(AV_CODEC_ID_MAGICYUV),
				MAP_ENUM(AV_CODEC_ID_SHEERVIDEO),
				MAP_ENUM(AV_CODEC_ID_YLC),
				MAP_ENUM(AV_CODEC_ID_PSD),
				MAP_ENUM(AV_CODEC_ID_PIXLET),
				MAP_ENUM(AV_CODEC_ID_SPEEDHQ),
				MAP_ENUM(AV_CODEC_ID_FMVC),
				MAP_ENUM(AV_CODEC_ID_SCPR),
				MAP_ENUM(AV_CODEC_ID_CLEARVIDEO),
				MAP_ENUM(AV_CODEC_ID_XPM),
				MAP_ENUM(AV_CODEC_ID_AV1),
				MAP_ENUM(AV_CODEC_ID_BITPACKED),
				MAP_ENUM(AV_CODEC_ID_MSCC),
				MAP_ENUM(AV_CODEC_ID_SRGC),
				MAP_ENUM(AV_CODEC_ID_SVG),
				MAP_ENUM(AV_CODEC_ID_GDV),
				MAP_ENUM(AV_CODEC_ID_FITS),
				MAP_ENUM(AV_CODEC_ID_FIRST_AUDIO),
				MAP_ENUM(AV_CODEC_ID_PCM_S16LE),
				MAP_ENUM(AV_CODEC_ID_PCM_S16BE),
				MAP_ENUM(AV_CODEC_ID_PCM_U16LE),
				MAP_ENUM(AV_CODEC_ID_PCM_U16BE),
				MAP_ENUM(AV_CODEC_ID_PCM_S8),
				MAP_ENUM(AV_CODEC_ID_PCM_U8),
				MAP_ENUM(AV_CODEC_ID_PCM_MULAW),
				MAP_ENUM(AV_CODEC_ID_PCM_ALAW),
				MAP_ENUM(AV_CODEC_ID_PCM_S32LE),
				MAP_ENUM(AV_CODEC_ID_PCM_S32BE),
				MAP_ENUM(AV_CODEC_ID_PCM_U32LE),
				MAP_ENUM(AV_CODEC_ID_PCM_U32BE),
				MAP_ENUM(AV_CODEC_ID_PCM_S24LE),
				MAP_ENUM(AV_CODEC_ID_PCM_S24BE),
				MAP_ENUM(AV_CODEC_ID_PCM_U24LE),
				MAP_ENUM(AV_CODEC_ID_PCM_U24BE),
				MAP_ENUM(AV_CODEC_ID_PCM_S24DAUD),
				MAP_ENUM(AV_CODEC_ID_PCM_ZORK),
				MAP_ENUM(AV_CODEC_ID_PCM_S16LE_PLANAR),
				MAP_ENUM(AV_CODEC_ID_PCM_DVD),
				MAP_ENUM(AV_CODEC_ID_PCM_F32BE),
				MAP_ENUM(AV_CODEC_ID_PCM_F32LE),
				MAP_ENUM(AV_CODEC_ID_PCM_F64BE),
				MAP_ENUM(AV_CODEC_ID_PCM_F64LE),
				MAP_ENUM(AV_CODEC_ID_PCM_BLURAY),
				MAP_ENUM(AV_CODEC_ID_PCM_LXF),
				MAP_ENUM(AV_CODEC_ID_S302M),
				MAP_ENUM(AV_CODEC_ID_PCM_S8_PLANAR),
				MAP_ENUM(AV_CODEC_ID_PCM_S24LE_PLANAR),
				MAP_ENUM(AV_CODEC_ID_PCM_S32LE_PLANAR),
				MAP_ENUM(AV_CODEC_ID_PCM_S16BE_PLANAR),
				MAP_ENUM(AV_CODEC_ID_PCM_S64LE),
				MAP_ENUM(AV_CODEC_ID_PCM_S64BE),
				MAP_ENUM(AV_CODEC_ID_PCM_F16LE),
				MAP_ENUM(AV_CODEC_ID_PCM_F24LE),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_QT),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_WAV),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_DK3),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_DK4),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_WS),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_SMJPEG),
				MAP_ENUM(AV_CODEC_ID_ADPCM_MS),
				MAP_ENUM(AV_CODEC_ID_ADPCM_4XM),
				MAP_ENUM(AV_CODEC_ID_ADPCM_XA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_ADX),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_G726),
				MAP_ENUM(AV_CODEC_ID_ADPCM_CT),
				MAP_ENUM(AV_CODEC_ID_ADPCM_SWF),
				MAP_ENUM(AV_CODEC_ID_ADPCM_YAMAHA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_SBPRO_4),
				MAP_ENUM(AV_CODEC_ID_ADPCM_SBPRO_3),
				MAP_ENUM(AV_CODEC_ID_ADPCM_SBPRO_2),
				MAP_ENUM(AV_CODEC_ID_ADPCM_THP),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_AMV),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA_R1),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA_R3),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA_R2),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_EA_SEAD),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_EA_EACS),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA_XAS),
				MAP_ENUM(AV_CODEC_ID_ADPCM_EA_MAXIS_XA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_ISS),
				MAP_ENUM(AV_CODEC_ID_ADPCM_G722),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_APC),
				MAP_ENUM(AV_CODEC_ID_ADPCM_VIMA),
				//MAP_ENUM(AV_CODEC_ID_VIMA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_AFC),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_OKI),
				MAP_ENUM(AV_CODEC_ID_ADPCM_DTK),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_RAD),
				MAP_ENUM(AV_CODEC_ID_ADPCM_G726LE),
				MAP_ENUM(AV_CODEC_ID_ADPCM_THP_LE),
				MAP_ENUM(AV_CODEC_ID_ADPCM_PSX),
				MAP_ENUM(AV_CODEC_ID_ADPCM_AICA),
				MAP_ENUM(AV_CODEC_ID_ADPCM_IMA_DAT4),
				MAP_ENUM(AV_CODEC_ID_ADPCM_MTAF),
				MAP_ENUM(AV_CODEC_ID_AMR_NB),
				MAP_ENUM(AV_CODEC_ID_AMR_WB),
				MAP_ENUM(AV_CODEC_ID_RA_144),
				MAP_ENUM(AV_CODEC_ID_RA_288),
				MAP_ENUM(AV_CODEC_ID_ROQ_DPCM),
				MAP_ENUM(AV_CODEC_ID_INTERPLAY_DPCM),
				MAP_ENUM(AV_CODEC_ID_XAN_DPCM),
				MAP_ENUM(AV_CODEC_ID_SOL_DPCM),
				MAP_ENUM(AV_CODEC_ID_SDX2_DPCM),
				MAP_ENUM(AV_CODEC_ID_GREMLIN_DPCM),
				MAP_ENUM(AV_CODEC_ID_MP2),
				MAP_ENUM(AV_CODEC_ID_MP3),
				MAP_ENUM(AV_CODEC_ID_AAC),
				MAP_ENUM(AV_CODEC_ID_AC3),
				MAP_ENUM(AV_CODEC_ID_DTS),
				MAP_ENUM(AV_CODEC_ID_VORBIS),
				MAP_ENUM(AV_CODEC_ID_DVAUDIO),
				MAP_ENUM(AV_CODEC_ID_WMAV1),
				MAP_ENUM(AV_CODEC_ID_WMAV2),
				MAP_ENUM(AV_CODEC_ID_MACE3),
				MAP_ENUM(AV_CODEC_ID_MACE6),
				MAP_ENUM(AV_CODEC_ID_VMDAUDIO),
				MAP_ENUM(AV_CODEC_ID_FLAC),
				MAP_ENUM(AV_CODEC_ID_MP3ADU),
				MAP_ENUM(AV_CODEC_ID_MP3ON4),
				MAP_ENUM(AV_CODEC_ID_SHORTEN),
				MAP_ENUM(AV_CODEC_ID_ALAC),
				MAP_ENUM(AV_CODEC_ID_WESTWOOD_SND1),
				MAP_ENUM(AV_CODEC_ID_GSM),
				MAP_ENUM(AV_CODEC_ID_QDM2),
				MAP_ENUM(AV_CODEC_ID_COOK),
				MAP_ENUM(AV_CODEC_ID_TRUESPEECH),
				MAP_ENUM(AV_CODEC_ID_TTA),
				MAP_ENUM(AV_CODEC_ID_SMACKAUDIO),
				MAP_ENUM(AV_CODEC_ID_QCELP),
				MAP_ENUM(AV_CODEC_ID_WAVPACK),
				MAP_ENUM(AV_CODEC_ID_DSICINAUDIO),
				MAP_ENUM(AV_CODEC_ID_IMC),
				MAP_ENUM(AV_CODEC_ID_MUSEPACK7),
				MAP_ENUM(AV_CODEC_ID_MLP),
				MAP_ENUM(AV_CODEC_ID_GSM_MS),
				MAP_ENUM(AV_CODEC_ID_ATRAC3),
				//MAP_ENUM(AV_CODEC_ID_VOXWARE),
				MAP_ENUM(AV_CODEC_ID_APE),
				MAP_ENUM(AV_CODEC_ID_NELLYMOSER),
				MAP_ENUM(AV_CODEC_ID_MUSEPACK8),
				MAP_ENUM(AV_CODEC_ID_SPEEX),
				MAP_ENUM(AV_CODEC_ID_WMAVOICE),
				MAP_ENUM(AV_CODEC_ID_WMAPRO),
				MAP_ENUM(AV_CODEC_ID_WMALOSSLESS),
				MAP_ENUM(AV_CODEC_ID_ATRAC3P),
				MAP_ENUM(AV_CODEC_ID_EAC3),
				MAP_ENUM(AV_CODEC_ID_SIPR),
				MAP_ENUM(AV_CODEC_ID_MP1),
				MAP_ENUM(AV_CODEC_ID_TWINVQ),
				MAP_ENUM(AV_CODEC_ID_TRUEHD),
				MAP_ENUM(AV_CODEC_ID_MP4ALS),
				MAP_ENUM(AV_CODEC_ID_ATRAC1),
				MAP_ENUM(AV_CODEC_ID_BINKAUDIO_RDFT),
				MAP_ENUM(AV_CODEC_ID_BINKAUDIO_DCT),
				MAP_ENUM(AV_CODEC_ID_AAC_LATM),
				MAP_ENUM(AV_CODEC_ID_QDMC),
				MAP_ENUM(AV_CODEC_ID_CELT),
				MAP_ENUM(AV_CODEC_ID_G723_1),
				MAP_ENUM(AV_CODEC_ID_G729),
				MAP_ENUM(AV_CODEC_ID_8SVX_EXP),
				MAP_ENUM(AV_CODEC_ID_8SVX_FIB),
				MAP_ENUM(AV_CODEC_ID_BMV_AUDIO),
				MAP_ENUM(AV_CODEC_ID_RALF),
				MAP_ENUM(AV_CODEC_ID_IAC),
				MAP_ENUM(AV_CODEC_ID_ILBC),
				MAP_ENUM(AV_CODEC_ID_OPUS),
				MAP_ENUM(AV_CODEC_ID_COMFORT_NOISE),
				MAP_ENUM(AV_CODEC_ID_TAK),
				MAP_ENUM(AV_CODEC_ID_METASOUND),
				MAP_ENUM(AV_CODEC_ID_PAF_AUDIO),
				MAP_ENUM(AV_CODEC_ID_ON2AVC),
				MAP_ENUM(AV_CODEC_ID_DSS_SP),
				MAP_ENUM(AV_CODEC_ID_FFWAVESYNTH),
				MAP_ENUM(AV_CODEC_ID_SONIC),
				MAP_ENUM(AV_CODEC_ID_SONIC_LS),
				MAP_ENUM(AV_CODEC_ID_EVRC),
				MAP_ENUM(AV_CODEC_ID_SMV),
				MAP_ENUM(AV_CODEC_ID_DSD_LSBF),
				MAP_ENUM(AV_CODEC_ID_DSD_MSBF),
				MAP_ENUM(AV_CODEC_ID_DSD_LSBF_PLANAR),
				MAP_ENUM(AV_CODEC_ID_DSD_MSBF_PLANAR),
				MAP_ENUM(AV_CODEC_ID_4GV),
				MAP_ENUM(AV_CODEC_ID_INTERPLAY_ACM),
				MAP_ENUM(AV_CODEC_ID_XMA1),
				MAP_ENUM(AV_CODEC_ID_XMA2),
				MAP_ENUM(AV_CODEC_ID_DST),
				MAP_ENUM(AV_CODEC_ID_ATRAC3AL),
				MAP_ENUM(AV_CODEC_ID_ATRAC3PAL),
				MAP_ENUM(AV_CODEC_ID_DOLBY_E),
				MAP_ENUM(AV_CODEC_ID_FIRST_SUBTITLE),
				MAP_ENUM(AV_CODEC_ID_DVD_SUBTITLE),
				MAP_ENUM(AV_CODEC_ID_DVB_SUBTITLE),
				MAP_ENUM(AV_CODEC_ID_TEXT),
				MAP_ENUM(AV_CODEC_ID_XSUB),
				MAP_ENUM(AV_CODEC_ID_SSA),
				MAP_ENUM(AV_CODEC_ID_MOV_TEXT),
				MAP_ENUM(AV_CODEC_ID_HDMV_PGS_SUBTITLE),
				MAP_ENUM(AV_CODEC_ID_DVB_TELETEXT),
				MAP_ENUM(AV_CODEC_ID_SRT),
				MAP_ENUM(AV_CODEC_ID_MICRODVD),
				MAP_ENUM(AV_CODEC_ID_EIA_608),
				MAP_ENUM(AV_CODEC_ID_JACOSUB),
				MAP_ENUM(AV_CODEC_ID_SAMI),
				MAP_ENUM(AV_CODEC_ID_REALTEXT),
				MAP_ENUM(AV_CODEC_ID_STL),
				MAP_ENUM(AV_CODEC_ID_SUBVIEWER1),
				MAP_ENUM(AV_CODEC_ID_SUBVIEWER),
				MAP_ENUM(AV_CODEC_ID_SUBRIP),
				MAP_ENUM(AV_CODEC_ID_WEBVTT),
				MAP_ENUM(AV_CODEC_ID_MPL2),
				MAP_ENUM(AV_CODEC_ID_VPLAYER),
				MAP_ENUM(AV_CODEC_ID_PJS),
				MAP_ENUM(AV_CODEC_ID_ASS),
				MAP_ENUM(AV_CODEC_ID_HDMV_TEXT_SUBTITLE),
				MAP_ENUM(AV_CODEC_ID_FIRST_UNKNOWN),
				MAP_ENUM(AV_CODEC_ID_TTF),
				MAP_ENUM(AV_CODEC_ID_SCTE_35),
				MAP_ENUM(AV_CODEC_ID_BINTEXT),
				MAP_ENUM(AV_CODEC_ID_XBIN),
				MAP_ENUM(AV_CODEC_ID_IDF),
				MAP_ENUM(AV_CODEC_ID_OTF),
				MAP_ENUM(AV_CODEC_ID_SMPTE_KLV),
				MAP_ENUM(AV_CODEC_ID_DVD_NAV),
				MAP_ENUM(AV_CODEC_ID_TIMED_ID3),
				MAP_ENUM(AV_CODEC_ID_BIN_DATA),
				MAP_ENUM(AV_CODEC_ID_PROBE),
				MAP_ENUM(AV_CODEC_ID_MPEG2TS),
				MAP_ENUM(AV_CODEC_ID_MPEG4SYSTEMS),
				MAP_ENUM(AV_CODEC_ID_FFMETADATA),
				MAP_ENUM(AV_CODEC_ID_WRAPPED_AVFRAME)
			};
			
			printf("Not implemented switch for codec_id: %d (%s)\n", codec_id, map[codec_id]);
			
			abort();
		}
	}
	const size_t basename_len = strlen(basename);
	memcpy(buf, basename, basename_len);
	memcpy(buf + basename_len,  file_ext,  strlen(file_ext) + 1);
}


int extract_audio(const char* const output_filepath_basename,  const char* const input_filepath){
	AVFormatContext* input_fmt_ctx = avformat_alloc_context();
	if (!input_fmt_ctx)
		return ERR_CANNOT_ALLOC_INPUT_CONTEXT;
	
	if (avformat_open_input(&input_fmt_ctx, input_filepath, NULL, NULL))
		return ERR_CANNOT_OPEN_FILE;
	
	if (avformat_find_stream_info(input_fmt_ctx, NULL))
		return ERR_CANNOT_FIND_STREAM_INFO;
	
	///AVCodec* output_audio_codec;
	AVStream* input_audio;
	for (int i = 0;  i < input_fmt_ctx->nb_streams;  i++){
		input_audio = input_fmt_ctx->streams[i];

		// if output context has audio codec support and current input stream is audio
		if (input_audio == NULL  ||  input_audio->codecpar->codec_type != AVMEDIA_TYPE_AUDIO)
			continue;
		
		break;
	}
	if (input_audio == NULL){
		return ERR_CANNOT_FIND_AUDIO;
	}
	
	static char output_filepath[4096];
	make_filename(output_filepath, output_filepath_basename, input_audio->codecpar->codec_id);
	AVOutputFormat* const outfmt = av_guess_format(NULL, output_filepath, NULL);
	if (outfmt == NULL)
		return ERR_CANNOT_GUESS_FORMAT;

	AVFormatContext* output_fmt_ctx;
	avformat_alloc_output_context2(&output_fmt_ctx, outfmt, NULL, output_filepath);
	if (!output_fmt_ctx)
		return ERR_CANNOT_ALLOC_OUTPUT_CONTEXT;
	
	AVStream* output_audio = avformat_new_stream(output_fmt_ctx, avcodec_find_decoder(input_audio->codecpar->codec_id)); ///output_audio_codec);
	if (NULL == output_audio)
		return ERR_CANNOT_ALLOCATE_AUDIO_OUTPUT_STREAM;
	
	if (avcodec_parameters_copy(output_audio->codecpar, input_audio->codecpar))
		return ERR_CANNOT_COPY_CODEC_PARAMS;
	
	output_audio->duration = input_audio->duration;
	output_audio->time_base.num = input_audio->time_base.num;
	output_audio->time_base.den = input_audio->time_base.den;
	
	av_dump_format(output_fmt_ctx, 0, output_filepath, 1); // print
	
	if (avio_open(&output_fmt_ctx->pb, output_filepath, AVIO_FLAG_WRITE))
		return ERR_CANNOT_OPEN_OUTPUT_FILE;
	
	if (avformat_write_header(output_fmt_ctx, NULL))
		return ERR_CANNOT_WRITE_HEADER;
	
	
	AVPacket pkt;
	while(av_read_frame(input_fmt_ctx, &pkt) == 0){
		if (pkt.stream_index != input_audio->index)
			continue;
		pkt.stream_index = output_audio->index;
		pkt.pts = av_rescale_q(pkt.pts, input_audio->time_base, output_audio->time_base);
		pkt.dts = av_rescale_q(pkt.dts, input_audio->time_base, output_audio->time_base);
		if (av_interleaved_write_frame(output_fmt_ctx, &pkt) != 0){
			return ERR_INTERLEAVED_WRITE_FAILED;
		}
	}
	
	av_write_trailer(output_fmt_ctx);
	avformat_close_input(&input_fmt_ctx);
	avio_closep(&output_fmt_ctx->pb);
	avformat_free_context(output_fmt_ctx);
	
	return 0;
}


int main(int argc, char *argv[]) {
	const char* const output_basename = argv[1];
	const char* const input_filepath = argv[2];
	static char output_filepath[1024];
	
	return extract_audio(output_basename, input_filepath);
}
