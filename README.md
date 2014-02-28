Ruby MultiMarkdown 4
====================

[![Build Status](https://travis-ci.org/tillsc/multi_markdown.png?branch=master)](https://travis-ci.org/tillsc/multi_markdown)

An extension library around
[Fletcher Penney's MultiMarkdown](http://github.com/fletcher/MultiMarkdown-4/)
library in C. It is based upon the ruby
[rpeg-markdown](https://github.com/rtomayko/rpeg-markdown/) and
[rpeg-multimarkdown](https://github.com/djungelvral/rpeg-multimarkdown) libraries by
[Ryan Tomayko](https://github.com/rtomayko) and  [Oliver "djungelvral"](https://github.com/djungelvral).

Synopsis
--------

    >> require 'multimarkdown'

    >> MultiMarkdown.new('Hello, world.').to_html
    #=> "<p>Hello, world.</p>"

    >> MultiMarkdown.new('_Hello World!_', :smart, :filter_html).to_html
    #=> "<p><em>Hello World!</em></p>"

    >> MultiMarkdown.new('_Hello World!_').to_latex
    #=> "\emph{Hello World!}"

	>> doc = MultiMarkdown.new("Title: Some document  \n\nSome text in the document")

    >> doc.metadata
    #=> {"title" => "Some document"}

    >> doc.metadata("Title")
    #=> "Some document"

See [MultiMarkdown documentation](http://fletcher.github.io/MultiMarkdown-4/)
and `MultiMarkdown`'s [RDoc](http://rubydoc.info/gems/multimarkdown) for further Details.

Installation / Hacking
----------------------

This library requires a recent version of glib2. All modern GNU userland
systems should be fine.

Install from [Rubygems](http://rubygems.org/gems/multimarkdown):

    $ [sudo] gem install rmultimarkdown

Bundle via [Bundler](http://bundler.io):

    gem 'rmultimarkdown'

Hacking:

    $ git clone --recursive git://github.com/tillsc/multi_markdown.git
    $ cd multi_markdown
    $ bundle install
    $ bundle exec rake test

Changes
-------

**Beware**: The versioning scheme isn't based upon 
[Semantic Versioning](http://semver.org)! The first three version numbers are
inherited from the underlying C library. Only the last number is used to indicate
changes in the Ruby wrapper itself.

  * [Version 4.5.0.1](http://github.com/tillsc/multi_markdown/tree/v4.5.0.1):
    First Version based upon  [rpeg-markdown](https://github.com/rtomayko/rpeg-markdown/)
    and [rpeg-multimarkdown](https://github.com/djungelvral/rpeg-multimarkdown).

COPYING
-------

MultiMarkdown-4, multi_markdown are both licensed under the GPL and the MIT License.
See [LICENSE](LICENCSE) for more information.
