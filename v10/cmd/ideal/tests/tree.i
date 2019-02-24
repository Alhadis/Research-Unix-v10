tree
.nf
.IS
box treenode
{
var ne, nw, sw, se,
    hook, lchild, rchild, cen;
ne = nw + width;
se = sw + width;
ne = se + (0,1)*height;
hook = (ne + nw)/2;
cen = (ne + sw)/2;
lchild = (sw + cen)/2;
rchild = (se + cen)/2;
conn ne to nw;
conn nw to sw;
conn sw to se;
conn se to ne;
conn (nw + sw)/2 to (ne + se)/2;
conn cen to (sw + se)/2;
}

box tree
{
var height, width;
height = ht/2;
width = 2*wd/3;
put node0:treenode{hook=root;};
put node1:treenode{hook=node0.hook-(0,1)*ht-wd;};
put node2:treenode{hook=node0.hook-(0,1)*ht+wd;};
put node3:treenode{hook=node2.hook-(0,1)*ht-wd;};
put node4:treenode{hook=node2.hook-(0,1)*ht+wd;};
conn node0.lchild to node1.hook;
conn node0.rchild to node2.hook;
conn node2.lchild to node3.hook;
conn node2.rchild to node4.hook;
}

box main
{
put T:tree{var root, ht, wd;
root = (1,-3);
ht = 1;
wd = 1;
};
}
.IE
.IS
...forget treenode tree
.IE
