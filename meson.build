project('cgol', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

subdir('include')
subdir('src')

sdl = dependency('sdl2', version : '>=2.0.12')

executable('cgol',
           sources,
           include_directories : inc,
           dependencies: [sdl],
           install : true)
