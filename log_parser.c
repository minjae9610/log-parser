#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_log
{
    char log[1000];
    struct s_log *next;
} t_log;

typedef struct s_log_list
{
    t_log *head;
    int size;
} t_log_list;

t_log *lstlast(t_log *lst);
void lstadd_back(t_log **lst, t_log *new);
void lstclear(t_log **lst, void (*del)(void *));
void lstdelone(t_log *lst, void (*del)(void *));
void del_t_log(void *log);
void swap(int *a, int *b);

char month_list[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
                          "Oct", "Nov", "Dec"};

t_log_list logs[33872500];

int main()
{
    FILE *fp = fopen("log.txt", "r");
    int top[3] = {-1, -1, -1};

    char buf[1000];
    while (fgets(buf, 1000, fp))
    {
        int month = 0, day, hour, minute, second, timestamp;
        char month_str[4];
        sscanf(buf, "%s %d %d:%d:%d", month_str, &day, &hour, &minute, &second);
        for (int i = 0; i < 12; ++i)
        {
            if (strcmp(month_str, month_list[i]) == 0)
            {
                month = i + 1;
                break;
            }
        }
        timestamp = second + minute * 60 + hour * 3600 + day * 86400 + month * 2592000;
        t_log *new = (t_log *)malloc(sizeof(t_log));
        strcpy(new->log, buf);
        lstadd_back(&(logs[timestamp].head), new);
        logs[timestamp].size += 1;

        if ((top[0] != timestamp && top[1] != timestamp && top[2] != timestamp) &&
            (top[2] == -1 || logs[top[2]].size < logs[timestamp].size))
            top[2] = timestamp;
        if (top[2] != -1 && (top[1] == -1 || logs[top[2]].size > logs[top[1]].size))
            swap(&top[2], &top[1]);
        if (top[1] != -1 && (top[0] == -1 || logs[top[1]].size > logs[top[0]].size))
            swap(&top[1], &top[0]);
    }
    fclose(fp);

    for (int i = 0; i < 3; ++i)
    {
        if (top[i] == -1)
            break;
        printf("Top %d: %d %d\n", i + 1, top[i], logs[top[i]].size);
        t_log *temp = logs[top[i]].head;
        while (temp)
        {
            printf("%s", temp->log);
            temp = temp->next;
        }
    }

    for (int i = 0; i < 33872500; ++i)
        if (logs[i].size > 0)
            lstclear(&logs[i].head, del_t_log);

    return 0;
}

t_log *lstlast(t_log *lst)
{
    if (lst)
        while (lst->next)
            lst = lst->next;
    return (lst);
}

void lstadd_back(t_log **lst, t_log *new)
{
    if (!lst || !new)
        return;
    if (*lst)
        lstlast(*lst)->next = new;
    else
        *lst = new;
}

void lstclear(t_log **lst, void (*del)(void *))
{
    t_log *temp;

    while (*lst)
    {
        temp = (*lst)->next;
        lstdelone(*lst, del);
        *lst = temp;
    }
}

void lstdelone(t_log *lst, void (*del)(void *))
{
    if (!lst || !del)
        return;
    free(lst);
}

void del_t_log(void *log)
{
    free(log);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
