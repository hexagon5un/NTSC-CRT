/*****************************************************************************/
/*
 * NTSC/CRT - integer-only NTSC video signal encoding / decoding emulation
 *
 *   by EMMIR 2018-2023
 *
 *   YouTube: https://www.youtube.com/@EMMIR_KC/videos
 *   Discord: https://discord.com/invite/hdYctSmyQJ
 */
/*****************************************************************************/
#ifndef _CRT_H_
#define _CRT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* crt.h
 *
 * An interface to convert a digital image to an analog NTSC signal
 * and decode the NTSC signal back into a digital image.
 * Can easily be integrated into real-time applications
 * or be used as a command-line tool.
 *
 */

/* SAMPLE RATE: 14.31818 MHz.
 * Which, divided by 4, gives us 3.579545 MHz for the chroma carrier
 */

/* do bloom emulation (side effect: makes screen have black borders) */
#define CRT_DO_BLOOM    0

#define CRT_CB_FREQ     4 /* carrier frequency relative to sample rate */
#define CRT_HRES        (2275 * CRT_CB_FREQ / 10) /* horizontal resolution */
#define CRT_VRES        262                       /* vertical resolution */
#define CRT_INPUT_SIZE  (CRT_HRES * CRT_VRES)

#define CRT_TOP         21     /* first line with active video */
#define CRT_BOT         261    /* final line with active video */
#define CRT_LINES       (CRT_BOT - CRT_TOP) /* number of active video lines */

struct CRT {
    signed char analog[CRT_INPUT_SIZE]; /* sampled at 14.31818 MHz */
    signed char inp[CRT_INPUT_SIZE]; /* CRT input, can be noisy */
    int hsync, vsync; /* used internally to keep track of sync over frames */
    int brightness, contrast, saturation; /* common monitor settings */
    int black_point, white_point; /* user-adjustable */
    int outw, outh; /* output width/height */
    int *out; /* output image */
};

/* Initializes the library. Sets up filters.
 *   w   - width of the output image
 *   h   - height of the output image
 *   out - pointer to output image data 32-bit RGB packed as 0xXXRRGGBB
 */
extern void crt_init(struct CRT *v, int w, int h, int *out);

/* Updates the output image parameters
 *   w   - width of the output image
 *   h   - height of the output image
 *   out - pointer to output image data 32-bit RGB packed as 0xXXRRGGBB
 */
extern void crt_resize(struct CRT *v, int w, int h, int *out);

/* Resets the CRT settings back to their defaults */
extern void crt_reset(struct CRT *v);

struct NTSC_SETTINGS {
    const int *rgb; /* 32-bit RGB image data (packed as 0xXXRRGGBB) */
    int w, h;       /* width and height of image */
    int as_color;   /* 0 = monochrome, 1 = full color */
    int field;      /* 0 = even, 1 = odd */
};

/* Convert RGB image to analog NTSC signal
 *   s - struct containing settings to apply to this field
 */
extern void crt_2ntsc(struct CRT *v, struct NTSC_SETTINGS *s);

/* Decodes the NTSC signal generated by crt_2ntsc()
 *   noise - the amount of noise added to the signal (0 - inf)
 */
extern void crt_draw(struct CRT *v, int noise);

#ifdef __cplusplus
}
#endif

#endif
