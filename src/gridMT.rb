#!/usr/bin/env ruby
#
# $Id$
#
# Mobility test for grid c lib

exit unless system 'ruby extconf.rb Grid'
exit unless system 'make'

require 'test/unit'
require 'Grid'

class TestSampleUnit < Test::Unit::TestCase

 def set_up
  @grid = Grid.new(4,1,0)
 end

 def testCreateGrid
  assert_equal 4, @grid.nnode
  assert_equal 1, @grid.maxcell
  assert_equal 0, @grid.ncell
 end

 def testNodeCellDeg
  assert_equal 0, @grid.nodeDeg(0)
  @grid.registerNodeCell( 0, 299 )
  assert_equal 1, @grid.nodeDeg(0)
 end

 def testNodeCellIterator
  assert_equal false, @grid.validNodeCell
  assert_equal false, @grid.moreNodeCell
  
  @grid.firstNodeCell(0);
  assert_equal false, @grid.validNodeCell
  
  @grid.registerNodeCell( 2, 299 )
  @grid.firstNodeCell(2);
  assert_equal 299, @grid.currentNodeCell
  assert_equal true,  @grid.validNodeCell
  assert_equal false, @grid.moreNodeCell
  @grid.nextNodeCell
  assert_equal false, @grid.validNodeCell
  
  @grid.registerNodeCell( 3, 398 )
  @grid.registerNodeCell( 3, 399 )
  @grid.firstNodeCell(3);
    assert_equal true,  @grid.validNodeCell
  assert_equal true,  @grid.moreNodeCell
  
  100.times {@grid.nextNodeCell} # abusive use of next
 end
 
 def XtestAddAndRemoveNodeCell
  assert_equal false, @grid.cellExists(1,0)
  assert_equal nil,   @grid.removeNodeCell(1,0)
  assert_equal @grid, @grid.registerNodeCell(1,0)
  assert_equal true,  @grid.cellExists(1,0)
  assert_equal @grid, @grid.removeNodeCell(1,0)
  assert_equal false, @grid.cellExists(1,0)
  assert_equal nil,   @grid.removeNodeCell(1,0)
 end
 
 def XtestRegisterNodeCellLimit
  assert_equal nil, @grid.registerNodeCell(1000,0)
 end

 def XtestEfficientStorage
  grid = Grid.new(1,1,1)
  assert_equal nil, grid.registerNodeCell(0,0)
  grid = Grid.new(1,1,2)
  assert_equal nil, grid.registerNodeCell(0,0)
  grid = Grid.new(1,1,3)
  assert_equal grid, grid.registerNodeCell(0,0)
  grid = Grid.new(1,1,4)
  assert_equal grid, grid.registerNodeCell(0,0)
  assert_equal grid, grid.registerNodeCell(0,1)
  assert_equal nil,  grid.registerNodeCell(0,2)
 end

 def XtestPackStorageSetFirstCell
  grid = Grid.new(2,1,5)
  grid.pack
  assert_equal grid, grid.registerNodeCell(0,0)
  assert_equal grid, grid.registerNodeCell(1,1)
  grid.removeNodeCell(1,1)
  assert_equal nil, grid.registerNodeCell(0,1)
  grid.pack
  assert_equal true, grid.cellExists(0,0)
  assert_equal false, grid.cellExists(0,1)
  assert_equal grid, grid.registerNodeCell(0,1)
  assert_equal true, grid.cellExists(0,1)
 end
 
 def XtestPackStorageReset
  grid = Grid.new(1,1,5)
  assert_equal grid, grid.registerNodeCell(0,0)
  assert_equal grid, grid.registerNodeCell(0,1)
  grid.removeNodeCell(0,0)
  grid.removeNodeCell(0,1)
  grid.pack
  assert_equal grid, grid.registerNodeCell(0,1)
  assert_equal grid, grid.registerNodeCell(0,2)
  assert_equal grid, grid.registerNodeCell(0,3)
 end
 
 def XtestPackScattered
  grid = Grid.new(2,1,5)
  assert_equal grid, grid.registerNodeCell(1,0)
  assert_equal grid, grid.registerNodeCell(0,1)
  grid.dump
  grid.pack
  grid.dump
  assert_equal true, grid.cellExists(1,0)
  assert_equal true, grid.cellExists(0,1)
 end
 
 def XtestMultipleNodeCellExists
  assert_equal false, @grid.cellExists(1,198)
  @grid.registerNodeCell(1,198)
  @grid.registerNodeCell(2,198)
  @grid.registerNodeCell(1,199)
  
  assert_equal true,  @grid.cellExists(1,198)
  assert_equal true,  @grid.cellExists(1,199)
  @grid.removeNodeCell(1,198)
  assert_equal false, @grid.cellExists(1,198)
  assert_equal true,  @grid.cellExists(1,199)
  @grid.registerNodeCell(1,198)
  assert_equal true,  @grid.cellExists(1,198)
  assert_equal true,  @grid.cellExists(1,199)
 end
 
 def XtestAddCell
  assert_equal 0, @grid.ncell
  @grid.addCell(0,1,2,3)
  assert_equal 1, @grid.ncell
  (0..3).each { |n| assert_equal 1, @grid.nodeDeg(n)}
 end
 
 def XtestGetGem1
  grid = @grid
  grid.addCell(0,1,2,3)
  gem = grid.getGem(0,1)
  assert_equal [0], gem
 end
 
 def XtestGetGem2
  grid = Grid.new(5,2,20)
  grid.addCell(0,1,2,3)
  grid.addCell(0,1,2,4)
  gem = grid.getGem(0,1)
  assert_equal [0, 1], gem
 end
 
 def XtestEquator
  grid = Grid.new(6,4,0)
  assert_equal grid, grid.addCell(4,5,0,1).addCell(4,5,1,2).addCell(4,5,2,3).addCell(4,5,3,0)
#  grid.dump
  grid.pack
  assert_equal 2, grid.nodeDeg(0)
 end

 # adding a cell bigger than maxcell
 
 # make register unique

 # allocating a new chunk of celllist

end
