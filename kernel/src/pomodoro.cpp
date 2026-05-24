#include "../inc/pomodoro.hpp"
#include "../inc/renderer.hpp"
#include "../inc/pit.hpp"

Pomodoro::Pomodoro(Renderer &render): renderer_(&render) {}

void Pomodoro::process_run() {
    if (remaining_time_sec_ > 0) {
        renderer_->render(state_, remaining_time_sec_);
        remaining_time_sec_--;
    } else {
        state_ =  (state_ == State::W_RUN) ? State::W_END : State::B_END;
        renderer_->render(state_, remaining_time_sec_);

    }
}

[[noreturn]] void Pomodoro::run() {
    while (true) {
        switch (state_) {
            case State::W_RUN:
                process_run();
                break;
            case State::W_END:
                break;
            case State::W_PAUSE:
                break;
            case State::B_RUN:
                process_run();
                break;
            case State::B_END:
                break;
            case State::B_PAUSE: // Ootab, kuni kbd interrupt vahetab statei tagasi B_WORK
                break;
        }
        pit_sleep_ms(1000);
    }
}

void Pomodoro::handle_kbd(char input) {
    if (input != 'u') {
        return;
    }

    switch (state_) {
        case State::W_RUN:
            state_ = State::W_PAUSE;
            break;
        case State::W_END:
            remaining_time_sec_ = BREAK_LENGTH;
            work_cycles_completed_++;
            state_ = State::B_RUN;
            break;
        case State::W_PAUSE:
            state_ = State::W_RUN;
            break;
        case State::B_RUN:
            state_ = State::B_PAUSE;
            break;
        case State::B_END:
            remaining_time_sec_ = WORK_LENGTH;
            state_ = State::W_RUN;
            break;
        case State::B_PAUSE:
            state_ = State::B_RUN;
            break;
    }
    renderer_->render(state_, remaining_time_sec_);
}

