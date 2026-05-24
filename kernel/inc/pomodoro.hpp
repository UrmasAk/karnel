//
// Created by jesper on 22.05.26.
//

#ifndef KARNEL_POMODORO_H
#define KARNEL_POMODORO_H
#include "pit.hpp"
#include "renderer.hpp"

constexpr int WORK_LENGTH = 20*60;
constexpr int BREAK_LENGTH = 5*60;

class Pomodoro {
public:
    enum class State {W_RUN, W_END, W_PAUSE, B_RUN, B_END, B_PAUSE};
    Pomodoro() = default;

    [[noreturn]] void run() {
        while (true) {
            switch (state_) {
            case (State::W_RUN): {
                if (remaining_time_sec_ > 0) {
                    renderer_->render(state_, remaining_time_sec_);
                    remaining_time_sec_--;
                } else {
                    state_ = State::W_END;
                    continue;
                }
            }
                
            }
            pit_sleep_ms(1000);
        }
    }

    void handle_kbd(char input) {
        state_ = State::W_PAUSE;
        run();
    }

private:

    Renderer *renderer_;
    int work_cycles_completed_{0};
    int remaining_time_sec_{WORK_LENGTH};
    Pomodoro::State state_{State::W_RUN};
};


#endif //KARNEL_POMODORO_H
