#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct GRAPH_NODE_ *pnode;;


typedef struct edge_ {
    int weight;
    pnode startpoint;
    pnode endpoint;
    struct edge_ *next;
} edge, *pedge;



typedef struct GRAPH_NODE_ {
    int node_num;
    pedge edges;
    struct GRAPH_NODE_ *next;
} node, *pnode;

void build_graph_cmd(pnode *head);
void insert_node_cmd(pnode *head);
void delete_node_cmd(pnode *head, int numtodelete,int flag);
void deleteGraph_cmd(pnode* head);
int* shortsPath_cmd(pnode head,int* dist, int srcnum,int destnum,int flag);
void TSP_cmd(pnode head);
void print_heap(pedge *edges);
pnode createNode(int i);
pnode findNode(pnode *head,int nodeNum);
void addEdge(pnode src,pnode dest);
void freenode(pnode node);
void init_heap(pnode *head);

void heap_insert(pedge *edges, int i);

void heap_delete(pedge *edge, int n,int i);

int factorial(int k);

int nextInput(char *buf){
    if(strcmp(buf,"\n") == 0) return 0;
    int i = 0;
    int j = 0;
    char c = '\t';
    while(c != ' ' && c != 'n' && c != '\n'){
        scanf("%c",&c);
        buf[i++] = c;
        j++;
        if(j == 100){
            c = '\n';
            buf = strcpy(buf,"\n");
            return 1;
        }
    }
    if(c == 'n') getchar();
    buf[i] = '\0';
    if(c == '\n') return 0;
    return 1;
}
char buf[300];
int numofedges = 0;
int MaxNodeNum = 0;

int main(){
    pnode head = NULL;
    nextInput(buf);
    while(1) {
        if(strcmp(buf,"\n") == 0 ) break;
        if(strcmp(buf,"A ") == 0) {
            build_graph_cmd(&head);
            continue;
        }
        if(strcmp(buf,"B ") == 0) {
            insert_node_cmd(&head);
            continue;
        };
        if(strcmp(buf,"D ") == 0){
            nextInput(buf);
            int todelete = strtol(buf, NULL, 0);
            delete_node_cmd(&head ,todelete,1);
            continue;
        }
        if(strcmp(buf,"S ") == 0){
            nextInput(buf);
            int src = strtol(buf, NULL, 0);
            nextInput(buf);
            int dest = strtol(buf, NULL, 0);
            int *dist = (int*)malloc((MaxNodeNum+1)*sizeof(int));
            shortsPath_cmd(head,dist,src,dest,0);
            free(dist);
            continue;
        }
        if(strcmp(buf,"T ") == 0){
            TSP_cmd(head);
        }
        nextInput(buf);
    }
    deleteGraph_cmd(&head);
    free(head);
    return 0;
}

void build_graph_cmd(pnode *head) {
    if(*head != NULL) deleteGraph_cmd(head);
    nextInput(buf);
    int numOfNodes = atoi(buf);
    MaxNodeNum = numOfNodes-1;
    pnode cur = createNode(0);
    *head = cur;
    for (int i = 1; i < numOfNodes; i++) {
        pnode newnode = createNode(i);
        cur->next = newnode;
        cur = cur->next;
    }
    nextInput(buf);
    while (strcmp(buf, "T ") && strcmp(buf, "B ") && strcmp(buf, "A ") && strcmp(buf, "S ")&& strcmp(buf, "D ") && strcmp(buf, "\n")) {
        nextInput(buf);
        int src;
        int dest;
        if (strcmp(buf," ")) {
            src = strtol(buf, NULL, 0);
        }
        pnode srcnode = findNode(head,src);
        nextInput(buf);
        while (strcmp(buf, "n") && strcmp(buf, "T ") && strcmp(buf, "B ") && strcmp(buf, "A ") && strcmp(buf, "S ")&& strcmp(buf, "D ") && strcmp(buf,"\n")){
            dest = strtol(buf, NULL, 0);
            pnode destNode = findNode(head,dest);
            addEdge(srcnode,destNode);
            nextInput(buf);
        }
    }
}

pnode createNode(int i){
    pnode newnode = (pnode)calloc(1,sizeof(node));
    newnode->next = NULL;
    newnode->node_num = i;
    newnode->edges = NULL;
    return newnode;
}

pnode findNode(pnode *head, int nodeNum){
    pnode cur =*head;
    if(cur != NULL && cur->node_num == nodeNum){
        return cur;
    }
    while(cur != NULL && cur->node_num != nodeNum) cur = cur->next;
    return cur;
}

void addEdge(pnode src,pnode dest){
    numofedges++;
    nextInput(buf);
    int weight = strtol(buf, NULL, 0);
    pedge newedge = calloc(1,sizeof(edge));
    newedge->next = src->edges;
    newedge->startpoint = src;
    newedge->weight = weight;
    newedge->endpoint = dest;
    src->edges = newedge;
}

void delete_node_cmd(pnode *head, int numtodelete,int flag) {
    pnode todelete = findNode(head, numtodelete);
    pnode cur = *head;
    while(cur != NULL && flag == 1){
        pedge curedge = cur->edges;
        if(curedge == NULL){
            cur = cur->next;
            continue;
        }
        if(curedge->endpoint->node_num == numtodelete){
            pedge temp = curedge;
            cur->edges = cur->edges->next;
            cur = cur->next;
            free(temp);
            continue;
        }
        while(curedge->next != NULL && curedge->next->endpoint->node_num != numtodelete) curedge =curedge->next;
        if(curedge->next != NULL && curedge->next->endpoint->node_num == numtodelete){
            pedge temp = curedge->next;
            curedge->next = temp->next;
            free(temp);
        }
        cur = cur->next;
    }
    cur = *head;
    if(cur->node_num == numtodelete){
        (*head) = cur->next;
        freenode(todelete);
        return;
    }
    while(cur->next->node_num != numtodelete) cur=cur->next;
    cur->next=cur->next->next;
    freenode(todelete);
}

void freenode(pnode todelete){
    pedge curedge = todelete->edges;
    pedge next = NULL;
    while(curedge != NULL){
        next = curedge->next;
        free(curedge);
        numofedges--;
        curedge = next;
    }
    free(todelete);
}

void insert_node_cmd(pnode *head){
    pnode cur = *head;
    nextInput(buf);
    int nodeNum = strtol(buf, NULL, 0);
    if(nodeNum > MaxNodeNum) MaxNodeNum = nodeNum;
    if(findNode(head,nodeNum) != NULL){
        delete_node_cmd(head, nodeNum,0);
    }

    pnode newnode = createNode(nodeNum);
    while(cur->next != NULL && cur->next->node_num < nodeNum) cur=cur->next;
    newnode->next = cur->next;
    cur->next = newnode;

    nextInput(buf);
    while (strcmp(buf, "T ")&& strcmp(buf, "D ") && strcmp(buf, "B ") && strcmp(buf, "A ") && strcmp(buf, "S ") && strcmp(buf, "\n")){
        int dest = strtol(buf, NULL, 0);
        pnode destnode = findNode(head,dest);
        addEdge(newnode,destnode);
        nextInput(buf);
    }
}

void heap_delete(pedge *edges, int n, int i) {
    if (*edges == NULL) return;
    int smallest = i;
    int left = 2*i+1;
    int right = 2*i+2;

    if(left < n && edges[left]->weight<edges[smallest]->weight)
        smallest = left;

    if(right < n && edges[right]->weight < edges[smallest]->weight)
        smallest = right;

    if(smallest != i){
        pedge temp = edges[i];
        edges[i] = edges[smallest];
        edges[smallest] = temp;
        heap_delete(edges,n,smallest);
    }
}

void heap_insert(pedge *edges, int i){
    int j = i;
    pedge temp = edges[i];
    while(j >= 1 && edges[j/2] != NULL && edges[i]  != NULL && temp->weight < edges[j/2]->weight){
        edges[j] = edges[j/2];
        j=j/2;
    }
    edges[j] = temp;
}

int* shortsPath_cmd(pnode head,int *dist, int srcnum,int destnum,int flag){
    int src = srcnum;
    for(int i = 0 ; i <= MaxNodeNum ; i++) dist[i] = 9999;
    pnode srcnode = findNode(&head,src);
    pedge curedge = srcnode->edges;
    while(curedge != NULL){
        dist[curedge->endpoint->node_num] = curedge->weight;
        curedge = curedge->next;
    }
    pedge *edges = (pedge*)calloc(numofedges+1,sizeof(pedge));
    pnode cur = head;
    int i = 0;
    while(cur != NULL){
        pedge curedge = cur->edges;
        while(curedge != NULL){
            edges[i] = curedge;
            curedge = curedge->next;
            i++;
        }
        cur = cur->next;
    }

    for(i = 1 ; i < numofedges ; i++){
        heap_insert(edges,i);
    }
    int j = numofedges-1;
    while(j> 0){
        // printf("\nsrc: %d, dest: %d, weight: %d\n",edges[0]->startpoint->node_num,edges[0]->endpoint->node_num,edges[0]->weight);
        if(edges[0] == NULL) break;
        if(dist[edges[0]->startpoint->node_num]+edges[0]->weight < dist[edges[0]->endpoint->node_num])
            dist[edges[0]->endpoint->node_num] = dist[edges[0]->startpoint->node_num]+edges[0]->weight;
        edges[0] = edges[j];
        heap_delete(edges, j,0);
        j--;
    }


    if(flag == 0) {
        if (dist[destnum] < 9999) {
            printf("Dijsktra shortest path: %d \n", dist[destnum]);
        } else {
            printf("Dijsktra shortest path: -1 \n");
        }
    }
    free(edges);
    return dist;
}

void print_heap(pedge *edges) {
    printf("Min Heap:\n");
    for (int i=0; i< 8 ; i++) {
        printf("dest: %d, src: %d, weight:%d\n",edges[i]->endpoint->node_num,edges[i]->startpoint->node_num,edges[i]->weight);
    }
    printf("\n");
}
void addPermutation(int *arr1, int n, int *arr2) {
    int permutation = 0;
    int i;
    for(i = 0 ; i < n ; i++){
        permutation=permutation*10+arr1[i];
    }
    i = 0;
    while(*(arr2+i) != 0){
        i++;
    }
    *(arr2+i) = permutation;
}

void permutation(int arr[], int n, int cid, int arr2[]){
    if(cid == n) {
        addPermutation(arr,n, arr2);
        return;
    }

    for(int i = cid ; i < n ; i++){
        int temp = arr[i];
        arr[i] = arr[cid];
        arr[cid] = temp;
        permutation(arr, n, cid + 1, arr2);
        temp = arr[i];
        arr[i] = arr[cid];
        arr[cid] = temp;
    }
}
void TSP_cmd(pnode head){
    nextInput(buf);
    int k = strtol(buf, NULL, 0);
    pnode *nodes = calloc(k,sizeof(pnode));
    int nodeNum;
    for(int i = 0 ; i < k ; i++){
        nextInput(buf);
        nodeNum = strtol(buf, NULL, 0);
        pnode node = findNode(&head,nodeNum);
        nodes[i] = node;
    }
    int *helper = malloc(k*sizeof(int));
    for(int i = 0 ; i < k ; i++){
        helper[i] = i+1;
    }
    int *permutations = calloc(factorial(k),sizeof(int));
    permutation(helper,k,0,permutations);

    int **distances = (int**)calloc(k, sizeof(int*));
    for(int i = 0 ; i < k ; i++){
        distances[i] = (int*)malloc((MaxNodeNum+1)*sizeof(int));
    }
    for(int i = 0 ; i < k ; i++){
        shortsPath_cmd(head,distances[i],nodes[i]->node_num,0,1);
    }
    /* for(int i = 0 ; i < k ; i++){
         for(int j = 0 ; j <= MaxNodeNum ; j++){
             printf("%d ",distances[i][j]);
         }
         printf("\n");
     }*/
    int shortest = 9999;
    int fact = factorial(k);
    for(int i = 0 ; i < fact ; i++){
        int temp = permutations[i];
        int distance = 0;
        while(temp / 10 != 0){
            int from = temp %10;
            temp = temp / 10;
            int to = temp % 10;
            distance = distance + distances[from-1][nodes[to-1]->node_num];
        }
        if(distance < shortest) shortest = distance;
    }
    if(shortest< 9999){
        printf("TSP shortest path: %d \n", shortest);
    }
    else{
        printf("TSP shortest path: -1 \n");
    }
    for(int i = 0; i < k; i++)
        free(*(distances+i));
    free(distances);
    free(permutations);
    free(helper);
    free(nodes);


}

int factorial(int k) {
    int fact = 1;
    for(int i = 1 ; i <= k ; i++){
        fact=fact * i;
    }
    return fact;
}

void deleteGraph_cmd(pnode* head){
    pnode cur = *head;
    pnode next = NULL;
    while(cur != NULL){
        next = cur->next;
        delete_node_cmd(head,cur->node_num,0);
        cur = next;
    }

}
