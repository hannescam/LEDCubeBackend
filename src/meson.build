ledcubebackend_sources = [
  'main.cpp',
  'SocketComms.cpp',
  'SocketWrapper.cpp',
  'SocketHandler.cpp',
  'Logger.cpp',
  'LoggerSettings.cpp',
  'ErrorConstructor.cpp',
]

ledcubebackend_deps = [
   dependency('yaml-cpp'),
#   LumaMenuCPP,
]

main_include = include_directories('include')

includedirs = main_include

subdir('MenuConstructors')

includedirs = [menu_inc_dirs, main_include]

executable('LEDCubeBackend', ledcubebackend_sources, dependencies: ledcubebackend_deps, install: true, include_directories : includedirs, link_with : MenuConstroctorsLib)