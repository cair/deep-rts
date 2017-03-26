# Copyright 2004 Toby Dickenson
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject
# to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


import sys, getopt, colorsys, imp
import hashlib

import time


class pydepgraphdot:

    def main(self, argv):
        self.data = argv
        self.colored = 0
        self.render()

    def fix(self,s):
        # Convert a module name to a syntactically correct node name
        return s.replace('.','_')
    
    def render(self):
        p , t = self.get_data()

        # normalise our input data
        changed = []
        for k,d in p.items():
            for v in d.keys():

                if not v in p:
                    changed.append(v)

        for ch in changed:
            p[ch] = {}
                    
        f = self.get_output_file()                    
                    
        f.write('digraph G {\n')
        #f.write('concentrate = true;\n')
        #f.write('ordering = out;\n')
        f.write('ranksep=1.0;\n')
        f.write('node [style=filled,fontname=Helvetica,fontsize=10];\n')
        allkd = p.items()
        sorted(allkd)
        for k,d in allkd:
            tk = t.get(k)
            if self.use(k,tk):
                allv = d.keys()
                sorted(allv)
                for v in allv:
                    tv = t.get(v)
                    if self.use(v,tv) and not self.toocommon(v,tv):
                        f.write('%s -> %s' % ( self.fix(k),self.fix(v) ) )
                        self.write_attributes(f,self.edge_attributes(k,v))
                        f.write(';\n')
                f.write(self.fix(k))
                self.write_attributes(f,self.node_attributes(k,tk))
                f.write(';\n')
        f.write('}\n')

    def write_attributes(self,f,a):
        if a:
            f.write(' [')
            f.write(','.join(a))
            f.write(']')

    def node_attributes(self,k,type):
        a = []
        a.append('label="%s"' % self.label(k))
        if self.colored:
            a.append('fillcolor="%s"' % self.color(k,type))
        else:
            a.append('fillcolor=white')
        if self.toocommon(k,type):
            a.append('peripheries=2')
        return a
                
    def edge_attributes(self,k,v):
        a = []
        weight = self.weight(k,v)
        if weight!=1:
            a.append('weight=%d' % weight)
        length = self.alien(k,v)
        if length:
            a.append('minlen=%d' % length)
        return a
            
    def get_data(self):
        return self.data['depgraph'],self.data['types']
    
    def get_output_file(self):
        return open("dependency_graph_%s.dot" % (time.time()), "w")

    def use(self,s,type):
        # Return true if this module is interesting and should be drawn. Return false
        # if it should be completely omitted. This is a default policy - please override.
        if s in ('os','sys','qt','time','__future__','types','re','string'):
            # nearly all modules use all of these... more or less. They add nothing to
            # our diagram.
            return 0
        if s.startswith('encodings.'):
            return 0
        if s=='__main__':
            return 1
        if self.toocommon(s,type):
            # A module where we dont want to draw references _to_. Dot doesnt handle these
            # well, so it is probably best to not draw them at all.
            return 0
        return 1

    def toocommon(self,s,type):
        # Return true if references to this module are uninteresting. Such references
        # do not get drawn. This is a default policy - please override.
        #
        if s=='__main__':
            # references *to* __main__ are never interesting. omitting them means
            # that main floats to the top of the page
            return 1
        if type==imp.PKG_DIRECTORY:
            # dont draw references to packages.
            return 1
        return 0
        
    def weight(self,a,b):
        # Return the weight of the dependency from a to b. Higher weights
        # usually have shorter straighter edges. Return 1 if it has normal weight.
        # A value of 4 is usually good for ensuring that a related pair of modules 
        # are drawn next to each other. This is a default policy - please override.
        #
        if b.split('.')[-1].startswith('_'):
            # A module that starts with an underscore. You need a special reason to
            # import these (for example random imports _random), so draw them close
            # together
            return 4
        return 1
    
    def alien(self,a,b):
        # Return non-zero if references to this module are strange, and should be drawn
        # extra-long. the value defines the length, in rank. This is also good for putting some
        # vertical space between seperate subsystems. This is a default policy - please override.
        #
        return 0

    def label(self,s):
        # Convert a module name to a formatted node label. This is a default policy - please override.
        #
        return '\\.\\n'.join(s.split('.'))

    def color(self,s,type):
        # Return the node color for this module name. This is a default policy - please override.
        #
        # Calculate a color systematically based on the hash of the module name. Modules in the
        # same package have the same color. Unpackaged modules are grey
        t = self.normalise_module_name_for_hash_coloring(s,type)
        return self.color_from_name(t)
        
    def normalise_module_name_for_hash_coloring(self,s,type):
        if type==imp.PKG_DIRECTORY:
            return s
        else:
            i = s.rfind('.')
            if i<0:
                return ''
            else:
                return s[:i]
        
    def color_from_name(self,name):
        n = hashlib.md5(name).digest()
        hf = float(ord(n[0])+ord(n[1])*0xff)/0xffff
        sf = float(ord(n[2]))/0xff
        vf = float(ord(n[3]))/0xff
        r,g,b = colorsys.hsv_to_rgb(hf, 0.3+0.6*sf, 0.8+0.2*vf)
        return '#%02x%02x%02x' % (r*256,g*256,b*256)


def main(data=None):
    if data is None:
        pydepgraphdot().main(sys.argv[1:])
    else:
        pydepgraphdot().main(data)

if __name__=='__main__':
    main()



