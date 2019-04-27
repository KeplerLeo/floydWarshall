#include<limits.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct {
    int sourceVertex, destVertex;
    int edgeWeight;
} edge;

typedef struct {
    int vertices, edges;
    edge* edgeMatrix;
} graph;

graph loadGraph() {
    FILE* fp = fopen("grafo.txt", "r");
    graph G;
    int i;

    fscanf(fp, "%d%d", &G.vertices, &G.edges);

    G.edgeMatrix = (edge*) malloc(G.edges * sizeof (edge));

    for (i = 0; i < G.edges; i++)
        fscanf(fp, "%d%d%d", &G.edgeMatrix[i].sourceVertex, &G.edgeMatrix[i].destVertex, &G.edgeMatrix[i].edgeWeight);
    fclose(fp);
    return G;
}

void floydWarshall(graph g) {
    int processWeights[g.vertices][g.vertices], processedVertices[g.vertices][g.vertices];
    int i, j, k;

    for (i = 0; i < g.vertices; i++)
        for (j = 0; j < g.vertices; j++) {
            processWeights[i][j] = SHRT_MAX;
            processedVertices[i][j] = (i != j) ? j + 1 : 0;
        }

    for (i = 0; i < g.edges; i++)
        processWeights[g.edgeMatrix[i].sourceVertex - 1][g.edgeMatrix[i].destVertex - 1] = g.edgeMatrix[i].edgeWeight;
    /*
     Adiciona todos os vértices um por um.
     Antes de começar a iteração existem distâncias mais curtas entre todos os pares de vértices, de modo que a distância
     represente apenas vértices do conjunto. 
     Após a iteração K é adicionado ao conjunto.
     */
    for (i = 0; i < g.vertices; i++)
        //Atribui todos os vértices como origem um por um
        for (j = 0; j < g.vertices; j++)
            //Atribui todos os vértices como destino para a origem 
            for (k = 0; k < g.vertices; k++) {
                //Se vértice k for o menor caminho entre I e J, então o valor é atualizado.
                if (processWeights[j][i] + processWeights[i][k] < processWeights[j][k]) {
                    processWeights[j][k] = processWeights[j][i] + processWeights[i][k];
                    processedVertices[j][k] = processedVertices[j][i];
                }
            }

    printf("pair    dist   path");
    for (i = 0; i < g.vertices; i++)
        for (j = 0; j < g.vertices; j++) {
            if (i != j) {
                printf("\n%d -> %d %3d %5d", i, j, processWeights[i][j], i);
                k = i + 1;
                do {
                    k = processedVertices[k - 1][j];
                    printf("->%d", k - 1);
                } while (k != j + 1);
            }
        }
}

int main(int argC, char* argV[]) {
    graph g = loadGraph();
    floydWarshall(g);


    return 0;
}
