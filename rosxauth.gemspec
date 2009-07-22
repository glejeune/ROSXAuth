Gem::Specification.new do |s|
  s.name = "rosxauth"
  s.version = "1.0.0"
  s.platform = Gem::Platform::RUBY

  s.authors = ["Gregoire Lejeune"]
  s.summary = %q{A Ruby class to give root execution privileges on MacOSX}
  s.email = %q{gregoire.lejeune@free.fr}
  s.homepage = %q{http://github.com/glejeune/ROSXAuth}
  s.description = %q{Ruby/XSLT is a simple class used to give root execution privileges on MacOSX}

  s.files = %w(ChangeLog COPYING README.rdoc AUTHORS test/test.rb lib/rosxauth/version.rb, ext/extconf.rb, ext/rosxauth.h, ext/rosxauth.c)
  s.require_path = "lib"
  s.extensions = FileList["ext/extconf.rb"].to_a

  s.rubyforge_project = 'rosxauth'
  s.has_rdoc = true
  s.extra_rdoc_files = ["README.rdoc", "ChangeLog", "COPYING", "AUTHORS"]
  s.rdoc_options = ["--title", "ROSXAuth", "--main", "README.rdoc", "--line-numbers"]
end
