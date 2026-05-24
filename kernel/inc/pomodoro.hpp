//
// Created by jesper on 22.05.26.
//

#ifndef KARNEL_POMODORO_H
#define KARNEL_POMODORO_H

constexpr int WORK_LENGTH = 5;
constexpr int BREAK_LENGTH = 2;

class Renderer;

class Pomodoro {
public:
    enum class State {W_RUN, W_END, W_PAUSE, B_RUN, B_END, B_PAUSE};
    Pomodoro(Renderer &render);

    void process_run();

    [[noreturn]] void run();

    void handle_kbd(char input);

private:
    Renderer *renderer_;
    int work_cycles_completed_{0};
    int remaining_time_sec_{WORK_LENGTH};
    State state_{State::W_RUN};
};


#endif //KARNEL_POMODORO_H
