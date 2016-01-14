/**
 * PORR - Projekt - Etap 1
 * Michał Cybulski, Piotr Grzegorski
 *
 * auction.c - Główny plik z implementacją funkcji związanych z obsługą listy
 * wierzchołków oraz z implementacją współbieżnego algorytmu aukcyjnego przy
 * wykorzystaniu API OpenMPI.
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

void zero_memory(int *array, int size) {
    int i;

    for (i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void max_memory(int *array, int size) {
    int i;

    for (i = 0; i < size; i++) {
        array[i] = INT_MAX;
    }
}

void copy_array(int *src, int *dest, int size) {
    int i;

    for (i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int *auction_distance (int adj[vertex_count][vertex_count], int destination_node)
{
    // wektor kosztów/cen
    int *price_v;
    int *common_price_v;
    // tablica przechowująca informacje o węzłach, które zostały usunięte ze ścieżek
    // w wyniku ostatniej iteracji
    int *contracted;
    int *common_contracted;
    int threads_finished = 0;
    int i;

    /* minimalne odległości od wybranego węzła do całej reszty */
    int *mind;
    int *common_mind;

    log_i(tid, "Beginning of auction distance");

    /* inicjalizacja wspólnego wektora kosztów */
    price_v = (int *) malloc ( vertex_count * sizeof(int));
    common_price_v = (int *) malloc ( vertex_count * sizeof(int));
    zero_memory(price_v, vertex_count);
    zero_memory(common_price_v, vertex_count);

    contracted = (int *) malloc(vertex_count * sizeof(int));
    common_contracted = (int *) malloc(vertex_count * sizeof(int));
    zero_memory(contracted, vertex_count);

    mind = (int *) malloc ( vertex_count * sizeof(int)); 
    common_mind = (int *) malloc ( vertex_count * sizeof(int));
    max_memory(mind, vertex_count);

    /* początek algorytmu */
    int my_first = (tid * vertex_count) / ntasks;                    
    int my_last = ((tid + 1) * vertex_count) / ntasks - 1;  
    int nodes_processed = 0;

    log_d(tid, "Thread %d/%d started! Processing nodes: %d-%d", 
            tid + 1, ntasks, my_first, my_last);

    // sprawdzanie odległości kolejnych wierzchołków od celu
    for (i = my_first; i <= my_last; i++) {
        int destination_node_pass = 0;
        if (i == destination_node) {
            /* nie wyliczamy odległości od destination_node, do siebie samej */
            destination_node_pass = 1;
        }

        log_d(tid, "Looking for paths for node: %d", i);

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
                    && threads_finished < ntasks)) {
            log_d(tid, "Thread finish: %d, thread count: %d", threads_finished, ntasks);
            int action_already_done = 0;
            int current_node = auctionPath->last->id;
            int nearest_neighbour = find_nearest_neighbour(adj, price_v, current_node);
            int dest_value = adj[current_node][nearest_neighbour] + price_v[nearest_neighbour];

            log_d(tid, "Current node: %d", current_node);
            log_d(tid, "Nearest neighbour: %d", nearest_neighbour);
            log_d(tid, "Destination value: %d", dest_value);

            // w pierwszej kolejności wykonać się muszą wszystkie skracania
            // ścieżek P, gdyż tylko one mogą powodać konflikty przy uaktualnieniu
            // wektora kosztów/cen
            if (price_v[current_node] < dest_value 
                    && !destination_node_pass
                    && !finished_iteration) {
                action_already_done = 1;
                log_d(tid, "Contracting!");
                price_v[current_node] = dest_value;
                // oznaczenie węzła jako usuniętego z którejś ścieżki P
                contracted[current_node] = 1;
                if (current_node != auctionPath->first->id) {
                    remove_last_node(auctionPath);
                }
            }

            // po ewentualnym usunięciu ze ścieżek węzłów można synchronizować
            // wektory cen i informacje o usuniętych węzłach
            if(MPI_Allreduce(price_v, common_price_v, vertex_count, MPI_INT, MPI_MAX, MPI_COMM_WORLD) != MPI_SUCCESS)
            {                                                                       
                log_e(tid, "MPI_Allreduce failed!");                                
                //TODO obsłużyć błąd!                                               
            } 
            copy_array(common_price_v, price_v, vertex_count);

            if(MPI_Allreduce(contracted, common_contracted, vertex_count, MPI_INT, MPI_MAX, MPI_COMM_WORLD) != MPI_SUCCESS)
            {                                                                       
                log_e(tid, "MPI_Allreduce failed!");                            
                //TODO obsłużyć błąd!                                               
            }                                                                   
            copy_array(common_contracted, contracted, vertex_count);  

            if (price_v[current_node] >= dest_value 
                    && !contracted[nearest_neighbour] 
                    && !action_already_done
                    && !finished_iteration) {
                log_d(tid, "Extending to node: %d", nearest_neighbour);
                add_node(auctionPath, nearest_neighbour);   
            }
            if (nearest_neighbour == destination_node
                    || destination_node_pass) {
                // znaleziony koniec - wychodzimy z pętli while
                finished_iteration = 1;
                if (i == my_last && !finished_everything) {
                    log_d(tid, "Thread finished everything. Should wait for rest");
                    finished_everything = 1;
                }
            }

            // synchronizacja informacji o ukończeniu pracy 
            if(MPI_Allreduce(&finished_everything, &threads_finished, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD) != MPI_SUCCESS)
            {                                                                   
                log_e(tid, "MPI_Allreduce failed!");                            
                //TODO obsłużyć błąd!                                               
            }    

            // czyszczenie informacji o węzłach usuniętych w tym kroku
            zero_memory(contracted, vertex_count);
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
        // zapisanie wyniku dla badanego węzła
        mind[i] = result_length;

        nodes_processed++;
        log_d(tid, "Finished looking for path for node: %d. It was %d/%d.",
                i, nodes_processed, my_last - my_first + 1);

        free_path(auctionPath);
    }

    /* zwalnianie pamięci wektora kosztów */
    log_d(tid, "Freeing!");
    free(price_v);
    free(contracted);

    if(MPI_Allreduce(mind, common_mind, vertex_count, MPI_INT, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS)
    {                                                                   
        log_e(tid, "MPI_Allreduce failed!");                            
        //TODO obsłużyć błąd!                                               
    } 
    free(mind);

    return common_mind;
}


