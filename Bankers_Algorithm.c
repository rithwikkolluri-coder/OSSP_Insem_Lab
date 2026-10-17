
#include <stdio.h>

int n, m, safe = 0, unsafe = 0;
int alloc[10][10], max[10][10], need[10][10];
int avail[10], seq[10], used[10];

void findSeq(int depth)
{
    if (depth == n)
    {
        int work[10], ok = 1, blocked = -1;

        for (int j = 0; j < m; j++)
            work[j] = avail[j];

        for (int i = 0; i < n; i++)
        {
            int p = seq[i], j;

            for (j = 0; j < m; j++)
            {
                if (need[p][j] > work[j])
                    break;
            }

            if (j < m)
            {
                ok = 0;
                blocked = p;
                break;
            }

            for (j = 0; j < m; j++)
                work[j] += alloc[p][j];
        }

        if (ok)
        {
            safe++;
            printf("Safe Sequence %d: ", safe);
        }
        else
        {
            unsafe++;
            printf("Unsafe Sequence %d: ", unsafe);
        }

        for (int i = 0; i < n; i++)
            printf("P%d ", seq[i]);

        if (!ok)
            printf("-> P%d cannot proceed", blocked);

        printf("\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (!used[i])
        {
            used[i] = 1;
            seq[depth] = i;
            findSeq(depth + 1);
            used[i] = 0;
        }
    }
}

int main()
{
    printf("Enter number of processes and resources: ");
    scanf("%d%d", &n, &m);

    if (n < 1 || n > 10 || m < 1 || m > 10)
    {
        printf("Invalid input\n");
        return 0;
    }

    printf("Enter Allocation matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    }

    printf("Enter Maximum matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    }

    printf("Enter Available vector:\n");
    for (int j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    for (int i = 0; i < n; i++)
    {
        used[i] = 0;

        for (int j = 0; j < m; j++)
        {
            if (alloc[i][j] < 0 || max[i][j] < alloc[i][j] || avail[j] < 0)
            {
                printf("Invalid matrix or available vector\n");
                return 0;
            }

            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    printf("\nNeed matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);

        printf("\n");
    }

    printf("\nAll process sequences:\n");
    findSeq(0);

    if (safe > 0)
        printf("\nSystem is in SAFE state\n");
    else
        printf("\nSystem is in UNSAFE state\n");

    printf("Total safe sequences: %d\n", safe);
    printf("Total unsafe sequences: %d\n", unsafe);

    return 0;
}

