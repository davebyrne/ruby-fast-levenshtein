# frozen_string_literal: true

require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

task default: :spec


require "rake/extensiontask"

Rake::ExtensionTask.new "fast_levenshtein" do |ext|
  ext.lib_dir = "lib/fast_levenshtein"
end

