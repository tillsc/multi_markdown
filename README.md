Ruby MultiMarkdown 4
======================

An extension library around [Fletcher Penney's implementation][1]
of MultiMarkdown in C.

[1]: http://github.com/fletcher/MultiMarkdown-4/

Synopsis
--------

    >> require 'multi_markdown'

    >> puts MultiMarkdown.new('Hello, world.').to_html
    # <p>Hello, world.</p>

    >> puts MultiMarkdown.new('_Hello World!_', :smart, :filter_html).to_html
    # <p><em>Hello World!</em></p>

    >> puts MultiMarkdown.new('_Hello World!_').to_latex
    # \emph{Hello World!}

    >> puts MultiMarkdown.new("Title: Some document\n\nSome text in the document").extract_metadata("title")
    # Some document

    >> MultiMarkdown.new('Hello! World!')

Installation / Hacking
----------------------

This library requires a recent version of glib2. All modern GNU userland
systems should be fine.

Install from GEM:

    $ sudo gem install multi_markdown

Hacking:

    $ git clone --recursive git://github.com/tillsc/multi_markdown.git
    $ cd multi_markdown
    $ bundle install
    $ bundle exec rake test

Changes
-------

  * [Version 4.0.1](http://github.com/tillsc/multi_markdown/tree/v4.0.1): Initial port from
    [rpeg-markdown](https://github.com/rtomayko/rpeg-markdown/) and
    [rpeg-multimarkdown](https://github.com/djungelvral/rpeg-multimarkdown)

COPYING
-------

The MultiMarkdown-4, multi_markdown are both licensed under the GPL and the MIT License.
