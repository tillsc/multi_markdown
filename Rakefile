require 'rake/clean'
require 'rdoc/task'
require 'bundler'

Bundler::GemHelper.install_tasks

task :default => :test

# ***** Build

DLEXT = RbConfig::CONFIG['DLEXT']

# For Mac OS X -- prevents prevent additional ._* files being added to tarball
ENV['COPYFILE_DISABLE'] = 'true'

namespace "MultiMarkdown-4" do

  desc "Initialize the submodule"
  task "init" => "generate_parser"

  desc "Generate needed parser files"
  task "generate_parser" do
    chdir('MultiMarkdown-4') do
      sh 'make parser.c'
    end
  end

end


file 'ext/Makefile' => ["MultiMarkdown-4:init"] + FileList['ext/{extconf.rb,*.c,*.h,*.rb}', 'MultiMarkdown-4/*.{c,h}'] do
  chdir('ext') do
    ruby 'extconf.rb'
  end
end
CLEAN.include 'ext/Makefile'

file "ext/multi_markdown.#{DLEXT}" => FileList['ext/Makefile', 'ext/*.{c,h,rb}', 'MultiMarkdown-4/*.{c,h}'] do |f|
  chdir('ext') do
    sh 'make'
  end
end
CLEAN.include 'ext/*.{o,bundle,so}'
CLEAN.include 'MultiMarkdown-4/*.o'

file "lib/multi_markdown.#{DLEXT}" => "ext/multi_markdown.#{DLEXT}" do |f|
  cp f.prerequisites, "lib/", :preserve => true
end
CLEAN.include "lib/*.{so,bundle}"

desc 'Build the multi_markdown extension'
task :build => "lib/multi_markdown.#{DLEXT}"

# ***** Test

desc 'Run unit and conformance tests'
task :test => [ 'test:unit', 'test:conformance' ]

namespace :test do

  desc 'Run unit tests'
  task :unit => :build do |t|
    ruby 'test/multi_markdown_test.rb'
  end

  desc "Run conformance tests"
  task :conformance => :build do |t|
    script = "#{pwd}/bin/rmultimarkdown"
    chdir("MultiMarkdown-4/MarkdownTest") do
      sh "./MarkdownTest.pl --script='#{script}' --flags='-c' --tidy"
      sh "./MarkdownTest.pl --script='#{script}' --testdir='MultiMarkdownTests'"
      sh "./MarkdownTest.pl --script='#{script}' --testdir='MultiMarkdownTests' --flags='-t latex' --ext='.tex'"
      sh "./MarkdownTest.pl --script='#{script}' --testdir='BeamerTests' --flags='-t latex' --ext='.tex'"
      sh "./MarkdownTest.pl --script='#{script}' --testdir='MemoirTests' --flags='-t latex' --ext='.tex'"
    end
  end

end

# ***** RDoc

Rake::RDocTask.new do |rd|
  rd.main = "README.md"
  rd.rdoc_files.include("README.md", "ext/**/*.c", "lib/**/*.rb")
end
