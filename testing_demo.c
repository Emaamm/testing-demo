/*
 * Simple Software Testing Demo (C)
 *
 * Parts:
 *   1. Function under test   -> get_grade()
 *   2. Branch tracking       -> BRANCH(n) marks which branch ran
 *   3. Test cases + runner   -> run_all_tests()
 *   4. Coverage report       -> print_coverage()
 *   5. Try-your-own-input    -> try_input()
 */
#include <stdio.h>
#include <string.h>

/* ---------- Part 2: branch tracking ---------- */
#define NUM_BRANCHES 6
static int hit[NUM_BRANCHES + 1];          /* hit[n] = how many times branch n ran */
#define BRANCH(n) (hit[n]++)

static const char *branch_desc[NUM_BRANCHES + 1] = {
    "",
    "score < 0 or > 100  -> 'X' (invalid)",
    "score >= 90         -> 'A'",
    "score >= 80         -> 'B'",
    "score >= 70         -> 'C'",
    "score >= 50         -> 'D'",
    "score < 50          -> 'F'"
};

/* ---------- Part 1: function under test ---------- */
char get_grade(int score) {
    if (score < 0 || score > 100) { BRANCH(1); return 'X'; }
    if (score >= 90)              { BRANCH(2); return 'A'; }
    if (score >= 80)              { BRANCH(3); return 'B'; }
    if (score >= 70)              { BRANCH(4); return 'C'; }
    if (score >= 50)              { BRANCH(5); return 'D'; }
    BRANCH(6);
    return 'F';
}

/* ---------- Part 3: test cases + runner ---------- */
typedef struct {
    const char *name;
    int input;
    char expected;
} TestCase;

static TestCase tests[] = {
    {"negative score",        -5, 'X'},
    {"above 100",            101, 'X'},
    {"top grade",             95, 'A'},
    {"boundary 90",           90, 'A'},
    {"boundary 89",           89, 'B'},
    {"middle B",              85, 'B'},
    {"boundary 70",           70, 'C'},
    {"boundary 50",           50, 'D'},
    {"boundary 49",           49, 'F'},
    {"zero",                   0, 'F'}
};

void run_all_tests(void) {
    int n = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    printf("\n--- Running %d tests ---\n", n);
    for (int i = 0; i < n; i++) {
        char actual = get_grade(tests[i].input);
        int ok = (actual == tests[i].expected);
        if (ok) passed++;
        printf("[%s] %-14s input=%3d expected=%c actual=%c\n",
               ok ? "PASS" : "FAIL", tests[i].name,
               tests[i].input, tests[i].expected, actual);
    }
    printf("Result: %d/%d passed\n", passed, n);
}

/* ---------- Part 4: coverage report ---------- */
void print_coverage(void) {
    int covered = 0;
    printf("\n--- Branch coverage ---\n");
    for (int b = 1; b <= NUM_BRANCHES; b++) {
        int ran = hit[b] > 0;
        if (ran) covered++;
        printf("Branch %d: %-8s (%d hits)  %s\n", b,
               ran ? "COVERED" : "MISSED", hit[b], branch_desc[b]);
    }
    printf("Coverage: %d/%d = %.0f%%\n", covered, NUM_BRANCHES,
           100.0 * covered / NUM_BRANCHES);
}

/* ---------- Part 5: try your own input ---------- */
void try_input(void) {
    int score;
    printf("Enter a score: ");
    if (scanf("%d", &score) != 1) {
        while (getchar() != '\n');         /* clear bad input */
        printf("Not a number.\n");
        return;
    }
    memset(hit, 0, sizeof(hit));           /* reset so we see only this run */
    char g = get_grade(score);
    printf("Grade: %c\n", g);
    for (int b = 1; b <= NUM_BRANCHES; b++)
        if (hit[b]) printf("Branch that ran: %d (%s)\n", b, branch_desc[b]);
}

int main(void) {
    int choice;
    do {
        printf("\n=== Testing Demo ===\n");
        printf("1. Run all tests\n2. Show coverage\n3. Try my own input\n0. Exit\n> ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); choice = -1; }
        switch (choice) {
            case 1: run_all_tests(); break;
            case 2: print_coverage(); break;
            case 3: try_input(); break;
            case 0: printf("Bye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}
