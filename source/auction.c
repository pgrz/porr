#include "auction.h"
#include "utils.h"

#include <limits.h>

void add_node(AuctionPath *ap, int node_id) {
    AuctionPathNode *current_last_node = ap->last;
    AuctionPathNode *new_node = malloc(sizeof(AuctionPathNode));
    memset(new_node, 0, sizeof(AuctionPathNode));

    if (current_last_node == 0) {
        /* dodawanie pierwszego węzła do ścieżki */
        ap->first = new_node;
    } else {
        current_last_node->next = new_node;
    }
    new_node->prev = current_last_node;
    ap->last = new_node;
    ap->count++;
}

void remove_last_node(AuctionPath *ap) {
    AuctionPathNode *current_last_node = ap->last;

    if (current_last_node == 0) {
        /* nic nie trzeba robić - ścieżka już jest pusta */
        return;
    }

    AuctionPathNode *previous_node = current_last_node->prev;

    if (previous_node == 0) {
        /* usuwamy ostatni węzeł */
        ap->first = 0;
        ap->last = 0;
    } else {
        previous_node->next = 0;
        ap->last = previous_node;
    }
    ap->count--;

    free(current_last_node);
}

AuctionPath *create_new_path(int first_node_id) {
    AuctionPath *new_path = malloc(sizeof(AuctionPath));
    memset(new_path, 0, sizeof(AuctionPath));

    add_node(new_path, first_node_id);

    return new_path;
}

void free_path(AuctionPath *ap) {
    while (ap->count > 0) {
        remove_last_node(ap);
    }

    free(ap);
}

/**
 * Wylicza minimalną ścieżkę od start_node do destination_node.
 * Wynik zapisywany jest w tablicy mind - mind[start_node].
 *
 * Wektor kosztów jest aktualizowany w trakcie trwania funkcji.
 */
void find_shortest_path(int adj[vertex_count][vertex_count],
                        int *price_v,
                        int *mind,
                        int start_node, 
                        int destination_node) {
/* FIXME - zaimplementować algorytm */
}

int *auction_distance (int adj[vertex_count][vertex_count], int destination_node)
{
    /* wektor kosztów */
    int *price_v;
    int i;

    /* minimalne odległości od wybranego węzła do całej reszty */
    int *mind;

    /* inicjalizacja wspólnego wektora kosztów */
    price_v = (int *) malloc ( vertex_count * sizeof(int));
    memset(price_v, 0, vertex_count * sizeof(int));

    mind = (int *) malloc ( vertex_count * sizeof(int)); 
    mind[destination_node]=INT_MAX; //obejście na potrzeby maina -- jeśli int_max to nie można dotrzeć

    /* początek algorytmu */
    for (i = 0; i < vertex_count; i++) {
        if (i == destination_node) {
            /* nie wyliczamy odległości od destination_node, do siebie samej */
            continue;
        }

        find_shortest_path(adj, price_v, mind, i, destination_node);
    }

    /* zwalnianie pamięci wektora kosztów */
    free(price_v);

    return mind;
}


