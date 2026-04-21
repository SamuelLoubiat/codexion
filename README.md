*This project has been created as part of the 42 curriculum by sloubiat.*

## Description
**Codexion** is a concurrency and resource management simulation developed in C. It models a shared co-working space where "coders" (represented by threads) compete for a limited number of dongles (mutex-protected resources) to compile code. 

The project focuses on solving classic synchronization problems such as deadlocks and starvation, while implementing real-time scheduling constraints like **Earliest Deadline First (EDF)** or **First In First Out (FIFO)**.

---

## Instructions

### Compilation
The project uses a Makefile and must be compiled with the following flags:
```bash
make
```
This generates the `codexion` executable.

### Execution
Run the program with the following parameters:
```bash
./codexion <nb_coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <nb_compiles> <cooldown> <scheduler>
```
* **scheduler**: `fifo` (First In, First Out) or `edf` (Earliest Deadline First).

---

## Blocking cases handled
To ensure the simulation runs correctly without crashing or stalling, several concurrency challenges were addressed:

* **Deadlock Prevention:** Implemented by imposing a strict order on dongle acquisition (Resource Hierarchy). A coder always picks up the lower-indexed dongle first, breaking the circular wait condition.
* **Starvation (Famine):** Prevented by the **EDF scheduler**, which ensures that coders closest to their burnout deadline are granted priority access to the dongles.
* **Race Conditions:** All shared variables (simulation status, dongle availability, and compilation counts) are wrapped in `pthread_mutex_t` to ensure atomic access.
* **Cooldown Management:** Dongles are not immediately reusable. The system tracks the release timestamp and prevents acquisition until the `dongle_cooldown` has passed.
* **Log Serialization:** A dedicated "display mutex" prevents log messages from multiple threads from overlapping or interleaving.

---

## Thread synchronization mechanisms
* **pthread_mutex_t:** Used to represent each individual dongle and to protect the "monitor" state.
* **Monitor Thread:** A dedicated thread runs independently to check the burnout status of all coders. It ensures the simulation stops and the burnout message is printed within the strict **10ms** requirement.
* **Priority Queue (Min-Heap):** A custom built heap structure manages the scheduling of requests for both `fifo` and `edf` policies.

---

## Resources
* *POSIX Threads (pthreads) Documentation*
* **AI Usage:** AI was used as a learning and support tool for this project. Specifically:
    * **Conceptual understanding:** To clarify the lifecycle of POSIX threads and the behavior of condition variables.
    * **Debugging:** To identify edge cases in the synchronization logic and timing issues.
    * **Documentation:** To assist in structuring and drafting this README.md file.