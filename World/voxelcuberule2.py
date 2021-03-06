cpp=""
tab=0

#import clipboard

def rotate(l,n):
    return l[n:] + l[:n]

def transform(a, b, c, t):
    v=[a,b,c]
    return (v[t[0]],v[t[1]],v[t[2]])

def addline(line):
    global cpp,tab
    cpp+="\t"*tab
    cpp+=line.replace("+-","-").replace("+0;", ";  ").replace("+0]","  ]")
    cpp+="\n"

def addlines(lines):
    for line in lines.split("\n"):
        addline(line)

addline("//////// C++ Generated by voxelcuberule.py ////////")

for d in [0,1,2]:
    t=rotate([0,1,2],d)
    varname = ['z','y','x'][d]
    for s in [-1,1]:
        compare = ['<','>'][(s+1)/2]
        addline("if (getVDataDefAtVC(x+%d,y+%d,z+%d).solid==0) {" % (transform(0,0,s,t)))
        tab+=1
        ### Following generates verts
        for (a,b) in [(1,1),(1,-1),(-1,-1),(-1,1)]:
            #addline("// normalbufferdata[i  ] = %d; normalbufferdata[i+1] = %d; normalbufferdata[i+2] = %d;" % transform(0,0,s,t))
            addline("drawp->colors.push_back(%s); drawp->colors.push_back(%s); drawp->colors.push_back(%s);" % ("r","g","b"))
            addline("drawp->normals.push_back(getCreateVDataPAtVC(x+%d,y+%d,z+%d)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+%d,y+%d,z+%d)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+%d,y+%d,z+%d)->znormalc);" % ((transform((s*a+1)/2, (b+1)/2, (s+1)/2, t))*3))
            addline("drawp->verts.push_back(x+%.1f); drawp->verts.push_back(y+%.1f); drawp->verts.push_back(z+%.1f);" % transform(s*.5*a, .5*b, s*.5,t))
            #addline("i+=nb;")
        tab-=1
        addline("}")

addline("////////       End of generated code       ////////")

print cpp
#clipboard.copy(cpp)
