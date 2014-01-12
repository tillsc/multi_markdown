require 'mkmf'

dir_config('multi_markdown')

mmd_objs = %w[parse_utilities.o parser.o GLibFacade.o writer.o text.o html.o latex.o memoir.o beamer.o lyx.o lyxbeamer.o opml.o odf.o critic.o rng.o rtf.o transclude.o]

$objs = mmd_objs.map { |s| "../MultiMarkdown-4/#{s}" } + ["multi_markdown.o"]

if pkg_config = find_executable('pkg-config')
  $CFLAGS = "-fcommon "+`#{pkg_config} --cflags glib-2.0`
  $LDFLAGS = `#{pkg_config} --libs glib-2.0`
else
  fail "glib2 not found"
end

create_header
create_makefile('multi_markdown')
