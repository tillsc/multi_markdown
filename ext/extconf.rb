require 'mkmf'

dir_config('multi_markdown')

mmd_objs = %w[parse_utilities.o parser.o GLibFacade.o writer.o text.o html.o latex.o memoir.o beamer.o lyx.o lyxbeamer.o opml.o odf.o critic.o rng.o rtf.o transclude.o]

$objs = mmd_objs.map { |s| "../MultiMarkdown-6/build/#{s}" } + ["ruby_multi_markdown.o"]

create_header
create_makefile('multi_markdown')
