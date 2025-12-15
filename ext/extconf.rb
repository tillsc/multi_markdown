require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

if RbConfig::CONFIG['CC'] == 'clang' || RbConfig::CONFIG['CC_VERSION_MESSAGE'].to_s.include?('clang')
  $CFLAGS="#{$CFLAGS} -fbracket-depth=512"
  $CXXFLAGS="#{$CXXFLAGS} -fbracket-depth=512"
else
  $CFLAGS="#{$CFLAGS} -std=c99"
  $CXXFLAGS="#{$CXXFLAGS} -std=c99"
end

$CFLAGS  << '  -DNDEBUG=1 -DDISABLE_OBJECT_POOL -D_XOPEN_SOURCE=700'
$CXXFLAGS << '  -DNDEBUG=1 -DDISABLE_OBJECT_POOL -D_XOPEN_SOURCE=700'

HEADER_DIRS = [
  File.expand_path('../mmd', __FILE__),
  INCLUDEDIR,
  '/usr/include'
]

dir_config('multi_markdown', HEADER_DIRS, [LIBDIR])

find_header('libMultiMarkdown.h')
find_header('d_string.h')
find_header('stdio.h')

$objs = Dir.glob('**/*.c').map { |s| s.gsub(/\.c$/, ".o") }

create_header
create_makefile('multi_markdown')
