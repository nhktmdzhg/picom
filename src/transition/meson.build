# The shader sources are embedded into the binary. They are generated at
# build time by `tools/animgen.c` from `data/shaders/*.frag`, so no
# generated file is kept in the git tree.

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

bmw_shader_sources = custom_target(
	'bmw_shader_sources.c',
	input: files(
		'../../data/shaders/bmw-aura-glow.frag',
		'../../data/shaders/bmw-energize-a.frag',
		'../../data/shaders/bmw-energize-b.frag',
		'../../data/shaders/bmw-fire.frag',
		'../../data/shaders/bmw-focus.frag',
		'../../data/shaders/bmw-glide.frag',
		'../../data/shaders/bmw-glitch.frag',
		'../../data/shaders/bmw-hexagon.frag',
		'../../data/shaders/bmw-pixel-wheel.frag',
		'../../data/shaders/bmw-pixel-wipe.frag',
		'../../data/shaders/bmw-pixelate.frag',
		'../../data/shaders/bmw-portal.frag',
		'../../data/shaders/bmw-rgbwarp.frag',
		'../../data/shaders/bmw-tv-glitch.frag',
		'../../data/shaders/bmw-tv.frag',
		'../../data/shaders/bmw-wisps.frag',
	),
	output: 'bmw_shader_sources.c',
	command: [animgen_native, 'shaders',
	          meson.project_source_root() / 'data' / 'shaders'],
	capture: true,
)

srcs += [files('generated/script_templates.c', 'curve.c', 'preset.c', 'script.c'),
         bmw_shader_sources]
