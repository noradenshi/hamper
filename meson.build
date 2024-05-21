project('hamper', 'c',
  default_options: [
    'prefix=/home/noradenshi/Projects/hamper',
    'c_std=c2x'
  ]
)

cc = meson.get_compiler('c')

deps = [ cc.find_library('raylib'), cc.find_library('m') ]

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
  install: true)
