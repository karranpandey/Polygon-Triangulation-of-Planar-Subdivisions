# Polygon-Triangulation-of-Planar-Subdivisions
Computational Geometry algorithms dealing with 2D planar sub divisions. First converts polygons to y-monotone and then triangulates y-monotone polygons

Introduction

We deal with some computational geometry problems relating to planar subdivisions in a 2D plane. Firstly, we implement a commonly used data structure, the Doubly Connected Edge List, which can be used to represent any planar subdivision accurately, both topologically and geometrically.

Using DCEL as our base data structure for polygons we go on to solve 2 problems closely connected to the Art Gallery Problem, Namely:

-Partitioning a Polygon into y-monotone polygons

-Triangulation of each y-monotone polygon

DCEL

This data structure is made keeping in mind that we want to store enough information so as to be able to reconstruct the figure using this information accurateely. We do this using a combination of geometric(the actual locations of the points and so on) and topological(which faces are bound by which edges, what is adjacent to what and so on) information so as to be as efficient as possible.

To do this we use three collections of data, for vertices, faces and edges.

- For vertices, we store the point co-ordinates and a pointer to an inner edge (an arbitrary half edge with the point as its origin)

- For faces, we store a pointer to an edge, Outer Component (any one half edge on it's outer boundary), and a list of pointers to edges Inner Components( contains for each hole in the face an arbitrary half edge on the boundary of the hole).

- For edges, we use half edges, (which are named such because we take one boundary to be associated two twin half edges having opposite directions). The record of a half edge stores a pointer to its twin half edge, one to the face that it bounds, one to its origin vertex, and two more to its previous and next half edge in the face that it bounds.

This information is enough to reconstruct the figure, as we can make each face using its half edges and moving to the next edge till we return to the required edge, and similarly for the holes, since each edge has it's origin co-ordinates we thus construct our required figure from the representation.

Partitioning a polygon into y-monotone polygons

This algorithm works by moving a sweep line from top to bottom, removing all split and merge vertices in a polygon along the way. It does this by constructing diagonals from these vertex to the lowest point above it for a split vertex and the highest point below it for a merge vertex. Based on the definition of split and merge vertices it is easy to see that the resulting polygons after adding the diagonals and removing all such vertices will be y-monotone. 

Method: 

The method uses a helper pointer for each edge, which points to the lowest vertex above the sweep line such that the horizontal line connecting the vertex to the edge lies inside the Polygon.

- For split vertices, we directly connect the vertex to the helper of the edge to its left as the helper is defined precisely in that way. 

- For merge vertices, the vertex it needs to be connected with is yet to be encountered. The merge vertex in question, when it is encountered becomes the helper of some edge, so we follow the sweep line down till another vertex replaces the merge vertex as the new helper, this vertex is the highest point below the merge vertex for which the diagonal lies inside of the polygon. So we connect the diagonal to remove the merge vertex. 

At the end of the process all split and merge vertices are gone and hence we have a y-monotone polygon.

Theoretical time analysis 

We use a DCEL for the polygon, and a priority queue (our status) to store the lines as we sweep down. For each event, according to our method we do a constant number of queries, insertions and deletions on the status and add a constant number of diagonals in the DCEL. Adding diagonals to the DCEL takes constant time, but queries on the status take O(logn) time as its a priority queue or balanced search tree. So for n points, we have n events and hence for the whole algorithm we can expect a complexity of O(nlogn).

Triangulating a y-monotone polygon

This algorithm traverses the vertices of the y monotone polygon from top to bottom (handling left first if they have the same y co-ordinate).
Every vertex it encounters it attempts to connect to all the previous vertices if the diagonal is inside the polygon. 
Following each iteration of this attempt, we are left with a loop invariant shape with two boundaries, the first consisting of all reflex vertices and the second consisting of a part of an edge of the polygon.
At the end we reach the vertex at the end of the second boundary, here it will be possible to join the vertex with all the reflex vertices on the first chain, thus resulting in a triangulated polygon. 

Method: 

To implement the concept we use an auxiliary data structure in the form of a stack S which contains all those vertices which have been encountered but still need more diagonals. 

When we handle a vertex, we add as many diagonals as we can from the vertex to all the vertices on the stack, as we successfully add diagonals it splits of triangles from the polygon, till the vertex where we can't add further diagonals. We add this vertex back onto S. 

To execute this process efficiently, 

We divide the vertices into two cases. 

1. If the vertex we are handling belongs to the same chain as the reflex vertices. 

We try to add diagonals to all the vertices that belong to the stack, as we successfully do this we keep popping the vertices from the stack (as all these vertices will be part of triangles that get cut off as a result of adding these diagonals), until we encounter a vertex with which we can't connect a diagonal inside the polygon, this vertex is pushed back on to the stack and we move on to the next vertex. 

2. If it is on the opposite chain. 

We can successfully add diagonals to all the vertices on the stack as it belongs to the other chain. After all the vertices are popped off the stack we re add first the previous to current vertex and then the current vertex as they are the only vertices that remain in our new loop invariant polygon, after this iteration.

As we can see both these cases preserve the loop invariant property and as mentioned intially handling the last vertex (which will be on the other chain) gives us a triangulated polygon.

Theoretical time analysis 

Every time we handle a vertex, the only main operation we do is pushing or popping elements on and off the stack. At each event, We do a push operation a maximum of twice, which bounds our push operation by O(2n), there can't be more pop operations than push operations in a stack and therefore, all the operations are bounded by O(2n) or in other words the algorithm runs in linear time, O(n).
