// SPDX-License-Identifier: MPL-2.0
//
// Embedded Burn-My-Windows shader sources. The shader sources themselves
// are licensed under GPL-3.0-or-later, (c) Simon Schneegans and the
// Burn-My-Windows contributors; the copyright and license notices are
// kept in the .frag files below.
//
// To add a shader: drop the .frag file into data/shaders/, add an
// `#embed` block below, and add an entry to bmw_shader_sources[] (the
// table must stay sorted by path, the lookup uses bsearch).

#include <stdlib.h>
#include <string.h>

#include "transition/bmw_shaders.h"

static const unsigned char bmw_aura_glow_frag[] = {
#embed "../../data/shaders/bmw-aura-glow.frag" suffix(, 0)
};

static const unsigned char bmw_energize_a_frag[] = {
#embed "../../data/shaders/bmw-energize-a.frag" suffix(, 0)
};

static const unsigned char bmw_energize_b_frag[] = {
#embed "../../data/shaders/bmw-energize-b.frag" suffix(, 0)
};

static const unsigned char bmw_fire_frag[] = {
#embed "../../data/shaders/bmw-fire.frag" suffix(, 0)
};

static const unsigned char bmw_focus_frag[] = {
#embed "../../data/shaders/bmw-focus.frag" suffix(, 0)
};

static const unsigned char bmw_glide_frag[] = {
#embed "../../data/shaders/bmw-glide.frag" suffix(, 0)
};

static const unsigned char bmw_glitch_frag[] = {
#embed "../../data/shaders/bmw-glitch.frag" suffix(, 0)
};

static const unsigned char bmw_hexagon_frag[] = {
#embed "../../data/shaders/bmw-hexagon.frag" suffix(, 0)
};

static const unsigned char bmw_pixel_wheel_frag[] = {
#embed "../../data/shaders/bmw-pixel-wheel.frag" suffix(, 0)
};

static const unsigned char bmw_pixel_wipe_frag[] = {
#embed "../../data/shaders/bmw-pixel-wipe.frag" suffix(, 0)
};

static const unsigned char bmw_pixelate_frag[] = {
#embed "../../data/shaders/bmw-pixelate.frag" suffix(, 0)
};

static const unsigned char bmw_portal_frag[] = {
#embed "../../data/shaders/bmw-portal.frag" suffix(, 0)
};

static const unsigned char bmw_rgbwarp_frag[] = {
#embed "../../data/shaders/bmw-rgbwarp.frag" suffix(, 0)
};

static const unsigned char bmw_tv_glitch_frag[] = {
#embed "../../data/shaders/bmw-tv-glitch.frag" suffix(, 0)
};

static const unsigned char bmw_tv_frag[] = {
#embed "../../data/shaders/bmw-tv.frag" suffix(, 0)
};

static const unsigned char bmw_wisps_frag[] = {
#embed "../../data/shaders/bmw-wisps.frag" suffix(, 0)
};

const struct bmw_shader_entry bmw_shader_sources[] = {
    {"bmw-aura-glow.frag", (const char *)bmw_aura_glow_frag},
    {"bmw-energize-a.frag", (const char *)bmw_energize_a_frag},
    {"bmw-energize-b.frag", (const char *)bmw_energize_b_frag},
    {"bmw-fire.frag", (const char *)bmw_fire_frag},
    {"bmw-focus.frag", (const char *)bmw_focus_frag},
    {"bmw-glide.frag", (const char *)bmw_glide_frag},
    {"bmw-glitch.frag", (const char *)bmw_glitch_frag},
    {"bmw-hexagon.frag", (const char *)bmw_hexagon_frag},
    {"bmw-pixel-wheel.frag", (const char *)bmw_pixel_wheel_frag},
    {"bmw-pixel-wipe.frag", (const char *)bmw_pixel_wipe_frag},
    {"bmw-pixelate.frag", (const char *)bmw_pixelate_frag},
    {"bmw-portal.frag", (const char *)bmw_portal_frag},
    {"bmw-rgbwarp.frag", (const char *)bmw_rgbwarp_frag},
    {"bmw-tv-glitch.frag", (const char *)bmw_tv_glitch_frag},
    {"bmw-tv.frag", (const char *)bmw_tv_frag},
    {"bmw-wisps.frag", (const char *)bmw_wisps_frag},
};
const unsigned number_of_bmw_shader_sources =
    (unsigned)(sizeof(bmw_shader_sources) / sizeof(bmw_shader_sources[0]));

static int bmw_shader_entry_cmp(const void *key, const void *elem) {
	return strcmp((const char *)key, ((const struct bmw_shader_entry *)elem)->path);
}

const char *bmw_lookup_embedded_shader(const char *path) {
	// The table is sorted by path, so a binary search works.
	const struct bmw_shader_entry *entry =
	    bsearch(path, bmw_shader_sources, number_of_bmw_shader_sources,
	            sizeof(bmw_shader_sources[0]), bmw_shader_entry_cmp);
	return entry ? entry->source : NULL;
}
