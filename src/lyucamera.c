#include "lyucamera.h"
#include "arvcamera.h"
#include "arvsystem.h"
#include "arvbuffer.h"

static char *arv_option_debug_domains = "interface,device,chunk,stream,stream-thread,cp,sp,genicam,evaluator,misc,viewer";

#define MAX_CAMERAS 16
#define BUFFER_DEFAULT 8

struct LyuCamera {
    int index;
    ArvCamera *camera;
    ArvStream *stream;

    LyuStreamCallback stream_cb;
    void *user_data;
    int nbuffer;

    GThread *thread;
    gboolean cancel;
} cameras[MAX_CAMERAS];
static int ncamera;

static ArvCamera *
lyu_get_camera_at(int index)
{
    if (index < 0 || index >= ncamera)
        return NULL;
    return cameras[index].camera;
}

static struct LyuCamera *
lyu_get_lyucamera_at(int index)
{
    if (index < 0 || index >= ncamera)
        return NULL;
    return cameras;
}

int
lyu_count_cameras()
{
    arv_debug_enable(arv_option_debug_domains);
    arv_update_device_list();
    ncamera = arv_get_n_devices();
    if (ncamera > MAX_CAMERAS)
        ncamera = MAX_CAMERAS;

    return ncamera;
}

int
lyu_camera_open(int index)
{
    if (index < 0 || index >= ncamera)
        return 0;

    ArvCamera *cam = lyu_get_camera_at(index);

    if (cam == NULL) {
        const char *device_id = arv_get_device_id (index);
        cam = arv_camera_new(device_id);
        if (cam) {
            arv_camera_gv_set_packet_size(cam, 8192);
            cameras[index].camera = cam;
            cameras[index].index = index;
        }
    }
    return cam != NULL;
}

void
lyu_camera_close(int index)
{
    if (index < 0 || index >= ncamera)
        return;
    if (cameras[index].camera)
        g_clear_object(&cameras[index].camera);
}

void
lyu_camera_close_all()
{
    for (int i = 0; i < ncamera; i++)
        g_clear_object(&cameras[i].camera);
    ncamera = 0;
}


/* Device control */

const char *
lyu_camera_get_vendor_name	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_vendor_name(cam);
}

const char *
lyu_camera_get_model_name	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_model_name(cam);
}

const char *
lyu_camera_get_device_id	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_device_id(cam);
}

/* Image format control */

void
lyu_camera_get_sensor_size 	(int index, int *width, int *height)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_sensor_size(cam, width, height);
}

void
lyu_camera_set_region		(int index, int x, int y, int width, int height)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_region(cam, x, y, width, height);
}

void
lyu_camera_get_region		(int index, int *x, int *y, int *width, int *height)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_region(cam, x, y, width, height);
}

void
lyu_camera_get_x_offset_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_x_offset_bounds(cam, min, max);
}

void
lyu_camera_get_y_offset_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_y_offset_bounds(cam, min, max);
}

void
lyu_camera_get_width_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_width_bounds(cam, min, max);
}

void
lyu_camera_get_height_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_height_bounds(cam, min, max);
}

void
lyu_camera_set_binning		(int index, int dx, int dy)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_binning(cam, dx, dy);
}

void
lyu_camera_get_binning		(int index, int *dx, int *dy)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_binning(cam, dx, dy);
}

void
lyu_camera_get_x_binning_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_x_binning_bounds(cam, min, max);
}

void
lyu_camera_get_y_binning_bounds	(int index, int *min, int *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_y_binning_bounds(cam, min, max);
}


int
lyu_camera_is_binning_available (int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_binning_available(cam);
}


void
lyu_camera_set_pixel_format 				(int index, LyuPixelFormat format)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_pixel_format(cam, format);
}

void
lyu_camera_set_pixel_format_from_string 		(int index, const char * format)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_pixel_format_from_string(cam, format);
}

LyuPixelFormat
lyu_camera_get_pixel_format 				(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_pixel_format(cam);
}

const char *
lyu_camera_get_pixel_format_as_string			(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_pixel_format_as_string(cam);
}

int64_t *
lyu_camera_get_available_pixel_formats			(int index, uint32_t *n_pixel_formats)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_available_pixel_formats(cam, n_pixel_formats);
}

const char**
lyu_camera_get_available_pixel_formats_as_strings	(int index, uint32_t *n_pixel_formats)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_available_pixel_formats_as_strings(cam, n_pixel_formats);
}

const char **
lyu_camera_get_available_pixel_formats_as_display_names	(int index, uint32_t *n_pixel_formats)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_available_pixel_formats_as_display_names(cam, n_pixel_formats);
}


/* Acquisition control */
static void
_ArvStreamCallback	(void *user_data, ArvStreamCallbackType type, ArvBuffer *buffer)
{
    if (type == ARV_STREAM_CALLBACK_TYPE_INIT) {
        if (!arv_make_thread_realtime (10) &&
            !arv_make_thread_high_priority (-10))
            g_warning ("Failed to make stream thread high priority");
    }
}

static void *
lyu_stream_thread(void *data)
{
    struct LyuCamera *lcam = (struct LyuCamera *)data;
    while (!g_atomic_int_get (&lcam->cancel)) {
        ArvBuffer *buffer = arv_stream_timeout_pop_buffer (lcam->stream, 80000);
        if (buffer) {
            if (ARV_IS_BUFFER(buffer)
                && arv_buffer_get_status(buffer) == ARV_BUFFER_STATUS_SUCCESS) {
                if (lcam->stream_cb)
                    lcam->stream_cb(lcam->index, buffer, lcam->user_data);
            }
            arv_stream_push_buffer(lcam->stream, buffer);
        }
    }

    return NULL;
}

void
lyu_camera_start_acquisition		(int index, LyuStreamCallback cb, void *user_data)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;

    struct LyuCamera *lcam = lyu_get_lyucamera_at(index);
    lcam->user_data = user_data;
    lcam->stream_cb = cb;
    lcam->stream = arv_camera_create_stream(cam, _ArvStreamCallback, lcam);

    int payload = arv_camera_get_payload (cam);
    lcam->nbuffer = BUFFER_DEFAULT;

    int nbuffer = lcam->nbuffer;
    while (nbuffer--)
        arv_stream_push_buffer (lcam->stream,  arv_buffer_new (payload, NULL));

    lcam->cancel = FALSE;
    lcam->thread = g_thread_new("ly_cam_stream", lyu_stream_thread, lcam);

    arv_camera_start_acquisition(cam);
}

void
lyu_camera_stop_acquisition		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;

    arv_camera_stop_acquisition(cam);

    struct LyuCamera *lcam = lyu_get_lyucamera_at(index);
    g_atomic_int_set (&lcam->cancel, TRUE);
    g_thread_join (lcam->thread);

    lcam->stream_cb = 0;
    g_object_unref(lcam->stream);
}

void
lyu_camera_abort_acquisition		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_abort_acquisition(cam);

    struct LyuCamera *lcam = lyu_get_lyucamera_at(index);
    lcam->stream_cb = 0;
    g_object_unref(lcam->stream);
}

LyuBuffer *	lyu_camera_acquisition			(int index, uint64_t timeout)
{
    struct LyuCamera *lcam = lyu_get_lyucamera_at(index);
    ArvBuffer *buffer;
    if (timeout > 0) {
        buffer = arv_stream_timeout_pop_buffer (lcam->stream, timeout);
    } else {
        buffer = arv_stream_pop_buffer (lcam->stream);
    }

    if (buffer)
        arv_stream_push_buffer(lcam->stream, buffer);

    if (arv_buffer_get_status(buffer) == ARV_BUFFER_STATUS_SUCCESS)
        return buffer;

    return NULL;
}

void
lyu_camera_set_acquisition_mode 	(int index, LyuAcquisitionMode value)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_acquisition_mode(cam, (int)value);
}

LyuAcquisitionMode
lyu_camera_get_acquisition_mode 	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return (int)arv_camera_get_acquisition_mode(cam);
}


void
lyu_camera_set_frame_count	(int index, int64_t frame_count)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_frame_count(cam, frame_count);
}

int64_t
lyu_camera_get_frame_count	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_frame_count(cam);
}

void
lyu_camera_get_frame_count_bounds	(int index, int64_t *min, int64_t *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_frame_count_bounds(cam, min, max);
}


int
lyu_camera_is_frame_rate_available 	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_frame_rate_available(cam);
}


void
lyu_camera_set_frame_rate		(int index, double frame_rate)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_frame_rate(cam, frame_rate);
}

double
lyu_camera_get_frame_rate 		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_frame_rate(cam);
}

void
lyu_camera_get_frame_rate_bounds 	(int index, double *min, double *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_frame_rate_bounds(cam, min, max);
}

void
lyu_camera_set_trigger			(int index, const char *source)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_trigger(cam, source);
}

void
lyu_camera_set_trigger_source		(int index, const char *source)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_trigger_source(cam, source);
}

const char *
lyu_camera_get_trigger_source		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_trigger_source(cam);
}

const char **
lyu_camera_get_available_trigger_sources(int index, uint32_t *n_sources)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_available_trigger_sources(cam, n_sources);
}

const char**
lyu_camera_get_available_triggers       (int index, uint32_t *n_triggers)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return NULL;
    return arv_camera_get_available_triggers(cam, n_triggers);
}

void
lyu_camera_clear_triggers               (int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_clear_triggers(cam);
}

void
lyu_camera_software_trigger 		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_software_trigger(cam);
}


int
lyu_camera_is_exposure_time_available	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_exposure_time_available(cam);
}

int
lyu_camera_is_exposure_auto_available	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_exposure_auto_available(cam);
}


void
lyu_camera_set_exposure_time 		(int index, double exposure_time_us)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_exposure_time(cam, exposure_time_us);
}

double
lyu_camera_get_exposure_time 		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_exposure_time(cam);
}

void
lyu_camera_get_exposure_time_bounds	(int index, double *min, double *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_exposure_time_bounds(cam, min, max);
}

void
lyu_camera_set_exposure_time_auto	(int index, LyuAuto auto_mode)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_exposure_time_auto(cam, (int)auto_mode);
}

LyuAuto
lyu_camera_get_exposure_time_auto	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return (int)arv_camera_get_exposure_time_auto(cam);
}


/* Analog control */

int
lyu_camera_is_gain_available		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_gain_available(cam);
}

int
lyu_camera_is_gain_auto_available	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_gain_auto_available(cam);
}


void
lyu_camera_set_gain	 	(int index, double gain)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_gain(cam, gain);
}

double
lyu_camera_get_gain 		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_gain(cam);
}

void
lyu_camera_get_gain_bounds	(int index, double *min, double *max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_get_gain_bounds(cam, min, max);
}

void
lyu_camera_set_gain_auto	(int index, LyuAuto auto_mode)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_gain_auto(cam, (int)auto_mode);
}

LyuAuto
lyu_camera_get_gain_auto	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return (int)arv_camera_get_gain_auto(cam);
}


/* Transport layer control */

uint32_t
lyu_camera_get_payload		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_payload(cam);
}


/* GigEVision specific API */

int
lyu_camera_is_gv_device			(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_gv_device(cam);
}


int
lyu_camera_gv_get_n_stream_channels	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_gv_get_n_stream_channels(cam);
}

void
lyu_camera_gv_select_stream_channel	(int index, int channel_id)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_gv_select_stream_channel(cam, channel_id);
}

int
lyu_camera_gv_get_current_stream_channel(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_gv_get_current_stream_channel(cam);
}


void
lyu_camera_gv_set_packet_delay		(int index, int64_t delay_ns)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_gv_set_packet_delay(cam, delay_ns);
}

int64_t
lyu_camera_gv_get_packet_delay 		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_gv_get_packet_delay(cam);
}

void
lyu_camera_gv_set_packet_size 		(int index, int packet_size)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_gv_set_packet_size(cam, packet_size);
}

uint32_t
lyu_camera_gv_get_packet_size		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return	arv_camera_gv_get_packet_size(cam);
}

uint32_t
lyu_camera_gv_auto_packet_size		(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_gv_auto_packet_size(cam);
}


/* void 		lyu_camera_gv_set_stream_options 	(int index, LyuGvStreamOption options); */

/* USB3Vision specific API */

int
lyu_camera_is_uv_device                 	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_is_uv_device(cam);
}

int
lyu_camera_uv_is_bandwidth_control_available 	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_uv_is_bandwidth_control_available(cam);
}

void
lyu_camera_uv_set_bandwidth             	(int index, uint32_t bandwidth)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_uv_set_bandwidth(cam, bandwidth);
}

uint32_t
lyu_camera_uv_get_bandwidth             	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_uv_get_bandwidth(cam);
}

void
lyu_camera_uv_get_bandwidth_bounds      	(int index, uint32_t* min, uint32_t* max)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_uv_get_bandwidth_bounds(cam, min, max);
}


/* Chunk data */

void
lyu_camera_set_chunk_mode 	(int index, int is_active)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_chunk_mode(cam, is_active);
}

int
lyu_camera_get_chunk_mode 	(int index)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_chunk_mode(cam);
}

void
lyu_camera_set_chunk_state 	(int index, const char *chunk, int is_enabled)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    arv_camera_set_chunk_state(cam, chunk, is_enabled);
}

int
lyu_camera_get_chunk_state 	(int index, const char *chunk)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return 0;
    return arv_camera_get_chunk_state(cam, chunk);
}

void
lyu_camera_set_chunks 		(int index, const char *chunk_list)
{
    ArvCamera *cam = lyu_get_camera_at(index);
    if (!cam)
        return;
    return arv_camera_set_chunks(cam, chunk_list);
}

int
lyu_buffer_get_image_width		(LyuBuffer *buffer)
{
    return arv_buffer_get_image_width(buffer);
}

int
lyu_buffer_get_image_height		(LyuBuffer *buffer)
{
    return arv_buffer_get_image_height(buffer);
}

LyuPixelFormat
lyu_buffer_get_image_pixel_format	(LyuBuffer *buffer)
{
    return arv_buffer_get_image_pixel_format(buffer);
}

const void *
lyu_buffer_get_data		(LyuBuffer *buffer, int *size)
{
    return arv_buffer_get_data(buffer, (size_t*)size);
}
