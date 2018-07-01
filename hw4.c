/*
 * Title: Cache simulator
 * Compile: gcc hw4.c -std=gnu99 -lm -o hw4
 * Run: ./hw4 –input inputfile –output outputfile
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>           //require -lm

struct Cache_direct {
    short valid[1];
    int tag[1];
    int rear;
    int LRU[1];
};

struct Cache_four {
    short valid[4];
    int tag[4];
    int rear;
    int LRU[4];
};

struct Cache_fully {
    short valid[10000];
    int tag[10000];
    int rear;
    int LRU[10000];
};

void field(unsigned address, int *tag, int *index, int *block_offset, int n, int m);

int main(int argc, char *argv[])
{
    int c_size, b_size, asso, ra;
    int two_n, two_m, n, m, number_of_set, number_of_subset;
    unsigned address;
    int tag, index, block_offset;
    int set;
    int size0, size1, size2;
    _Bool hit_miss[1000000];
    FILE *ifp, *ofp;

    ifp=fopen(argv[2], "r");
    fscanf(ifp, "%d", &c_size);                         //cache size (KB)
    fscanf(ifp, "%d", &b_size);                         //block size (B)
    fscanf(ifp, "%d", &asso);                           //associativity
    fscanf(ifp, "%d", &ra);                             //replace algorithm

    
    two_m=b_size/4;                                     //block size=2^m words
    two_n=(c_size*1000)/b_size;                         //總block數, cache size=2^n blocks
    m=log2(two_m);

    if (asso == 0) {
        n=log2(two_n);
        number_of_set=two_n;
        number_of_subset=1;
        size0=number_of_set;
        size1=0;
        size2=0;
    }
    else if (asso == 1) {
        n=log2(two_n/4);
        number_of_set=two_n/4;
        number_of_subset=4;
        size0=0;
        size1=number_of_set;
        size2=0;
    }
    else if (asso == 2) {
        n=0;
        number_of_set=1;
        number_of_subset=two_n;
        size0=0;
        size1=0;
        size2=number_of_set;
    }

    printf("csize=%d, bsize=%d, asso=%d, ra=%d\n", c_size, b_size, asso, ra);
    printf("two_n=%d, two_m=%d\n", two_n, two_m);
    printf("n=%d, m=%d, number_of_set=%d, number_of_subset=%d\n", n, m, number_of_set, number_of_subset);    

    
    

    struct Cache_direct cache0[size0];                  //有size0個set的cache
    struct Cache_four cache1[size1];                    //有size1個set的cache
    struct Cache_fully cache2[size2];                   //有size2個set的cache

    if (asso == 0) {
        for (int i=0; i < number_of_set; i++) {         //全部初始化為0
            cache0[i].rear=0;
            for (int j=0; j < number_of_subset; j++) {
                cache0[i].valid[j]=0;
                cache0[i].tag[j]=0;
                cache0[i].LRU[j]=0;
            }
        }
    }
    else if (asso == 1) {
        for (int i=0; i < number_of_set; i++) {         //全部初始化為0
            cache1[i].rear=0;
            for (int j=0; j < number_of_subset; j++) {
                cache1[i].valid[j]=0;
                cache1[i].tag[j]=0;
                cache1[i].LRU[j]=0;
            }
        }
    }
    else if (asso == 2) {
        for (int i=0; i < number_of_set; i++) {         //全部初始化為0
            cache2[i].rear=0;
            for (int j=0; j < number_of_subset; j++) {
                cache2[i].valid[j]=0;
                cache2[i].tag[j]=0;
                cache2[i].LRU[j]=0;
            }
        }
    }
    




    int fuck=0;
    int count=0;                                            //第幾個address -1
    while (fscanf(ifp, "%x", &address) == 1) {
        
        //printf("\n#%d address=%x\n", count+1, address);
        if (asso == 0) {                                    //direct-mapped



            field(address, &tag, &index, &block_offset, n, m);
            set=index;                                      //set is determined by index
            //printf("tag=%d ", tag);
            //printf("index=%d ", index);
            //printf("block_offset=%d\n", block_offset);

            hit_miss[count]=0;
            int subset;
            for (subset=0; subset < number_of_subset; subset++) {
                
                if (cache0[set].valid[subset]==1 && cache0[set].tag[subset]==tag) {
                    hit_miss[count]=1;
                    if (ra == 1) {
                        cache0[set].LRU[subset]=count+1;    //記錄使用的時間
                    }
                    break;
                }
            }

            if (hit_miss[count] == 0) {                     //if miss
                if (ra == 0) {                              //replace algorithm: FIFO

                    int rear=cache0[set].rear;
                    cache0[set].valid[rear]=1;
                    cache0[set].tag[rear]=tag;

                    if (rear >= number_of_subset-1)
                        cache0[set].rear=0;
                    else
                        cache0[set].rear++;
                }
                else if (ra == 1) {                         //replace algorithm: LRU

                    int minLRU=cache0[set].LRU[0];
                    int select_subset=0;
                    for (subset=0; subset < number_of_subset; ++subset) {

                        if (cache0[set].valid[subset] == 0) {
                            select_subset=subset;
                            break;
                        } 
                        if (cache0[set].LRU[subset] < minLRU) {
                            minLRU=cache0[set].LRU[subset];
                            select_subset=subset;
                        }
                    }
                    cache0[set].valid[select_subset]=1;
                    cache0[set].tag[select_subset]=tag;
                    cache0[set].LRU[select_subset]=count+1; //記錄使用的時間
                }
            } //end if miss
        }
        else if (asso == 1) {                               //four-way set associative
            


            field(address, &tag, &index, &block_offset, n, m);
            set=index;                                      //set is determined by index
            //printf("tag=%d ", tag);
            //printf("index=%d ", index);
            //printf("block_offset=%d\n", block_offset);

            hit_miss[count]=0;
            int subset;
            for (subset=0; subset < number_of_subset; subset++) {
                
                if (cache1[set].valid[subset]==1 && cache1[set].tag[subset]==tag) {
                    hit_miss[count]=1;
                    if (ra == 1) {
                        cache1[set].LRU[subset]=count+1;    //記錄使用的時間
                    }
                    break;
                }
            }

            if (hit_miss[count]==0) {                       //if miss
                if (ra == 0) {                              //replace algorithm: FIFO
                    int rear=cache1[set].rear;
                    cache1[set].valid[rear]=1;
                    cache1[set].tag[rear]=tag;
                    if (rear>=3)
                        cache1[set].rear=0;
                    else
                        cache1[set].rear++;
                }
                else if (ra == 1) {                         //replace algorithm: LRU
                    int minLRU=cache1[set].LRU[0];
                    int select_subset=0;
                    for (subset=0; subset < number_of_subset; ++subset) {

                        if (cache1[set].valid[subset] == 0) {
                            select_subset=subset;
                            break;
                        } 
                        if (cache1[set].LRU[subset] < minLRU) {
                            minLRU=cache1[set].LRU[subset];
                            select_subset=subset;
                        }
                    }
                    cache1[set].valid[select_subset]=1;
                    cache1[set].tag[select_subset]=tag;
                    cache1[set].LRU[select_subset]=count+1;  //記錄使用的時間
                }
            }

        }
        else if (asso == 2) {                               //fully associative



            field(address, &tag, &index, &block_offset, n, m);
            set=0;                                          //fully associative has only 1 set
            //printf("tag=%d ", tag);
            //printf("index=%d ", index);
            //printf("block_offset=%d\n", block_offset);

            hit_miss[count]=0;
            int subset;
            for (subset=0; subset < number_of_subset; subset++) {
                
                if (cache2[set].valid[subset]==1 && cache2[set].tag[subset]==tag) {
                    hit_miss[count]=1;
                    if (ra == 1) {
                        cache2[set].LRU[subset]=count+1;    //記錄使用的時間
                    }
                    break;
                }
            }

            if (hit_miss[count] == 0) {                     //if miss
                if (ra == 0) {                              //replace algorithm: FIFO

                    int rear=cache2[set].rear;
                    cache2[set].valid[rear]=1;
                    cache2[set].tag[rear]=tag;
                    if (rear >= number_of_subset-1)
                        cache2[set].rear=0;
                    else
                        cache2[set].rear++;
                }
                else if (ra == 1) {                         //replace algorithm: LRU

                    int minLRU=cache2[set].LRU[0];
                    int select_subset=0;
                    for (subset=0; subset < number_of_subset; ++subset) {

                        if (cache2[set].valid[subset] == 0) {
                            select_subset=subset;
                            break;
                        } 
                        if (cache2[set].LRU[subset] < minLRU) {
                            minLRU=cache2[set].LRU[subset];
                            select_subset=subset;
                        }
                    }
                    //if (subset!=number_of_subset) printf("#%d use %d space\n", count+1, ++fuck);   
                    //if (cache2[set].tag[select_subset]==99950653) 
                    //    printf("#%d replace %d\n", count+1, cache[set].LRU[select_subset]);
                    //if (count == 599-1)
                    //    for (subset=0; subset < number_of_subset; ++subset){
                    //        printf("%d ", cache2[set].LRU[subset]);
                    //    }
                    cache2[set].valid[select_subset]=1;
                    cache2[set].tag[select_subset]=tag;
                    cache2[set].LRU[select_subset]=count+1; //記錄使用的時間
                }
            } //end if miss
            
        } //end else
        count++;
    } //end while

    int COUNT=count;
    int beginning;
    int miss_count=0;
    printf("COUNT=%d\n", COUNT);

    ofp=fopen(argv[4], "w");

    beginning=0;
    fprintf(ofp, "Hits instructions: ");
    for (count=0; count<COUNT; ++count) {
        if (hit_miss[count]==1) {
            if (beginning==0) {
                fprintf(ofp, "%d", count+1);
                beginning++;
            }
            else
                fprintf(ofp, ",%d", count+1);
        }
    }
    fprintf(ofp, "\n");
    beginning=0;
    fprintf(ofp, "Misses instructions: ");
    for (count=0; count<COUNT; ++count) {
        if (hit_miss[count]==0) {
            miss_count++;
            if (beginning==0) {
                fprintf(ofp, "%d", count+1);
                beginning++;
            }
            else
                fprintf(ofp, ",%d", count+1);
        }
    }
    fprintf(ofp, "\n");
    fprintf(ofp, "Miss rate: %f\n", (double)miss_count/(double)COUNT);
    

    fclose(ifp);
    fclose(ofp);
    return 0;
}


void field(unsigned address, int *tag, int *index, int *block_offset, int n, int m)
{
    int i;
    int tag_length=32-n-m-2;
    unsigned twoexp=1;

    *block_offset=0;
    for (i=1; i<=m; i++) {
        if ((address >> (i+1)) & 0x00000001) {
            *block_offset+=twoexp;
        }
        twoexp*=2;
    }

    *index=0;
    twoexp=1;
    for (i=1; i<=n; i++) {
        if ((address >> (i+m+1)) & 0x00000001)
            *index+=twoexp;
        twoexp*=2;
    }

    *tag=0;
    twoexp=1;
    for (i=1; i<=tag_length; i++) {
        if ((address >> (i+n+m+1)) & 0x00000001)
            *tag+=twoexp;
        twoexp*=2;
    }
}


