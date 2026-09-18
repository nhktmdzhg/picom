# animgen compiles the animation presets from data/animation_presets.conf
# into C code.
animgen_native = executable(
	'animgen_native',
	files(
		'../../tools/animgen.c',
		'../log.c',
		'../utils/dynarr.c',
		'../utils/misc.c',
		'../utils/str.c',
		'curve.c',
		'script.c',
	),
	dependencies: [base_deps, libconfig_dep, test_h_dep, cc.find_library('m')],
	c_args: ['-D_GNU_SOURCE'],
	include_directories: picom_inc,
	native: true,
	build_by_default: false,
	install: false,
)

# The preset templates are generated into the build directory; no generated
# file is kept in the git tree. The embedded shader sources are part of the
# source tree (bmw_shaders.c uses the C23 `#embed` directive).
script_templates = custom_target(
	'script_templates.c',
	output: 'script_templates.c',
	input: files('../../data/animation_presets.conf'),
	command: [animgen_native, '@INPUT@', '@OUTPUT@'],
)

srcs += [script_templates, files('bmw_shaders.c', 'curve.c', 'preset.c', 'script.c')]
