require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require 'rake/testtask'
require 'fileutils'
require './lib/rosxauth/version'
include FileUtils

PKG_NAME = "rosxauth"
PKG_VERS = ROSXAuth::VERSION
PKG_FILES = %w(ChangeLog COPYING README.rdoc AUTHORS) +
 	      Dir.glob("{test,lib}/**/*") +
 	      Dir.glob("ext/*.{h,c,rb}")

CLEAN.include ['**/.*.sw?', '*.gem', '.config', 'test/test.log']
RDOC_OPTS = ['--quiet', '--title', "ROSXAuth, the Documentation",
    "--opname", "index.html",
    "--line-numbers",
    "--main", "README.rdoc",
    "--inline-source"]

desc "Packages up ROSXAuth."
task :default => [:package]
task :package => [:clean]

task :doc => [:rdoc, :after_doc]

Rake::RDocTask.new do |rdoc|
    rdoc.rdoc_dir = 'doc/rdoc'
    rdoc.options += RDOC_OPTS
    rdoc.main = "README.rdoc"
    rdoc.title = "ROSXAuth, the Documentation"
    rdoc.rdoc_files.add ['README.rdoc', 'AUTHORS', 'COPYING', 'ChangeLog',
      'ext/rosxauth.c']
end

spec =
    Gem::Specification.new do |s|
      s.name = PKG_NAME
      s.version = PKG_VERS
      s.platform = Gem::Platform::RUBY

      s.authors = ["Gregoire Lejeune"]
      s.summary = %q{A Ruby class to give root execution privileges on MacOSX}
      s.email = %q{gregoire.lejeune@free.fr}
      s.homepage = %q{http://github.com/glejeune/ROSXAuth}
      s.description = %q{Ruby/XSLT is a simple class used to give root execution privileges on MacOSX}

      s.files = PKG_FILES
      s.require_path = "lib"
      s.extensions = FileList["ext/extconf.rb"].to_a

      s.rubyforge_project = 'rosxauth'
      s.has_rdoc = true
      s.extra_rdoc_files = ["README.rdoc", "ChangeLog", "COPYING", "AUTHORS"]
      s.rdoc_options = ["--title", "ROSXAuth", "--main", "README.rdoc", "--line-numbers"]
    end

Rake::GemPackageTask.new(spec) do |p|
    p.need_tar = true
    p.gem_spec = spec
end

task :install do
  sh %{rake package}
  sh %{sudo gem install pkg/#{PKG_NAME}-#{PKG_VERS}}
end

task :uninstall => [:clean] do
  sh %{sudo gem uninstall #{PKG_NAME}}
end
