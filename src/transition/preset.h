// SPDX-License-Identifier: MPL-2.0
// Copyright (c) Yuxuan Shui <yshuiv7@gmail.com>

#pragma once

#include <stdbool.h>

typedef struct config_setting_t config_setting_t;
struct win_script;

/// Parse a animation preset definition into a win_script.
bool win_script_parse_preset(struct win_script *output, config_setting_t *setting,
                             const char *include_dir);

/// Resolve a shader path referenced by a built-in animation preset, relative to
/// the include directory of the configuration file. Returns NULL on failure.
struct shader_specification *
win_script_preset_shader(const char *include_dir, const char *path);
