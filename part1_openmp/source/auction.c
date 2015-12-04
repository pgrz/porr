/**
 * PORR - Projekt - Etap 1
 * Michał Cybulski, Piotr Grzegorski
 *
 * auction.c - Główny plik z implementacją funkcji związanych z obsługą listy
 * wierzchołków oraz z implementacją współbieżnego algorytmu aukcyjnego przy
 * wykorzystaniu API OpenMP.
 */

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
                           volatile int *price_v,
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

void zero_memory(volatile int *array, int size) {
    int i;

    for (i = 0; i < size; i++) {
        array[i] = 0;
    }
}

int *auction_distance (int adj[vertex_count][vertex_count], int destination_node)
{
    // wektor kosztów/cen
    volatile int *price_v;
    // tablica przechowująca informacje o węzłach, które zostały usunięte ze ścieżek
    // w wyniku ostatniej iteracji
    volatile int *contracted;
    volatile int threads_finished = 0;
    int i;

    /* minimalne odległości od wybranego węzła do całej reszty */
    int *mind;

    log(INFO, -1, "Beginning of auction distance");

    /* inicjalizacja wspólnego wektora kosztów */
    price_v = (volatile int *) malloc ( vertex_count * sizeof(int));
    zero_memory(price_v, vertex_count);

    contracted = (volatile int *) malloc(vertex_count * sizeof(int));
    zero_memory(contracted, vertex_count);

    mind = (volatile int *) malloc ( vertex_count * sizeof(int)); 
    zero_memory(mind, vertex_count);
    mind[destination_node]=INT_MAX; //obejście na potrzeby maina -- jeśli int_max to nie można dotrzeć

    /* początek algorytmu */
    # pragma omp parallel \
    shared(adj, destination_node, mind, price_v, contracted, threads_finished) \
    private(i)
    {
        int thread_num = omp_get_thread_num();
        int thread_count = omp_get_num_threads();

        int my_first = (thread_num * vertex_count) / thread_count;                    
        int my_last = ((thread_num + 1) * vertex_count) / thread_count - 1;  
        int nodes_processed = 0;

        log(DEBUG, thread_num, "Thread %d/%d started! Processing nodes: %d-%d", 
                thread_num, thread_count, my_first, my_last);

        // sprawdzanie odległości kolejnych wierzchołków od celu
        for (i = my_first; i <= my_last; i++) {
            int destination_node_pass = 0;
            if (i == destination_node) {
                /* nie wyliczamy odległości od destination_node, do siebie samej */
                destination_node_pass = 1;
            }

            log(DEBUG, thread_num, "Looking for paths for node: %d", i);

            AuctionPath *auctionPath;
            int result = INT_MAX;
            int current_node = i;
            int result_length = 0;

            int finished_iteration = 0;
            int finished_everything = 0;
            auctionPath = create_new_path(current_node);
            while (!finished_iteration 
                    || (finished_iteration 
                        && finished_everything 
                        && threads_finished < thread_count)) {
                log(DEBUG, -1, "Thread finish: %d, thread count: %d", threads_finished, thread_count);
                int action_already_done = 0;
                int current_node = auctionPath->last->id;
                int nearest_neighbour = find_nearest_neighbour(adj, price_v, current_node);
                int dest_value = adj[current_node][nearest_neighbour] + price_v[nearest_neighbour];

                log(DEBUG, thread_num, "Current node: %d", current_node);
                log(DEBUG, thread_num, "Nearest neighbour: %d", nearest_neighbour);
                log(DEBUG, thread_num, "Destination value: %d", dest_value);

                // wszystkie wątki muszą skończyć iterację, a dopiero później możemy
                // synchronizować wektor kosztów/cen
                log(DEBUG, thread_num, "Waiting on first barrier");
                # pragma omp barrier
                log(DEBUG, thread_num, "Finished waiting on first barrier");

                // w pierwszej kolejności wykonać się muszą wszystkie skracania
                // ścieżek P, gdyż tylko one mogą powodać konflikty przy uaktualnieniu
                // wektora kosztów/cen. Każda operacja pisząca po wektorze cen musi
                // znaleźć się w sekcji krytycznej
                # pragma omp critical
                {
                    log(DEBUG, thread_num, "Critical section");
                    if (price_v[current_node] < dest_value 
                            && !destination_node_pass
                            && !finished_iteration) {
                        action_already_done = 1;
                        log(DEBUG, thread_num, "Contracting!");
                        price_v[current_node] = dest_value;
                        // oznaczenie węzła jako usuniętego z którejś ścieżki P
                        contracted[current_node] = 1;
                        if (current_node != auctionPath->first->id) {
                            remove_last_node(auctionPath);
                        }
                    }
                    log(DEBUG, thread_num, "Finishing critical section");
                }
                // musimy być pewni, że wszystkie wątki, które miały usunąć węzły
                // ze swoich ścieżek już to zrobiły
                log(DEBUG, thread_num, "Waiting on second barrier");
                # pragma omp barrier
                log(DEBUG, thread_num, "Finished waiting on second barrier");

                // sekcja krytyczna ze względu na zliczanie skończonych prac
                # pragma omp critical
                {
                    if (price_v[current_node] >= dest_value 
                            && !contracted[nearest_neighbour] 
                            && !action_already_done
                            && !finished_iteration) {
                        log(DEBUG, thread_num, "Extending to node: %d", nearest_neighbour);
                        add_node(auctionPath, nearest_neighbour);   
                    }
                    if (nearest_neighbour == destination_node
                            || destination_node_pass) {
                        // znaleziony koniec - wychodzimy z pętli while
                        finished_iteration = 1;
                        if (i == my_last && !finished_everything) {
                            log(DEBUG, thread_num, "Thread finished everything. Should wait for rest");
                            threads_finished++;
                            finished_everything = 1;
                        }
                    }
                }
                
                // przed wyczyszczeniem tablicy contracted trzeba się upewnić, że już
                // nie będzie używana 
                log(DEBUG, thread_num, "Waiting on third barrier");
                # pragma omp barrier
                log(DEBUG, thread_num, "Finished waiting on third barrier");

                # pragma omp single
                {
                    zero_memory(contracted, vertex_count);
                }
            }

            AuctionPathNode *capn = auctionPath->first;
            int finish = 0;
            while (!finish) {
                AuctionPathNode *apn = capn->next;
                if (apn == 0) {
                    finish = 1;
                } else {
                    result_length += adj[capn->id][apn->id];
                    capn = apn;
                }
            }
            // nie wystąpią wyścigi - każdy wątek pisze pod własną komórkę pamięci
            mind[i] = result_length;

            nodes_processed++;
            log(DEBUG, thread_num, "Finished looking for path for node: %d. It was %d/%d.",
                    i, nodes_processed, my_last - my_first + 1);

            free_path(auctionPath);
        }

        log(DEBUG, thread_num, "Waiting on last barrier");
        # pragma omp barrier
        log(DEBUG, thread_num, "Passed last barrier");
    }
    
    
    /* zwalnianie pamięci wektora kosztów */
    log(DEBUG, -1, "Freeing!");
    free(price_v);
    free(contracted);

    return mind;
}


