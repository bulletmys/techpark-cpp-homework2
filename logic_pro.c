#include <unistd.h>
#include <wait.h>
#include "logic_pro.h"
#include "stdio.h"
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

void MakePartition(size_t first, size_t second, int partition_queue_qid) {
    struct partition part;
    part.mType = M_Type;
    part.first = first;
    part.last = second;
    msgsnd(partition_queue_qid, &part, sizeof(struct partition) - sizeof(long), 0);
}


unsigned long long int sum_pro(const int *array, int size, int num_of_forks) {
    int msg_qid, status, partition_queue_qid;

    if ((msg_qid = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        printf("QUEUE Creation Error");
    }

    if ((partition_queue_qid = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        printf("QUEUE Creation Error");
    }

    int length_msg = sizeof(struct msgbuf) - sizeof(long);
    int length_part = sizeof(struct partition) - sizeof(long);


    int step = size / num_of_forks;
    int step_last = size - step * (num_of_forks - 1);

    //Создание очереди для распределения участков массива по форкам
    for (int i = 0; i < size - step_last; i += step) {
        MakePartition(i, i + step - 1, partition_queue_qid);
    }
    MakePartition(size - step_last, size - 1, partition_queue_qid);


    pid_t *pid_array = (pid_t *) malloc(sizeof(pid_t) * num_of_forks);

    pid_t main_pid = getpid();

    for (int k = 0; k < num_of_forks - 1; ++k) {
        if (main_pid == getpid()) {
            pid_array[k] = fork();
        }
    }

    struct partition partition;

    msgrcv(partition_queue_qid, &partition, length_part, M_Type, 0);

    unsigned long long sum = 0;

    for (size_t j = partition.first; j <= partition.last; ++j) {
        sum += array[j] % 1024;
    }

    //Отправляем результат суммы у дочерних форков в очередь и завершаем процесс
    if (main_pid != getpid()) {
        struct msgbuf buf = {1, 0};
        buf.sum = sum;
        msgsnd(msg_qid, &buf, length_msg, 0);
        exit(0);
    }

    for (int l = 0; l < num_of_forks - 1; ++l) {
        waitpid(pid_array[l], &status, 0);
    }

    for (int m = 0; m < num_of_forks - 1; ++m) {
        struct msgbuf receive;
        msgrcv(msg_qid, &receive, length_msg, M_Type, 0);

        sum += receive.sum;
    }

    free(pid_array);
    return sum;

}
