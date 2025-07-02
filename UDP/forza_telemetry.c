#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#define address struct sockaddr_in
#define main_loop while

#include <math.h>
#include <stdio.h>





const char* rpm_color(float rpm) {
    if (rpm < 4000.0f) {
        return "\033[32m";    // verde
    } else if (rpm < 7100.0f) {
        return "\033[33m";    // giallo
    } else {
        return "\033[31m";    // rosso
    }
}

struct SledData {
    int isRaceOn;             // 0 = menu, 1 = race
    unsigned int timestampMS; // Game session time in ms
    float engineMaxRpm;
    float engineIdleRpm;
    float currentEngineRpm;
    float accelerationX;
    float accelerationY;
    float accelerationZ;
    float velocityX;
    float velocityY;
    float velocityZ;
    float angularVelocityX;
    float angularVelocityY;
    float angularVelocityZ;
    float yaw;
    float pitch;
    float roll;
    float normSuspensionTravelFL;
    float normSuspensionTravelFR;
    float normSuspensionTravelRL;
    float normSuspensionTravelRR;
    float tireSlipRatioFL;
    float tireSlipRatioFR;
    float tireSlipRatioRL;
    float tireSlipRatioRR;
    float tireSlipAngleFL;
    float tireSlipAngleFR;
    float tireSlipAngleRL;
    float tireSlipAngleRR;
    float tireCombinedSlipFL;
    float tireCombinedSlipFR;
    float tireCombinedSlipRL;
    float tireCombinedSlipRR;
    float suspensionTravelMetersFL;
    float suspensionTravelMetersFR;
    float suspensionTravelMetersRL;
    float suspensionTravelMetersRR;
    float wheelSpeedFL;
    float wheelSpeedFR;
    float wheelSpeedRL;
    float wheelSpeedRR;
    float throttle;
    float steer;
    float brake;
    float clutch;
    float gear;
    float gForceLat;
    float gForceLong;
    float lap;
    float racePosition;
    float accel;
};

float calculate_speed_kmh(struct SledData* sledData) {
    float vx = sledData->velocityX;
    float vy = sledData->velocityY;
    float vz = sledData->velocityZ;

    float speed_mps = sqrtf(vx*vx + vy*vy + vz*vz);
    float speed_kmh = speed_mps * 3.6f;

    return speed_kmh;
}

void print_sled_data(const struct SledData* sledData) {
    printf("isRaceOn: %d\n", sledData->isRaceOn);
    printf("timestampMS: %u\n", sledData->timestampMS);
    printf("engineMaxRpm: %.2f\n", sledData->engineMaxRpm);
    printf("engineIdleRpm: %.2f\n", sledData->engineIdleRpm);
    printf("currentEngineRpm: %.2f\n", sledData->currentEngineRpm);

    printf("accelerationX: %.3f\n", sledData->accelerationX);
    printf("accelerationY: %.3f\n", sledData->accelerationY);
    printf("accelerationZ: %.3f\n", sledData->accelerationZ);

    printf("velocityX: %.3f\n", sledData->velocityX);
    printf("velocityY: %.3f\n", sledData->velocityY);
    printf("velocityZ: %.3f\n", sledData->velocityZ);

    printf("angularVelocityX: %.3f\n", sledData->angularVelocityX);
    printf("angularVelocityY: %.3f\n", sledData->angularVelocityY);
    printf("angularVelocityZ: %.3f\n", sledData->angularVelocityZ);

    printf("yaw: %.3f\n", sledData->yaw);
    printf("pitch: %.3f\n", sledData->pitch);
    printf("roll: %.3f\n", sledData->roll);

    printf("normSuspensionTravelFL: %.3f\n", sledData->normSuspensionTravelFL);
    printf("normSuspensionTravelFR: %.3f\n", sledData->normSuspensionTravelFR);
    printf("normSuspensionTravelRL: %.3f\n", sledData->normSuspensionTravelRL);
    printf("normSuspensionTravelRR: %.3f\n", sledData->normSuspensionTravelRR);

    printf("tireSlipRatioFL: %.3f\n", sledData->tireSlipRatioFL);
    printf("tireSlipRatioFR: %.3f\n", sledData->tireSlipRatioFR);
    printf("tireSlipRatioRL: %.3f\n", sledData->tireSlipRatioRL);
    printf("tireSlipRatioRR: %.3f\n", sledData->tireSlipRatioRR);

    printf("tireSlipAngleFL: %.3f\n", sledData->tireSlipAngleFL);
    printf("tireSlipAngleFR: %.3f\n", sledData->tireSlipAngleFR);
    printf("tireSlipAngleRL: %.3f\n", sledData->tireSlipAngleRL);
    printf("tireSlipAngleRR: %.3f\n", sledData->tireSlipAngleRR);

    printf("tireCombinedSlipFL: %.3f\n", sledData->tireCombinedSlipFL);
    printf("tireCombinedSlipFR: %.3f\n", sledData->tireCombinedSlipFR);
    printf("tireCombinedSlipRL: %.3f\n", sledData->tireCombinedSlipRL);
    printf("tireCombinedSlipRR: %.3f\n", sledData->tireCombinedSlipRR);

    printf("suspensionTravelMetersFL: %.3f\n", sledData->suspensionTravelMetersFL);
    printf("suspensionTravelMetersFR: %.3f\n", sledData->suspensionTravelMetersFR);
    printf("suspensionTravelMetersRL: %.3f\n", sledData->suspensionTravelMetersRL);
    printf("suspensionTravelMetersRR: %.3f\n", sledData->suspensionTravelMetersRR);

    printf("wheelSpeedFL: %.3f\n", sledData->wheelSpeedFL);
    printf("wheelSpeedFR: %.3f\n", sledData->wheelSpeedFR);
    printf("wheelSpeedRL: %.3f\n", sledData->wheelSpeedRL);
    printf("wheelSpeedRR: %.3f\n", sledData->wheelSpeedRR);

    printf("throttle: %.3f\n", sledData->throttle);
    printf("steer: %.3f\n", sledData->steer);
    printf("brake: %.3f\n", sledData->brake);
    printf("clutch: %.3f\n", sledData->clutch);

    printf("gear: %.3f\n", sledData->gear);
    printf("gForceLat: %.3f\n", sledData->gForceLat);
    printf("gForceLong: %.3f\n", sledData->gForceLong);

    printf("lap: %.3f\n", sledData->lap);
    printf("racePosition: %.3f\n", sledData->racePosition);
    printf("accel: %.3f\n", sledData->accel);
}

int main() {
    int datagram_socket;
    address server_address;
    socklen_t server_address_len;
    struct SledData *sledData;
    size_t bytes_read;

    datagram_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(datagram_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("bind");
        exit(1);
    }

    sledData = malloc(sizeof(struct SledData));

    if (sledData == NULL) {
        perror("malloc");
        return -1; // Esci o gestisci l'errore
    }

    while (1) {
        printf("\033[2J\033[H"); // Clear screen

        ssize_t bytes_read = recv(datagram_socket, sledData, sizeof(struct SledData), 0);
        if (bytes_read < 0) {
            printf("Error receiving data\n");
            perror("recv");
            continue;
        }

        if (sledData->isRaceOn == 0) {
            continue;
        }

        float speed_kmh = calculate_speed_kmh(sledData);
        // Seleziona colore in base agli RPM
        const char* color = rpm_color(sledData->currentEngineRpm);

        // Stampa colore + testo + reset colore
        printf("%sEngine RPM: %.0f //  \033[0m\n",
               color, sledData->currentEngineRpm);
        printf("Speed: %.0f km/h //\n\n", speed_kmh);
        print_sled_data(sledData);
    }


    free(sledData);
    return 0;
}