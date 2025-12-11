require 'rake/clean'
require 'rdoc/task'
require 'bundler'
require 'fileutils'
require 'rake/testtask'

Bundler::GemHelper.install_tasks

task :default => "test:unit"

task :release => ["MultiMarkdown-6:init", "test:unit"]

# ***** Build

DLEXT = RbConfig::CONFIG['DLEXT']
MMD_DIR = File.expand_path("../ext/mmd", __FILE__)

# For Mac OS X -- prevents additional ._* files being added to tarball
ENV['COPYFILE_DISABLE'] = 'true'

namespace "MultiMarkdown-6" do

  desc "Initialize the submodule"
  task "init" do
    FileUtils.rm_rf(MMD_DIR)
    chdir('MultiMarkdown-6') do
      sh 'CMAKE_POLICY_VERSION_MINIMUM=3.5 make' # creates build/version.h

      # Copy all c and h files to MMD_DIR which will be released in the gem
      ['src', 'build'].each do |dir|
        chdir(dir) do
          Dir.glob('{.,include}/*.{h,c}').each do |s|
            next if s =~ /template|char_lookup/
            dest = "#{MMD_DIR}/#{File.dirname(s)}"
            FileUtils.mkdir_p(dest)
            FileUtils.cp(s, dest)
          end
        end
      end
    end
  end

end

file 'ext/Makefile' => FileList['ext/**/{extconf.rb,*.c,*.h,*.rb}'] do
  chdir('ext') do
    ruby 'extconf.rb'
  end
end
CLEAN.include 'ext/Makefile'

file "ext/multi_markdown.#{DLEXT}" => FileList['ext/Makefile', 'ext/**/*.{c,h,rb}'] do |f|
  chdir('ext') do
    sh 'make'
  end
end
CLEAN.include 'ext/**/*.{o,bundle,so}'

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
  Rake::TestTask.new(:unit => :build) do |t|
    t.libs << 'lib'
    t.libs << 'test'
    t.test_files = FileList['test/**/*_test.rb']
  end

  desc "Run conformance tests"
  task :conformance => :build do |t|
    script = "#{pwd}/bin/rmultimarkdown"
    chdir("MultiMarkdown-6/tests") do
      sh "./MarkdownTest.pl --script='#{script}' --testdir='MMD6Tests'"
    end
  end

end

# ***** RDoc

Rake::RDocTask.new do |rd|
  rd.main = "README.md"
  rd.rdoc_files.include("README.md", "ext/**/*.c", "lib/**/*.rb")
end
