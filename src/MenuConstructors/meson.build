MenuConstroctorsIncludeDir = include_directories('include')

MenuConstroctorsSources = [
	'MenuConstructor.cpp',
	'GroupConstructor.cpp',
]

subdir('widgets')

menu_inc_dirs = [
	MenuConstroctorsIncludeDir,
	widget_inc_dir,
	includedirs
]

MenuConstroctorsLib = static_library('LumaMenuC++', sources : MenuConstroctorsSources, cpp_args : ['-std=c++17'], include_directories : menu_inc_dirs, link_with : widgets_stat_lib)
