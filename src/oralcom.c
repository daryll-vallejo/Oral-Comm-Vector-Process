#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double source;
    double receiver;
    double noise;
    double feedback;
} OralCom;

OralCom oralcom;

// Calculate the Spearman Rank Correlation Coefficient (SRCC)
double SRCC(double x, double y, int n) {
    double d_sqrd = pow(y - x, 2);
    double p = 6 * d_sqrd;
    p /= (double) n * (pow(n, 2) - 1);
    return 1 - p;
}

// Generate a random number from the Gaussian distribution
double gaussian_distribution() {
    double u1 = ((double) rand() / RAND_MAX);
    double u2 = ((double) rand() / RAND_MAX);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

// Simulate the feedback process and average by epoch
void simulate_feedback(double alpha, double beta, double gamma, double delta, double sigma_x, double sigma_y, int word_count, int receiver_word_count, double dt) {
    double x = oralcom.source;
    double y = oralcom.noise;
    double feedback_sum = 0.0;
    double iter = (y / (SRCC(word_count, receiver_word_count, word_count + receiver_word_count))) / dt; // Iteration is scaled word count based on the interface and time step
    int epoch = (int) round(iter);

    printf("epoch\t source(t)\t noise(t)\n");
    for (int i = 0; i <= epoch; i++) {
        printf("%d\t %.5f\t %.5f\n", i, x, y);
        
        // Generate random values for the Wiener process
        double epsilon_x = gaussian_distribution();
        double epsilon_y = gaussian_distribution();
        
        // Update x and y using a stochastic model
        x += (alpha * x - beta * x * x) * dt + sigma_x * x * sqrt(dt) * epsilon_x;
        y += (gamma * y - delta * y * y) * dt + sigma_y * y * sqrt(dt) * epsilon_y;
        
        // Update feedback based on the final x value and noise level
        double feedback_intensity = 1.0 / (1.0 + y); // Inverse relationship
        double feedback = x * feedback_intensity;
        
        // Accumulate feedback for averaging
        feedback_sum += feedback;
    }

    // Compute the average feedback
    oralcom.feedback = feedback_sum / (epoch + 1);
}

// Update the receiver's message
void update_receiver(int receiver_word_count) {
    double feedback_effect = oralcom.feedback * 0.5; // Adjust the impact of feedback
    oralcom.receiver = oralcom.source + feedback_effect;
    oralcom.receiver += ((double)receiver_word_count + ((double)receiver_word_count * pow(oralcom.feedback, 2)) / oralcom.source) / 100;
    oralcom.receiver += SRCC(oralcom.source, oralcom.receiver, receiver_word_count);
}

// Function to wait for a given number of seconds
void wait(int seconds) {
    time_t start_time = time(NULL);
    while (difftime(time(NULL), start_time) < seconds) {
        // Busy-wait loop
    }
}
