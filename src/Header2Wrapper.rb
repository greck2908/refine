# Ruby C extension build for refine package
#
# $Id$

class Header2Wrapper

 def initialize object
  @object = object
 end

 def convert
  basename = @object.downcase
  headername = basename+'.h'
  header = IO.readlines(headername)

  File.open(basename+'_ruby.c','w') do |f|
   f.puts <<-HEADER

/* autogenerated wrapper file created from ${headername} with ${__FILE__} */

#include "ruby.h"
#include "#{headername}"
 
   HEADER
   f.puts <<-FOOTER

VALUE c#{@object};
 
void Init_#{@object}()
{
   c#{@object} = rb_define_module( "#{@object}" );
}

   FOOTER
  end
 end

end
