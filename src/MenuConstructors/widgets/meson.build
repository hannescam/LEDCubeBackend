widget_src = [
	'PropertyWidget.cpp',
	'ButtonWidget.cpp',
	'SwitchWidget.cpp',
	'TextboxWidget.cpp',
	'ExpanderWidget.cpp',
	'SpinnerWidget.cpp',
	'DropdownWidget.cpp',
	'SliderWidget.cpp'
]

widget_inc_dir = [
	include_directories('include'),
	includedirs,
]

widgets_stat_lib = static_library('LumaMenuC++', sources : widget_src, cpp_args : ['-std=c++17'], include_directories : widget_inc_dir)