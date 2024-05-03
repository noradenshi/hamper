project('hamper', 'c',
  default_options: [
    'prefix=/home/noradenshi/Projects/hamper',
    'c_std=c2x'
  ]
)

cc = meson.get_compiler('c')

deps = [ cc.find_library('raylib') ]

sources = [
  'src/main.c',
  'src/hamster.c',
  'src/editor.c',
  'src/resources.c',
  'src/tilemap.c',
  'src/animation.c',
  'src/keybind.c',
  'src/gamestate.c',
  'src/alley.c',
]

executable('exe', sources,
  include_directories: ['include'],
  dependencies: deps,
  install: true)
