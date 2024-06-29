project('hamper', 'c',
  default_options: [
    'c_std=c2x'
  ]
)

dir = meson.current_source_dir()
cc = meson.get_compiler('c')
deps = [ dependency('raylib'), cc.find_library('m') ]

sources = [
  'src/main.c',
  'src/hamster.c',
  'src/editor.c',
  'src/resources.c',
  'src/tilemap.c',
  'src/animation.c',
  'src/keybinds.c',
  'src/gamestate.c',
  'src/alley.c',
  'src/levels.c',
  'src/menu.c',
  'src/button.c',
  'src/entity.c',
]

executable('exe', sources,
  include_directories: ['include'],
  dependencies: deps,
  install_dir: dir + '/bin',
  install: true)
