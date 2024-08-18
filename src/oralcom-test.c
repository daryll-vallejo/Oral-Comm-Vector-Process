#include "oralcom.c"

int main() {
    srand(time(NULL)); // Seed the random number generator once

    int receiver_word_count = 7; // Example word count
    int word_count = 7; // Source word count
    double dt = 0.01; // Time step

    oralcom.source = 1.028842; // Initial source value
    oralcom.receiver = 0.0; // Initialize receiver
    oralcom.noise = 0.0669942; // Low noise level
    oralcom.feedback = 0.0; // Initialize feedback

    printf("source(%lf): \"How do we solve this quadratic equation?\"\n", oralcom.source);
    wait(1);

    printf("noise(%lf): psychological noise\n", oralcom.noise);
    wait(1);
    
    // Simulate feedback process
    simulate_feedback(0.1, 0.015, 0.2, 0.02, 0.3, 0.15, word_count, receiver_word_count, dt);
    
    // Update the receiver with the feedback
    update_receiver(receiver_word_count);
    
    printf("receiver(%lf): decoded\n", oralcom.receiver);
    wait(1);
    
    printf("feedback(%lf): \"The solution involves using the quadratic formula.\"\n", oralcom.feedback);
    
    return 0;
}
