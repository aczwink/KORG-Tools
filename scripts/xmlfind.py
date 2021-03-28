import xml.etree.ElementTree;
import sys;

parent_map = {}
def build_parent_map(node):
	for c in node:
		parent_map[c] = node;
		build_parent_map(c);
		
def print_tabs(count):
	return "\t" * count;

def print_parents(node):
	parent = parent_map[node];
	if(parent is None):
		count = 0;
	else:
		count = print_parents(parent);
	
	print(print_tabs(count), "<" + node.tag + ">");
	
	return count + 1;
	
def print_parents_closing(node, count):
	if(count == 0):
		return;
	count -= 1;
		
	print(print_tabs(count), "</" + node.tag + ">");
	
	print_parents_closing(parent_map[node], count);

input = sys.argv[1];

tree = xml.etree.ElementTree.ElementTree();
root = tree.parse(input);
build_parent_map(root);
parent_map[root] = None;

for node in root.findall(".//" + sys.argv[2]):
	parent = parent_map[node];
	nParents = print_parents(parent);
	print(print_tabs(nParents), end="");
	print(xml.etree.ElementTree.tostring(node,  encoding="unicode"));
	print_parents_closing(parent, nParents);
