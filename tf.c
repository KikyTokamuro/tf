/*
MIT License

Copyright (c) 2020-2021 Daniil A. (Kiky Tokamuro)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <libconfig.h>
#include <unistd.h>
#include <argp.h>

// Process status
int run = 1;

// Config vars
const char *temperature_f;
const char *fan_f;
int  sleep_t;

// Struct level items
typedef struct level_e {
    int min;
    int max;
} level_e;

// Temperature levels
level_e *levels = NULL;

// Info
const char *argp_program_version = "tf v1.0";
const char *argp_program_bug_address = "<kiky.tokamuro@yandex.ru>";
const char doc[] = "tf - tiny program for fan control Thinkpad notebooks";

// Arguments
static struct argp_option options[] = {
    {"config", 'c', "CONFIG", 0, "Path to config file (default: /etc/tf.cfg)"},
    {0}
};

struct arguments {
    char *config;
};

// Parsing arguments
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    
    switch (key) {
    case 'c': arguments->config = arg; break;
    default: return ARGP_ERR_UNKNOWN;
    }
    
    return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

// Write to fan file
void fan_write(int level) {
    FILE *fan = fopen(fan_f, "w");
    if (fan == NULL) {
        fprintf(stderr, "Failed to open %s.\n", fan_f);
	exit(1);
    }

    if (level == -1) {
        fprintf(fan, "level auto\n");
        fclose(fan);
        return;
    }
    
    // Message for fan_write
    char message[12] = {'\0'};
    snprintf(message, sizeof(message), "level %d", level);

    fprintf(fan, "%s\n", message);
    fclose(fan);
}

// Get CPU temp
int get_temp() {
    FILE *temp = fopen(temperature_f, "r");
    if (temp == NULL) {
        fprintf(stderr, "Failed to open %s.\n", temperature_f);
	exit(1);
    }

    int temp_c; 
    fscanf(temp, "%d", &temp_c);
    temp_c /= 1000;

    fclose(temp);

    return temp_c;
}

// Signal handler
void kill_handler(int sig) {
    fprintf(stdout, "tf stopped\n");
    run = 0;
}

void signals_init() {
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
        fprintf(stderr, "Failed setup handler for SIGINT signal.\n");
	exit(1);
    }

    // SIGTERM
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        fprintf(stderr, "Failed setup handler for SIGTERM signal.\n");
	exit(1);
    }
}

int main(int argc, char **argv) {
    // Init signals
    signals_init();
    
    // Arguments
    struct arguments arguments;
    arguments.config = "/etc/tf.cfg";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Init config
    config_t cfg;
    config_setting_t *level_settings;
    config_init(&cfg);

    // Read config file
    if (!config_read_file(&cfg, arguments.config)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return 1;
    }

    // Read temperature param from config
    if (!config_lookup_string(&cfg, "temperature", &temperature_f)) {
        fprintf(stderr, "Failed read temperature param from config file.\n");
        config_destroy(&cfg);
        return 1;
    }

    // Read fan param from config
    if (!config_lookup_string(&cfg, "fan", &fan_f)) {
        fprintf(stderr, "Failed read fan param from config file.\n");
        config_destroy(&cfg);
        return 1;
    }
        
    // Read sleep param from config
    if (!config_lookup_int(&cfg, "sleep", &sleep_t)) {
        fprintf(stderr, "Failed read sleep param from config file.\n");
        config_destroy(&cfg);
        return 1;
    }
    
    // Read levels params from config
    level_settings = config_lookup(&cfg, "levels");
    if (level_settings != NULL) {
        int count = config_setting_length(level_settings);
        
        if ((count < 8) || (count > 8)) {
            fprintf(stderr, "The count of levels must be 8 (0-7).\n");
            config_destroy(&cfg);
            return 1;
        }
        
        levels = malloc(count * sizeof(level_e));
        
        for (int i = 0; i < count; i++) {
            config_setting_t *level = config_setting_get_elem(level_settings, i);
            
            int min, max;
            
            if (!(config_setting_lookup_int(level, "min", &min) &&
                  config_setting_lookup_int(level, "max", &max))) {
                fprintf(stderr, "Failed read level param from level%d", i);
                return 1;
            }
            
            levels[i].min = min;
            levels[i].max = max;
        }
    } else {
        fprintf(stderr, "Failed read levels params from config file.\n");
        config_destroy(&cfg);
        return 1;
    }
    
    // Current fan level
    unsigned short current_level = 0;
    
    // Install level 0
    fan_write(current_level);
    
    while (run) {
        //Current cpu temp
        int cpu = get_temp();
        
        // Get min and max for current level
        int current_min = levels[current_level].min;
        int current_max = levels[current_level].max;
        
        // CPU temp control
        if (current_level < 7 && cpu >= current_max) {
            current_level++;
            fan_write(current_level);
        } else if (current_level > 0 && cpu <= current_min) {
            current_level--;
            fan_write(current_level);
        }
        
        sleep(sleep_t);
    }
    
    fan_write(-1);
    config_destroy(&cfg);
    free(levels);
    return 0;
}
