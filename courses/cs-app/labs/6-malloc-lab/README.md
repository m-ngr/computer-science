# Malloc Lab - Summary of the Results

| Package                                | Description                                  | Memory Utilization | Throughput | Performance Index |
| -------------------------------------- | -------------------------------------------- | ------------------ | ---------- | ----------------- |
| [mm-naive.c](./mm-naive.c)             | The given handout                            | -                  | -          | -                 |
| [mm-text.c](./mm-text.c)               | Textbook CS:APPe3 implicit free list example | 44/60              | 11/40      | 55%               |
| [mm-implicit-v1.c](./mm-implicit-v1.c) | Implicit free list based malloc package v1   | 46/60              | 11/40      | 57%               |
| [mm-implicit-v2.c](./mm-implicit-v2.c) | Implicit free list based malloc package v2   | 46/60              | 11/40      | 57%               |
| [mm-explicit.c](./mm-explicit.c)       | Explicit free list based malloc package      | 43/60              | 40/40      | 83%               |
| [mm-seglist-v1.c](./mm-seglist-v1.c)   | Segregated list based malloc package v1      | 48/60              | 40/40      | 88%               |
| [mm-seglist-v2.c](./mm-seglist-v2.c)   | Segregated list based malloc package v2      | 55/60              | 40/40      | 95%               |
| [mm-seglist-v3.c](./mm-seglist-v3.c)   | Segregated list based malloc package v3      | 57/60              | 40/40      | 97%               |
| [mm-seglist-v4.c](./mm-seglist-v4.c)   | Segregated list based malloc package v4      | 58/60              | 40/40      | 98%               |
