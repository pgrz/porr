#include "auction.h"
#include "utils.h"

#include <limits.h>

void add_node(AuctionPath *ap, int node_id) {
    AuctionPathNode *current_last_node = ap->last;
    AuctionPathNode *new_node = malloc(sizeof(AuctionPathNode));
    memset(new_node, 0, sizeof(AuctionPathNode));

    new_node->id = node_id;

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

int find_nearest_neighbour(int adj[vertex_count][vertex_count],
                           int *price_v,
                           int node_id) {
    int min_value = INT_MAX;
    int min_dest_node;
    int i;
    int current_value;

    for (i = 0; i < vertex_count; i++) {
        if (i == node_id) {
            continue;
        }

        int adj_val = adj[node_id][i];
        if (adj_val == INT_MAX) {
            current_value = INT_MAX;
        } else {
            current_value = adj_val + price_v[i];
        }

        if (current_value <= min_value) {
            min_value = current_value;
            min_dest_node = i;
        }
    }

    return min_dest_node;
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
    AuctionPath *auctionPath;
    int result = INT_MAX;
    int current_node = start_node;
    int result_length = 0;
    
    auctionPath = create_new_path(start_node);
    while (1) {
        int current_node = auctionPath->last->id;
        int nearest_neighbour = find_nearest_neighbour(adj, price_v, current_node);
        int dest_value = adj[current_node][nearest_neighbour] + price_v[nearest_neighbour];

        log(DEBUG, -1, "Current node: %d", current_node);
        log(DEBUG, -1, "Nearest neighbour: %d", nearest_neighbour);
        log(DEBUG, -1, "Destination value: %d", dest_value);

        if (price_v[current_node] < dest_value) {
            log(DEBUG, -1, "Contracting!");
            price_v[current_node] = dest_value;
            if (current_node != auctionPath->first->id) {
                remove_last_node(auctionPath);
            }
        } else {
            log(DEBUG, -1, "Extending to node: %d", nearest_neighbour);
            add_node(auctionPath, nearest_neighbour);   
            if (nearest_neighbour == destination_node) {
                break;
            }
        }
    }

    AuctionPathNode *capn = auctionPath->first;
    while (1) {
        AuctionPathNode *apn = capn->next;
        if (apn == 0) {
            break;
        }

        result_length += adj[capn->id][apn->id];
        capn = apn;
    }
    mind[start_node] = result_length;
}

int *auction_distance (int adj[vertex_count][vertex_count], int destination_node)
{
    /* wektor kosztów */
    int *price_v;
    int i;

    /* minimalne odległości od wybranego węzła do całej reszty */
    int *mind;

    log(INFO, -1, "Beginning of auction distance");

    /* inicjalizacja wspólnego wektora kosztów */
    price_v = (int *) malloc ( vertex_count * sizeof(int));
    memset(price_v, 0, vertex_count * sizeof(int));

    mind = (int *) malloc ( vertex_count * sizeof(int)); 
    mind[destination_node]=INT_MAX; //obejście na potrzeby maina -- jeśli int_max to nie można dotrzeć

    /* początek algorytmu */
    // FIXME - ustawić na 0 podczas implementacji równoległej
    for (i = vertex_count - 1; i < vertex_count; i++) {
        if (i == destination_node) {
            /* nie wyliczamy odległości od destination_node, do siebie samej */
            continue;
        }
        // FIXME usunąć podczas implementacji równoległej
        memset(price_v, 0, vertex_count * sizeof(int));

        log(INFO, -1, "Looking for paths for node: %d", i);
        find_shortest_path(adj, price_v, mind, i, destination_node);
        log(INFO, -1, "Finished looking for path for node: %d", i);
    }

    /* zwalnianie pamięci wektora kosztów */
    free(price_v);

    return mind;
}


