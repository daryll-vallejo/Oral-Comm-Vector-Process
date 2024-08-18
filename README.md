
# Oral Communication Vector Process - Simple Beta Project

## Overview

This project simulates a communication process where a source message is affected by noise and feedback. The primary goal is to model the feedback process and its impact on message clarity using stochastic methods.

## Code Explanation

### Includes and Typedef

```c
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
```

The code starts by including necessary libraries for input/output, mathematical functions, and time handling. It defines a `struct` to hold the parameters of the communication process.

### Spearman Rank Correlation Coefficient (SRCC)

```c
double SRCC(double x, double y, int n) {
    double d_sqrd = pow(y - x, 2);
    double p = 6 * d_sqrd;
    p /= (double) n * (pow(n, 2) - 1);
    return 1 - p;
}
```

This function calculates the Spearman Rank Correlation Coefficient (SRCC), which measures the rank correlation between two variables. The formula used is:

##### SRCC = 1 - (6 * Σd^2) / (n * (n^2 - 1))

Here, `d^2` represents the squared difference between ranks, `n` is the number of observations, and the formula adjusts the correlation based on ranking discrepancies.

### Gaussian Distribution

```c
double gaussian_distribution() {
    double u1 = ((double) rand() / RAND_MAX);
    double u2 = ((double) rand() / RAND_MAX);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}
```

This function generates random numbers following a Gaussian distribution using the Box-Muller transform. It converts uniformly distributed random variables into a Gaussian distribution, useful for simulating noise or other stochastic processes.

### Feedback Simulation

```c
void simulate_feedback(double alpha, double beta, double gamma, double delta, double sigma_x, double sigma_y, int word_count, int receiver_word_count, double dt) {
    double x = oralcom.source;
    double y = oralcom.noise;
    double feedback_sum = 0.0;
    double iter = (y / (SRCC(word_count, receiver_word_count, word_count + receiver_word_count))) / dt;
    int epoch = (int) round(iter);

    printf("epoch\t source(t)\t noise(t)\n");
    for (int i = 0; i <= epoch; i++) {
        printf("%d\t %.5f\t %.5f\n", i, x, y);

        double epsilon_x = gaussian_distribution();
        double epsilon_y = gaussian_distribution();

        x += (alpha * x - beta * x * x) * dt + sigma_x * x * sqrt(dt) * epsilon_x;
        y += (gamma * y - delta * y * y) * dt + sigma_y * y * sqrt(dt) * epsilon_y;

        double feedback_intensity = 1.0 / (1.0 + oralcom.noise);
        double feedback = x * feedback_intensity;

        feedback_sum += feedback;
    }

    oralcom.feedback = feedback_sum / (epoch + 1);
}
```

This function simulates the feedback process using a stochastic model. The feedback is accumulated and averaged over epochs to update the `oralcom.feedback` value. The updates to `x` and `y` are based on stochastic differential equations:

##### dx = (alpha * x - beta * x^2) * dt + sigma_x * x * dW_x
##### dy = (gamma * y - delta * y^2) * dt + sigma_y * y * dW_y

Here, `dW_x` and `dW_y` represent Gaussian noise terms. The feedback intensity is inversely related to noise, and the feedback is updated accordingly.

### Receiver Update

```c
void update_receiver(int receiver_word_count) {
    double feedback_effect = oralcom.feedback * 0.5;
    oralcom.receiver = oralcom.source + feedback_effect;
    oralcom.receiver += ((double)receiver_word_count + ((double)receiver_word_count * pow(oralcom.feedback, 2)) / oralcom.source) / 100;
    oralcom.receiver += SRCC(oralcom.source, oralcom.receiver, receiver_word_count);
}
```

This function updates the receiver's message based on the feedback received. It adjusts the receiver's value using a combination of feedback intensity and correlation metrics. The update is given by:

##### receiver = source + 0.5 * feedback + (receiver_word_count + (receiver_word_count * feedback^2) / source) / 100 + SRCC

## Limitations

The provided code is in a beta stage and has the following limitations:

- **Accuracy:** The feedback model and simulation might not perfectly represent real-world communication processes due to simplifications and assumptions.
- **Noise Handling:** The model assumes a static noise level, which might not reflect varying real-world conditions. Real systems may experience dynamic changes in noise levels.
- **Scaling:** The code might not scale well with larger data sets or more complex communication scenarios without significant adjustments. The current implementation is designed for simplicity.
- **Performance:** The use of a busy-wait loop for delays is inefficient and could be improved with better timing mechanisms. This may impact performance in more complex simulations.
- **Model Assumptions:** Assumptions such as constant parameters and simple stochastic models may not capture all aspects of real-world systems, requiring more sophisticated models for accurate predictions.
- 
