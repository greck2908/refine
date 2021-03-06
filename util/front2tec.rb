#!/usr/bin/env ruby

starting_path = Dir.pwd

refine_path = "#{File.dirname $0}/../src"

$:.push refine_path

Dir.chdir refine_path

require 'RubyExtensionBuilder'
RubyExtensionBuilder.new('Grid').build
require 'Adj/Adj'
require 'Line/Line'
require 'Grid/Grid'

Dir.chdir starting_path


root_filename = ARGV[0]
front_filename = root_filename+'.front'
tec_filename = ARGV[1] || root_filename+'.t'
grid = Grid.from_front File.expand_path(front_filename)
grid.writeTecplotSurfaceGeom File.expand_path(tec_filename)

