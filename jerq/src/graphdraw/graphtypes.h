
#define INF	100000000
#define NULL	0
#define MAXLABELSIZE 40

struct node_entry {
    int degree;
    char label[MAXLABELSIZE];
    int x;
    int y;
    char graphics;
    struct edge_ent *adj_list;
    };
typedef struct node_entry *GRAPH;

struct edge_ent {
    int endpoint;
    char label[MAXLABELSIZE];
    char graphics;
    int label2;
    struct edge_ent *nextedge;
    struct edge_ent *prevedge;
    struct edge_ent *otheredge;
    };
typedef struct edge_ent *edge_entry;

extern GRAPH Readgraph(),NewGraph(),CopyGraph();

#define Degree(graph,n)    (graph[n].degree)
#define NLabel(graph,n)    (graph[n].label)
#define Xcoord(graph,n)    (graph[n].x)
#define Ycoord(graph,n)    (graph[n].y)
#define FirstEdge(graph,n) (graph[n].adj_list)
#define VertGraphics(graph,n) (graph[n].graphics)

#define EndPoint(e) (e->endpoint)
#define ELabel(e)   (e->label)
#define ELabel2(e)  (e->label2)
#define Other(e)    (e->otheredge)
#define NextEdge(e) (e->nextedge)
#define EdgeGraphics(e) (e->graphics)

