#ifndef TECHPARK_LOGIC_PRO_H
#define TECHPARK_LOGIC_PRO_H

#define M_Type 1

struct msgbuf {
    long mType;
    unsigned long long sum;
};
struct partition {
    long mType;
    size_t first;
    size_t last;
};

unsigned long long int sum_pro(const int *array, int size, int num_of_forks);

void MakePartition(size_t first, size_t second, int partition_queue_qid);


#endif //TECHPARK_LOGIC_PRO_H
