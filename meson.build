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
  'src/resources.c',
  'src/tilemap.c',
  'src/animation.c',
  'src/keybinds.c',
  'src/gamestate.c',
  'src/levels.c',

  'src/scenes/editor.c',
  'src/scenes/menu.c',

  'src/ui/button.c',
  'src/ui/inputfield.c',
  'src/ui/level_selector.c',

  'src/entities/hamster.c',
  'src/entities/alley.c',
  'src/entities/entity.c',
]

executable('exe', sources,
  include_directories: ['include'],
  dependencies: deps,
  install_dir: dir + '/bin',
  install: true)
