#pragma once

constexpr int WORK_LENGTH = 10;
constexpr int BREAK_LENGTH = 2;

class Renderer;

class Pomodoro {
public:
    enum class State {W_RUN, W_END, W_PAUSE, B_RUN, B_END, B_PAUSE};
    Pomodoro() = default;
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

