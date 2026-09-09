// SPDX-License-Identifier: MPL-2.0
// Copyright (c) Yuxuan Shui <yshuiv7@gmail.com>

#include <libconfig.h>
#include <stdbool.h>
#include <string.h>
#include <test.h>

#include "bmw_shaders.h"
#include "config.h"
#include "preset.h"
#include "script.h"

extern struct {
	const char *name;
	bool (*func)(struct win_script *output, config_setting_t *setting,
	             const char *include_dir);
} win_script_presets[];

struct shader_specification *
win_script_preset_shader(const char *include_dir, const char *path) {
	char *full_path = locate_auxiliary_file("shaders", path, include_dir);
	if (full_path) {
		auto ret = shader_spec_from_path(full_path);
		free(full_path);
		return ret;
	}
	if (bmw_lookup_embedded_shader(path) != NULL) {
		// Not found on disk, but built into the binary.
		return shader_spec_from_path(path);
	}
	log_error("Couldn't find preset shader file \"%s\"", path);
	return NULL;
}

bool win_script_parse_preset(struct win_script *output, config_setting_t *setting,
                             const char *include_dir) {
	const char *preset = NULL;
	if (!config_setting_lookup_string(setting, "preset", &preset)) {
		log_error("Missing preset name in script");
		return false;
	}
	for (unsigned i = 0; win_script_presets[i].name; i++) {
		if (strcmp(preset, win_script_presets[i].name) == 0) {
			log_debug("Using animation preset: %s", preset);
			return win_script_presets[i].func(output, setting, include_dir);
		}
	}
	log_error("Unknown preset: %s", preset);
	return false;
}

TEST_CASE(bmw_shader_lookup) {
	TEST_NOTEQUAL(bmw_lookup_embedded_shader("bmw-fire.frag"), NULL);
	TEST_NOTEQUAL(bmw_lookup_embedded_shader("bmw-rgbwarp.frag"), NULL);
	TEST_EQUAL(bmw_lookup_embedded_shader("nonexistent.frag"), NULL);
	TEST_EQUAL(bmw_lookup_embedded_shader(""), NULL);
	// The lookup must be consistent for every embedded shader; this also
	// guards the sorted-table invariant required by the bsearch lookup.
	for (unsigned i = 0; i < number_of_bmw_shader_sources; i++) {
		auto entry = &bmw_shader_sources[i];
		TEST_EQUAL((void *)bmw_lookup_embedded_shader(entry->path),
		           (void *)entry->source);
	}
}
