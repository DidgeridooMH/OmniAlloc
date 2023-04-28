project('omnialloc', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

omni = library(
  'omnialloc',
  sources : [
    'src/ComponentPool.cpp'],
  include_directories : ['include'],
  cpp_args : ['-DOMNIALLOC_LIBRARY_BUILD'],
  install : true)

omni_dep = declare_dependency(
  link_with : omni,
  include_directories : ['include'])

executable('omnialloc_test',
           'test/main.cpp',
           install : true,
           dependencies : [omni_dep])
