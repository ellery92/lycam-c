#ifndef LYU_CAMERA_H
#define LYU_CAMERA_H

#include <stdint.h>

/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
#define G_BEGIN_DECLS  extern "C" {
#define G_END_DECLS    }
#else
#define G_BEGIN_DECLS
#define G_END_DECLS
#endif

G_BEGIN_DECLS

/**
 * LyuAuto:
 * @LYU_AUTO_OFF: manual setting
 * @LYU_AUTO_ONCE: automatic setting done once, then returns to manual
 * @LYU_AUTO_CONTINUOUS: setting is adjusted continuously
 */

typedef enum {
              LYU_AUTO_OFF,
              LYU_AUTO_ONCE,
              LYU_AUTO_CONTINUOUS
} LyuAuto;

/**
 * LyuAcquisitionMode:
 * @LYU_ACQUISITION_MODE_CONTINUOUS: continuous acquisition
 * @LYU_ACQUISITION_MODE_SINGLE_FRAME: only one frame will be acquired
 * @LYU_ACQUISITION_MODE_MULTI_FRAME: multiple frames will be acquired
 */

typedef enum {
              LYU_ACQUISITION_MODE_CONTINUOUS,
              LYU_ACQUISITION_MODE_SINGLE_FRAME,
              LYU_ACQUISITION_MODE_MULTI_FRAME
} LyuAcquisitionMode;

const char * 		lyu_acquisition_mode_to_string 		(LyuAcquisitionMode value);
LyuAcquisitionMode 	lyu_acquisition_mode_from_string	(const char *string);

typedef uint32_t LyuPixelFormat;

#define LYU_PIXEL_FORMAT_BIT_PER_PIXEL(pixel_format) (((pixel_format) >> 16) & 0xff)

/* Grey pixel formats */

#define	LYU_PIXEL_FORMAT_MONO_8			0x01080001
#define	LYU_PIXEL_FORMAT_MONO_8_SIGNED		0x01080002

#define	LYU_PIXEL_FORMAT_MONO_10		0x01100003
#define LYU_PIXEL_FORMAT_MONO_10_PACKED		0x010c0004

#define LYU_PIXEL_FORMAT_MONO_12		0x01100005
#define LYU_PIXEL_FORMAT_MONO_12_PACKED		0x010c0006

#define LYU_PIXEL_FORMAT_MONO_14		0x01100025 /* https://bugzilla.gnome.org/show_bug.cgi?id=655131 */

#define LYU_PIXEL_FORMAT_MONO_16		0x01100007

#define LYU_PIXEL_FORMAT_BAYER_GR_8		0x01080008
#define LYU_PIXEL_FORMAT_BAYER_RG_8		0x01080009
#define LYU_PIXEL_FORMAT_BAYER_GB_8		0x0108000a
#define LYU_PIXEL_FORMAT_BAYER_BG_8		0x0108000b

#define LYU_PIXEL_FORMAT_BAYER_GR_10		0x0110000c
#define LYU_PIXEL_FORMAT_BAYER_RG_10		0x0110000d
#define LYU_PIXEL_FORMAT_BAYER_GB_10		0x0110000e
#define LYU_PIXEL_FORMAT_BAYER_BG_10		0x0110000f

#define LYU_PIXEL_FORMAT_BAYER_GR_12		0x01100010
#define LYU_PIXEL_FORMAT_BAYER_RG_12		0x01100011
#define LYU_PIXEL_FORMAT_BAYER_GB_12		0x01100012
#define LYU_PIXEL_FORMAT_BAYER_BG_12		0x01100013

#define LYU_PIXEL_FORMAT_BAYER_GR_12_PACKED	0x010c002a
#define LYU_PIXEL_FORMAT_BAYER_RG_12_PACKED	0x010c002b
#define LYU_PIXEL_FORMAT_BAYER_GB_12_PACKED	0x010c002c
#define LYU_PIXEL_FORMAT_BAYER_BG_12_PACKED	0x010c002d

#define LYU_PIXEL_FORMAT_BAYER_GR_16		0x0110002e
#define LYU_PIXEL_FORMAT_BAYER_RG_16		0x0110002f
#define LYU_PIXEL_FORMAT_BAYER_GB_16		0x01100030
#define LYU_PIXEL_FORMAT_BAYER_BG_16		0x01100031

/* Color pixel formats */

#define LYU_PIXEL_FORMAT_RGB_8_PACKED		0x02180014
#define LYU_PIXEL_FORMAT_BGR_8_PACKED		0x02180015

#define LYU_PIXEL_FORMAT_RGBA_8_PACKED		0x02200016
#define LYU_PIXEL_FORMAT_BGRA_8_PACKED		0x02200017

#define LYU_PIXEL_FORMAT_RGB_10_PACKED		0x02300018
#define LYU_PIXEL_FORMAT_BGR_10_PACKED		0x02300019

#define LYU_PIXEL_FORMAT_RGB_12_PACKED		0x0230001a
#define LYU_PIXEL_FORMAT_BGR_12_PACKED		0x0230001b

#define LYU_PIXEL_FORMAT_YUV_411_PACKED		0x020c001e
#define LYU_PIXEL_FORMAT_YUV_422_PACKED		0x0210001f
#define LYU_PIXEL_FORMAT_YUV_444_PACKED		0x02180020

#define LYU_PIXEL_FORMAT_RGB_8_PLANAR		0x02180021
#define LYU_PIXEL_FORMAT_RGB_10_PLANAR		0x02300022
#define LYU_PIXEL_FORMAT_RGB_12_PLANAR		0x02300023
#define LYU_PIXEL_FORMAT_RGB_16_PLANAR		0x02300024

#define LYU_PIXEL_FORMAT_YUV_422_YUYV_PACKED 	0x02100032

/* Custom */

#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_GR_12_PACKED  	0x810c0001
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_RG_12_PACKED  	0x810c0002
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_GB_12_PACKED  	0x810c0003
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_BG_12_PACKED  	0x810c0004
#define LYU_PIXEL_FORMAT_CUSTOM_YUV_422_YUYV_PACKED 	0x82100005
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_GR_16		0x81100006
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_RG_16		0x81100007
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_GB_16		0x81100008
#define LYU_PIXEL_FORMAT_CUSTOM_BAYER_BG_16		0x81100009

int lyu_count_cameras();

int lyu_camera_open(int index);
void lyu_camera_close(int index);
void lyu_camera_close_all();

/* Device control */

const char *	lyu_camera_get_vendor_name	(int index);
const char *	lyu_camera_get_model_name	(int index);
const char *	lyu_camera_get_device_id	(int index);

/* Image format control */

void 		lyu_camera_get_sensor_size 	(int index, int *width, int *height);
void		lyu_camera_set_region		(int index, int x, int y, int width, int height);
void		lyu_camera_get_region		(int index, int *x, int *y, int *width, int *height);
void		lyu_camera_get_x_offset_bounds	(int index, int *min, int *max);
void		lyu_camera_get_y_offset_bounds	(int index, int *min, int *max);
void		lyu_camera_get_width_bounds	(int index, int *min, int *max);
void		lyu_camera_get_height_bounds	(int index, int *min, int *max);
void		lyu_camera_set_binning		(int index, int dx, int dy);
void		lyu_camera_get_binning		(int index, int *dx, int *dy);
void		lyu_camera_get_x_binning_bounds	(int index, int *min, int *max);
void		lyu_camera_get_y_binning_bounds	(int index, int *min, int *max);

int	lyu_camera_is_binning_available (int index);

void 		lyu_camera_set_pixel_format 				(int index, LyuPixelFormat format);
void		lyu_camera_set_pixel_format_from_string 		(int index, const char * format);
LyuPixelFormat	lyu_camera_get_pixel_format 				(int index);
const char * 	lyu_camera_get_pixel_format_as_string			(int index);
int64_t *	lyu_camera_get_available_pixel_formats			(int index, uint32_t *n_pixel_formats);
const char **	lyu_camera_get_available_pixel_formats_as_strings	(int index, uint32_t *n_pixel_formats);
const char **	lyu_camera_get_available_pixel_formats_as_display_names	(int index, uint32_t *n_pixel_formats);

/* Acquisition control */

struct _ArvBuffer;
typedef struct _ArvBuffer LyuBuffer;
typedef void (*LyuStreamCallback) (int index, LyuBuffer *buffer, void *user_data);

void		lyu_camera_start_acquisition (int index, LyuStreamCallback cb, void *user_data);
void		lyu_camera_stop_acquisition		(int index);
void		lyu_camera_abort_acquisition		(int index);

LyuBuffer *	lyu_camera_acquisition			(int index, uint64_t timeout);

int			lyu_buffer_get_image_width		(LyuBuffer *buffer);
int			lyu_buffer_get_image_height		(LyuBuffer *buffer);
LyuPixelFormat		lyu_buffer_get_image_pixel_format	(LyuBuffer *buffer);
const void *		lyu_buffer_get_data		(LyuBuffer *buffer, int *size);


void			lyu_camera_set_acquisition_mode 	(int index, LyuAcquisitionMode value);
LyuAcquisitionMode 	lyu_camera_get_acquisition_mode 	(int index);

void 		lyu_camera_set_frame_count	(int index, int64_t frame_count);
int64_t		lyu_camera_get_frame_count	(int index);
void		lyu_camera_get_frame_count_bounds	(int index, int64_t *min, int64_t *max);

int 	lyu_camera_is_frame_rate_available 	(int index);

void		lyu_camera_set_frame_rate		(int index, double frame_rate);
double 		lyu_camera_get_frame_rate 		(int index);
void		lyu_camera_get_frame_rate_bounds 	(int index, double *min, double *max);
void		lyu_camera_set_trigger			(int index, const char *source);
void 		lyu_camera_set_trigger_source		(int index, const char *source);
const char *	lyu_camera_get_trigger_source		(int index);
const char **	lyu_camera_get_available_trigger_sources(int index, uint32_t *n_sources);
const char**    lyu_camera_get_available_triggers       (int index, uint32_t *n_triggers);
void            lyu_camera_clear_triggers               (int index);
void 		lyu_camera_software_trigger 		(int index);

int 	lyu_camera_is_exposure_time_available	(int index);
int 	lyu_camera_is_exposure_auto_available	(int index);

void 		lyu_camera_set_exposure_time 		(int index, double exposure_time_us);
double 		lyu_camera_get_exposure_time 		(int index);
void		lyu_camera_get_exposure_time_bounds	(int index, double *min, double *max);
void		lyu_camera_set_exposure_time_auto	(int index, LyuAuto auto_mode);
LyuAuto		lyu_camera_get_exposure_time_auto	(int index);

/* Analog control */

int 	lyu_camera_is_gain_available		(int index);
int 	lyu_camera_is_gain_auto_available	(int index);

void 		lyu_camera_set_gain	 	(int index, double gain);
double 		lyu_camera_get_gain 		(int index);
void		lyu_camera_get_gain_bounds	(int index, double *min, double *max);
void		lyu_camera_set_gain_auto	(int index, LyuAuto auto_mode);
LyuAuto		lyu_camera_get_gain_auto	(int index);

/* Transport layer control */

uint32_t		lyu_camera_get_payload		(int index);

/* GigEVision specific API */

int	lyu_camera_is_gv_device			(int index);

int		    lyu_camera_gv_get_n_stream_channels	(int index);
void		lyu_camera_gv_select_stream_channel	(int index, int channel_id);
int 		lyu_camera_gv_get_current_stream_channel(int index);

void		lyu_camera_gv_set_packet_delay		(int index, int64_t delay_ns);
int64_t 	lyu_camera_gv_get_packet_delay 		(int index);
void 		lyu_camera_gv_set_packet_size 		(int index, int packet_size);
uint32_t	lyu_camera_gv_get_packet_size		(int index);
uint32_t	lyu_camera_gv_auto_packet_size		(int index);

/* void 		lyu_camera_gv_set_stream_options 	(int index, LyuGvStreamOption options); */

/* USB3Vision specific API */

int        lyu_camera_is_uv_device                 	(int index);
int        lyu_camera_uv_is_bandwidth_control_available 	(int index);
void            lyu_camera_uv_set_bandwidth             	(int index, uint32_t bandwidth);
uint32_t           lyu_camera_uv_get_bandwidth             	(int index);
void            lyu_camera_uv_get_bandwidth_bounds      	(int index, uint32_t* min, uint32_t* max);

/* Chunk data */

void 			lyu_camera_set_chunk_mode 	(int index, int is_active);
int 		lyu_camera_get_chunk_mode 	(int index);
void 			lyu_camera_set_chunk_state 	(int index, const char *chunk, int is_enabled);
int 		lyu_camera_get_chunk_state 	(int index, const char *chunk);
void 			lyu_camera_set_chunks 		(int index, const char *chunk_list);
/* LyuChunkParser * 	lyu_camera_create_chunk_parser 	(int index); */

G_END_DECLS

#endif
