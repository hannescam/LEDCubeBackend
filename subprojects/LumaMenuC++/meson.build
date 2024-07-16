project('LumaMenuC++', 'cpp', default_options : ['cpp_std=c++17'], version: '0.0.2-unstable',)
deps = [dependency('python3-embed'), dependency('pybind11'), dependency('libgpiodcxx', version : '>=2.0.0')]
src = ['LumaMenuC++.cpp', 'LumaC++Wrapper.cpp']
inc = 'include'

lib = shared_library('LumaMenuC++', sources : src, dependencies : deps, cpp_args : ['-std=c++17'], include_directories : inc)
proj_dep = declare_dependency(compile_args : ['-std=c++17'], dependencies : deps, include_directories : inc, link_with : lib)