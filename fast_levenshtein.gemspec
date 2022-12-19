require_relative 'lib/fast_levenshtein/version'

Gem::Specification.new do |spec|
  spec.name          = "fast_levenshtein"
  spec.version       = FastLevenshtein::VERSION
  spec.authors       = ["Dave Byrne"]
  spec.email         = ["davebyrne82@gmail.com"]

  spec.summary       = %q{Fast C implementation of the Levenshtein Distance algorithm}
  spec.description   = %q{Fast C implementation of the Levenshtein Distance algorithm}
  spec.homepage      = "https://github.com/davebyrne/ruby-fast-levenshtein"
  spec.required_ruby_version = Gem::Requirement.new(">= 2.3.0")

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage
  spec.metadata["changelog_uri"] = spec.homepage

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_development_dependency 'rake-compiler', '~> 1.2'
  spec.extensions = %w[ext/fast_levenshtein/extconf.rb]
end
