#!/usr/bin/env python

import pathlib
import shutil
import sys
import os

buildroot = pathlib.Path(os.environ['MESON_BUILD_ROOT'])

if not buildroot.exists():
	raise FileNotFoundError('Build directory does not exist.')

bundleroot = (buildroot / 'ThretrisBundle')
if bundleroot.exists():
	shutil.rmtree(bundleroot)

bundleroot.mkdir()

exe_suffix = '.exe' if os.name == 'nt' else ''
so_prefix = '' if os.name == 'nt' else 'lib'
so_suffix = '.dll' if os.name == 'nt' else '.dylib' if sys.platform.startswith('darwin') else '.so'

thretris = pathlib.Path(os.environ['MESON_SOURCE_ROOT'])

shutil.copy2(buildroot / 'subprojects' / 'cacaoengine' / 'cacao' / ('cacaoengine' + exe_suffix), bundleroot)
shutil.copytree(thretris / 'assets', bundleroot / 'assets')
shutil.copy2(thretris / 'launchconfig.cacao.yml', bundleroot)
shutil.copy2(buildroot / ('launch' + so_suffix), bundleroot)

shaderroot = (bundleroot / 'assets' / 'shaders')
for file in (buildroot / ('launch' + so_suffix + '.p')).rglob('*.spv'):
	shutil.copy2(file, shaderroot)

def get_engine_path():
	return (bundleroot / ('cacaoengine' + exe_suffix))