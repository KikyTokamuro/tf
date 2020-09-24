#include <signal.h>
#include <string.h>

#include "tf.h"

// Process status
int run = 1;

// Signal handler
void kill_handler(int sig) {
    fprintf(stdout, "tf stopped\n");
    run = 0;
}

int main(int argc, char const *argv[]) {
    // Signal set setup
    sigset_t set;
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGINT); 
    sigaddset(&set, SIGTERM);

    // Sigaction setup
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = kill_handler;
    act.sa_mask = set;

    // SIGINT
    if (sigaction(SIGINT, &act, NULL) < 0) {
        fprintf(stderr, "Failed setup handler for SIGINT signal\n");
    }

    // SIGTERM
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        fprintf(stderr, "Failed setup handler for SIGTERM signal\n");
    }

    // Current fan level
    unsigned short current_level = LEVEL_1;

    // Message for fan_write
    char message[12] = {'\0'};

    while (run) {
        // Current cpu temp
        int cpu = get_temp();

        int current_min = config[current_level].min;
        int current_max = config[current_level].max;

        // CPU temp control
        if (current_level < 7 && cpu >= current_max) {
            current_level++;
        } else if (current_level > 0 && cpu <= current_min) {
            current_level--;
        }

        // Write Fan level
        snprintf(message, sizeof(message), "level %hu", current_level);
        fan_write(message);

        sleep(SLEEP_TIME);
    }

    fan_write("level auto");

    return 0;
}
