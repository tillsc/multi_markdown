#   Copyright 2014 Till Schulte-Coerne
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "multi_markdown/version"

Gem::Specification.new do |s|
  s.name        = "multi_markdown"
  s.version     = MultiMarkdown::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Till Schulte-Coerne"]
  s.email       = ["till.schulte-coerne@innoq.com"]
  s.homepage    = "http://github.com/tillsc/multi_markdown"
  s.summary     = "A MultiMarkdown 4 binding for Ruby"
  s.description = s.summary
  s.extra_rdoc_files = ['README.md', 'LICENSE']

  s.add_dependency "bundler"

  s.files = %w(LICENSE README.md Rakefile multi_markdown.gemspec) + Dir.glob("{bin,lib,test}/**/*") + Dir.glob("{ext,MultiMarkdown-4}/*.{c,h,rb}")
  s.test_files = Dir.glob("{test}/**/*")
  s.executables = "ruby_multi_markdown"
  s.extensions = ['ext/extconf.rb']
  s.require_paths = ["lib"]
end
