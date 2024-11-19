project('thretris', 'cpp', default_options: ['cpp_std=c++20'])

cacao = subproject('cacaoengine', required: true, default_options: ['use_backend=gl-sdl', 'build_playground=false'])
cacao_dep = cacao.get_variable('cacao_dep')

dxc = find_program('dxc', required: true)
spv_gen = generator(dxc, output: '@BASENAME@.spv', arguments: ['-spirv', '-fvk-use-gl-layout', '@EXTRA_ARGS@', '-Fo', '@OUTPUT@', '@INPUT@'])

vshaders = spv_gen.process([
	'src/shaders/block.vs.hlsl',
	'src/shaders/plat.vs.hlsl'
], extra_args: [ '-T', 'vs_6_0' ])

pshaders = spv_gen.process([
	'src/shaders/block.ps.hlsl',
	'src/shaders/plat.ps.hlsl'
], extra_args: [ '-T', 'ps_6_0' ])

launch = shared_module('launch', name_prefix: '', dependencies: cacao_dep, include_directories: 'include', sources: [
	'src/ThretrisMain.cpp',
	'src/Thretris.cpp',
	'src/Thretromino.cpp',
	'src/GameMgr.cpp',
	'src/CamMgr.cpp',
	vshaders,
	pshaders
], build_by_default: true)

cacao_exe = cacao.get_variable('cacao_exe')
python = find_program('python', required: true)
run_target('bundle', command: [python, meson.project_source_root()/'scripts'/'bundle_playground.py'], depends: [
	launch,
	cacao_exe
])
run_target('run', command: [python, meson.project_source_root()/'scripts'/'bundleandrun.py'], depends: [
	launch,
	cacao_exe
])