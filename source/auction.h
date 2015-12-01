/**
 * PORR - Projekt - Etap 1
 * Michał Cybulski, Piotr Grzegorski
 *
 * auction.h - Plik nagłówkowy z deklaracją struktur listy wierzchołków i
 * funkcji do obsługi algorytmu aukcyjnego.
 */

#ifndef __AUCTION_H__
#define __AUCTION_H__

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "generator.h"

int *auction_distance (int adj[vertex_count][vertex_count], int destination_node);
void find_nearest_a ( int s, int e, int mind[vertex_count], int connected[vertex_count], int *d,
                    int *v );
void update_mind_a ( int s, int e, int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count],
                   int mind[vertex_count] );

/**
 * Struktura przechowująca informacje o pojedynczym węźle w ścieżce P.
 */
typedef struct AuctionPathNode {
    struct AuctionPathNode *next;
    struct AuctionPathNode *prev;
    int id;
} AuctionPathNode;

/**
 * Struktura przechowująca informacje o całej ścieżce P.
 */
typedef struct AuctionPath {
    struct AuctionPathNode *first;
    struct AuctionPathNode *last;
    int count;
} AuctionPath;

#endif //__AUCTION_H__
