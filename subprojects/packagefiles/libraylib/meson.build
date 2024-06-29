project('raylib', 'c')

if not (host_machine.system() == 'windows' and host_machine.cpu_family() == 'x86_64')
  error('This wrap of libbob is a binary wrap for x64_64 Windows, and will not work on your system')
endif

cc = meson.get_compiler('c')

dep = declare_dependency(
  dependencies : cc.find_library('raylib',
    dirs : meson.current_source_dir() / 'lib'),
  include_directories : include_directories('include'),
)

meson.override_dependency('raylib', dep)
