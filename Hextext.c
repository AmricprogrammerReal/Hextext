#include <stdio.h>  // For file operations (fopen, fclose, fgetc, printf, fprintf, perror)
#include <stdlib.h> // For exit

int main(int argc, char *argv[]) {
    FILE *input_file;
    FILE *output_file = NULL; // Initialize to NULL
    int byte_read;
    int byte_count = 0;
    int write_to_file = 0; // Flag to determine if output should go to a file

    // 1. Check for correct command-line arguments
    // Expected usage:
    // ProgramName <input_filename>
    // OR
    // ProgramName <input_filename> <output_filename>
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input_filename> [output_filename]\n", argv[0]);
        fprintf(stderr, "Example 1: %s my_document.txt         (prints to console)\n", argv[0]);
        fprintf(stderr, "Example 2: %s my_document.txt hex_output.txt (writes to file)\n", argv[0]);
        return 1; // Indicate an error
    }

    // Determine if an output file was specified
    if (argc == 3) {
        write_to_file = 1; // Set flag to write to file
    }

    // 2. Open the input file in binary read mode ("rb")
    input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // 3. If an output file was specified, open it in write mode ("w")
    // "w" is fine for text output, as we're writing formatted hex strings.
    if (write_to_file) {
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            perror("Error opening output file");
            fclose(input_file); // Close input file before exiting
            return 1;
        }
    }

    // Print header to console or file
    if (write_to_file) {
        fprintf(output_file, "Hexadecimal dump of file: '%s'\n", argv[1]);
        fprintf(output_file, "------------------------------------------------------------------\n");
    } else {
        printf("Hexadecimal dump of file: '%s'\n", argv[1]);
        printf("------------------------------------------------------------------\n");
    }

    // 4. Read the file byte by byte and print/write hex values
    while ((byte_read = fgetc(input_file)) != EOF) {
        // Print to the chosen destination (file or console)
        if (write_to_file) {
            fprintf(output_file, "%02X ", byte_read);
        } else {
            printf("%02X ", byte_read);
        }

        byte_count++;

        // Add a newline character after every 16 bytes for readability
        if (byte_count % 16 == 0) {
            if (write_to_file) {
                fprintf(output_file, "\n");
            } else {
                printf("\n");
            }
        }
    }

    // If the last line didn't end with a full 16 bytes, print a final newline
    if (byte_count % 16 != 0) {
        if (write_to_file) {
            fprintf(output_file, "\n");
        } else {
            printf("\n");
        }
    }

    // Print footer
    if (write_to_file) {
        fprintf(output_file, "------------------------------------------------------------------\n");
        fprintf(output_file, "Successfully processed %d bytes.\n", byte_count);
    } else {
        printf("------------------------------------------------------------------\n");
        printf("Successfully processed %d bytes.\n", byte_count);
    }

    // 5. Close both files
    fclose(input_file);
    if (write_to_file) {
        fclose(output_file);
        printf("Hexadecimal output written to '%s'\n", argv[2]);
    }

    return 0; // Indicate success
}
