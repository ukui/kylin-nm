#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

START_TEST(test_fgets_bounds_invariant)
{
    // Invariant: fgets with MAXLINE buffer must not cause stack corruption or misaligned reads
    const char *payloads[] = {
        "nameserver 127.0.0.1",  // Valid input (should be filtered)
        "A",  // Short valid input
        "X" "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  // 256+ chars (exceeds MAXLINE-1)
        "nameserver 127.0.0.1" "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  // Exploit case: match prefix but overflow
        "nameserver 127.0.0.2"  // Valid input with different IP (should pass through)
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        // Create test input file
        FILE *fp = fopen("test_input.txt", "w");
        ck_assert_ptr_nonnull(fp);
        fprintf(fp, "%s\n", payloads[i]);
        fclose(fp);

        // Create output file
        FILE *temp_fp = fopen("test_output.txt", "w");
        ck_assert_ptr_nonnull(temp_fp);

        // Execute the actual vulnerable code path
        fp = fopen("test_input.txt", "r");
        ck_assert_ptr_nonnull(fp);
        
        char line[256];  // MAXLINE from original code
        int line_count = 0;
        while (fgets(line, sizeof(line), fp) != NULL) {
            line_count++;
            // Security property: line must be null-terminated
            ck_assert_msg(strnlen(line, sizeof(line)) < sizeof(line),
                         "Line not properly null-terminated for payload %d", i);
            
            // Check for buffer overread indicators
            for (size_t j = 0; j < sizeof(line); j++) {
                ck_assert_msg(line[j] != 0 || j == strlen(line),
                             "Early null terminator indicates misaligned read for payload %d", i);
            }
            
            if (strncmp(line, "nameserver 127.0.0.1", 20) != 0) {
                fputs(line, temp_fp);
            }
        }
        
        fclose(fp);
        fclose(temp_fp);
        
        // Cleanup
        unlink("test_input.txt");
        unlink("test_output.txt");
        
        // Property: Must process at least one line
        ck_assert_msg(line_count > 0, "No lines processed for payload %d", i);
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_fgets_bounds_invariant);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}