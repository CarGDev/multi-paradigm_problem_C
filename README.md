# multi-paradigm_problem_C
program that calculates basic statistics (mean, median, and mode) on a list of integers. 

## Project overview

A program that calculates the **mean**, **median** and **mode** of a list of integers. The same problem is implemented in three languages, each with a different paradigm:

| Language | Paradigm | Repository |
|----------|----------|------------|
| C | Procedural | `multi-paradigm_problem_C` |
| OCaml | Functional | `multi-paradigm_problem_OCaml` |
| Python | Object-oriented (`StatisticsCalculator` class) | `multi-paradigm_problem_Python` |

> **Status:** the source code has not been written yet. This README is the design reference for what must be implemented. Everything below is *planned*; nothing is claimed as implemented.

## Calculations

| Statistic | Definition | Edge cases |
|-----------|------------|------------|
| Mean | The sum of all values divided by the number of values. | **Empty list:** undefined (division by zero); report an error instead of a number. |
| Median | The middle value of the sorted list. | **Empty list:** undefined; report an error. **Even length:** no single middle value, so use the average of the two middle values (e.g. `[1, 2, 3, 4]` gives `2.5`). |
| Mode | The value that occurs most often. | **Empty list:** undefined; report an error. **Multiple modes:** several values share the highest count (e.g. `[1, 1, 2, 2, 3]`); return all of them. **All values unique:** every value occurs once, so there is no meaningful mode; report "no mode". |

## Planned additions (checklist)

Optional statistics, built on the same functions where possible:

- [ ] Minimum, maximum and range (max - min)
- [ ] Sum and count
- [ ] Variance and standard deviation (reuse the mean function)
- [ ] Quartiles or percentiles (extend the median idea)

Robustness cases to handle and test:

- [ ] Empty list
- [ ] Even-length list (median)
- [ ] Multiple modes, and the case where every value occurs once
- [ ] Negative numbers and duplicates
- [ ] Invalid input, such as non-integers typed by the user

## How to build and run

> The commands below are the standard ones for each toolchain. They have **not been verified** because the code does not exist yet; re-check them once it does.

### C (procedural)

Source files: `src/main.c`, `src/main.h`.

```sh
gcc -Wall -Wextra -o main src/main.c
./main
```

(`*.out`, `*.o` and other build artifacts are already ignored by `.gitignore`.)

Example (expected output once implemented):

```text
Input:  1 2 2 3 4
Mean:   2.4
Median: 2
Mode:   2

Input:  1 2 3 4
Mean:   2.5
Median: 2.5
Mode:   no mode (all values unique)

Input:  (empty)
Error:  the list is empty
```

## References (APA 7)

- Bruce, P., & Bruce, A. (2017). *Practical statistics for data scientists: 50 essential concepts*. O'Reilly Media. — **Statistics:** variance, standard deviation, percentiles.
- Conchon, S., & Filliâtre, J.-C. (2026). *Learn programming with OCaml: Algorithms and data structures* (U. Nair, Trans.). OCaml Software Foundation. https://creativecommons.org/licenses/by-sa/4.0/ — **OCaml:** functional implementation. *(Publisher inferred from the funding note on the title page; confirm.)*
- Illowsky, B., & Dean, S. (2018). *Introductory statistics*. OpenStax, Rice University. https://openstax.org/details/books/introductory-statistics — **Statistics:** definitions of mean, median and mode.
- Kernighan, B. W., & Ritchie, D. M. (1988). *The C programming language* (2nd ed.). Prentice Hall. — **C:** procedural implementation.
- Ramalho, L. (2022). *Fluent Python* (2nd ed.). O'Reilly Media. — **Python:** object-oriented implementation.
