#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int btc_mcu_main(int argc, char **argv);
int eth_mcu_main(int argc, char **argv);

static void usage(const char *program)
{
    fprintf(stderr,
            "Dual BTC/ETH MCU reference\n\n"
            "Usage:\n"
            "  %s btc <BTC coldsign command...>\n"
            "  %s eth <address|sign-hash-fpga|sign-fpga> ...\n\n"
            "The MCU process never receives the FPGA private key.\n",
            program, program);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "btc") == 0)
        return btc_mcu_main(argc - 1, argv + 1);
    if (strcmp(argv[1], "eth") == 0)
        return eth_mcu_main(argc - 1, argv + 1);
    usage(argv[0]);
    return EXIT_FAILURE;
}
