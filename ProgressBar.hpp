#ifndef PROGRESSBAR_PROGRESSBAR_HPP
#define PROGRESSBAR_PROGRESSBAR_HPP

#include <chrono>
#include <iostream>

class ProgressBar {
private:
    unsigned int ticks = 0;
    float speed; // milliseconds per tick

    const unsigned int total_ticks;
    const unsigned int bar_width;
    const char complete_char = '=';
    const char incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

public:
    ProgressBar(unsigned int total, unsigned int width, char complete, char incomplete) :
            total_ticks {total}, bar_width {width}, complete_char {complete}, incomplete_char {incomplete} {}

    ProgressBar(unsigned int total, unsigned int width) : total_ticks {total}, bar_width {width} {}

    unsigned int operator++() { return ++ticks; }

    unsigned int percentage() const { return (float) ticks / total_ticks * 100; };

    auto estimated_runtime() const {
       return std::chrono::milliseconds( (int) (speed*total_ticks) );
    };

    auto time_remaining() const {
       return std::chrono::milliseconds( (int) (speed*(total_ticks-ticks)) );
    };

    void display()
    {
        float progress = (float) ticks / total_ticks;
        int pos = (int) (bar_width * progress);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start_time).count();
	speed = time_elapsed / (float) ticks; // milliseconds per tick

        std::cout << "[";

        for (int i = 0; i < bar_width; ++i) {
            if (i < pos) std::cout << complete_char;
            else if (i == pos) std::cout << ">";
            else std::cout << incomplete_char;
        }
        std::cout << "] " << int(progress * 100.0) << "% "
                  << float(time_elapsed) / 1000.0
		  << "/" << std::chrono::duration_cast<std::chrono::seconds>(estimated_runtime()).count() << "s\r";
        std::cout.flush();
    }

    void done()
    {
        display();
        std::cout << std::endl;
    }
};

#endif //PROGRESSBAR_PROGRESSBAR_HPP
