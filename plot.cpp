#include <gvc.h>
#include <stdlib.h> // for atoi
#include <stdio.h>
#include <string>

int plot_vector(char* filename, int num_units, int* vector)
{
  Agraph_t *g, *sg;
  Agnode_t *feed;
  Agnode_t **m;
  Agedge_t *e;
  Agsym_t *a;
  GVC_t *gvc;
  
  m= (Agnode_t**) malloc((num_units+2)*sizeof(Agnode_t*));
  char s[128]; 
  int i;

  /* set up a graphviz context */
  gvc = gvContext();
  /* parse command line args - minimally argv[0] sets layout engine */
  /* Create a simple digraph */
  g = agopen("g", Agdirected, 0);

  int index = 0;
  index += sprintf(s,"vector: ");

  for (i=0; i<2*num_units+1; i++) {
    index += sprintf(&s[index], "%d ", vector[i]);
  }
  agsafeset(g, "label", s, "");
  agsafeset(g, "rankdir", "LR", "");
  agattr(g, AGNODE, "shape", "rectangle");

  sg = agsubg(g, "source", 1);
  agsafeset(sg, "rank", "source", "");
  feed = agnode(sg, "feed", 1);

  for (i=0;i<num_units;++i) {
    sprintf(s,"%ld", i);
    m[i] = agnode(g, s, 1);
  }
  
  sg = agsubg(g, "sink", 1);
  agsafeset(sg, "rank", "sink", "");

  m[num_units] = agnode(sg, "concentrate", 1);
  m[num_units+1] = agnode(sg, "tailings", 1);

  e = agedge(g, feed, m[vector[0]], 0, 1);
  agsafeset(e, "color", "black", "");

  for (i=0;i<num_units;++i) {
    e = agedge(g, m[i], m[vector[2*i+1]], 0, 1);
    agsafeset(e, "color", "blue", "");
    agsafeset(e, "tailport", "n", "");
    agsafeset(e, "headport", "w", "");
    e = agedge(g, m[i], m[vector[2*i+2]], 0, 1);
    agsafeset(e, "color", "red", "");
    agsafeset(e, "tailport", "s", "");
    agsafeset(e, "headport", "w", "");
  }
  /* Set an attribute - in this case one that affects the visible rendering */
  /* Compute a layout using layout engine from command line args */
  gvLayout(gvc, g, "dot");
  /* Write the graph according to -T and -o options */
  gvRenderFilename(gvc, g, "pdf", filename);
  /* Free layout data */
  gvFreeLayout(gvc, g);
  free(m);
  /* Free graph structures */
  agclose(g);
  /* close output file, free context, and return number of errors */
  return (gvFreeContext(gvc));
}


int main(int argc, char** argv) {

  int* vector=new int[argc-1];
  for (int i=1;i<argc;++i) {
    vector[i-1] = atoi(argv[i]);
  }
  plot_vector("example.pdf", (argc-1)/2, vector);

  delete[] vector;

}
