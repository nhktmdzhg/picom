// SPDX-License-Identifier: MPL-2.0
// Copyright (c) Yuxuan Shui <yshuiv7@gmail.com>

#pragma once

/// Embedded shader sources ported from Burn-My-Windows. The table lives in
/// src/transition/bmw_shaders.c, which embeds the .frag files directly with
/// the C23 `#embed` directive. Adding a shader means adding an `#embed` block
/// and a table entry (keeping the table sorted by path) there. The shader
/// sources are licensed under GPL-3.0-or-later, see LICENSES/GPL-3.0-or-later.
struct bmw_shader_entry {
	const char *path;
	const char *source;
};

extern const struct bmw_shader_entry bmw_shader_sources[];
extern const unsigned number_of_bmw_shader_sources;

/// Look up an embedded shader by its file name (e.g. "bmw-fire.frag").
/// Returns NULL if `path` does not refer to an embedded shader.
const char *bmw_lookup_embedded_shader(const char *path);
